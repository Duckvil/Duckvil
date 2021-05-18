#include "Engine/Duckvil.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "Process/Process.h"

#include "HotReloader/FileWatcher.h"

#include "Editor/Widgets/HexEditorWidgetEvent.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Memory/SmartPointer/UniquePointer.h"

#include "Logger/Logger.h"

#include "Memory/ByteBuffer.h"

#undef max
#undef GetObject
#undef allocator
#undef GetMessage

namespace Duckvil {

    bool init_runtime_reflection(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

        _pModule->load(&_runtimeReflectionModule);

        duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
        duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_runtimeReflectionInit);
        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", (void**)&_runtimeReflectionRecorderInit);

        _pData->m_pRuntimeReflection = _runtimeReflectionInit();
        _pData->m_pRuntimeReflectionRecorder = _runtimeReflectionRecorderInit();
        _pData->m_pRuntimeReflectionData = _pData->m_pRuntimeReflection->m_fnInit(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pRuntimeReflection);

        RuntimeReflection::make_current({ _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData, _pData->m_pRuntimeReflectionRecorder });

        _pData->m_pRuntimeReflectionData->m_pEvents = _pData->m_heap.Allocate<Event::Pool<Event::mode::immediate>>(_pData->m_heap);

        return true;
    }

    bool init_logger(__data* _pData, PlugNPlay::__module* _pModule)
    {
        logger_ftable(*_duckvilLoggerInit)();

        PlugNPlay::__module_information _loggerModule("Logger");

        _pModule->load(&_loggerModule);
        _pModule->get(_loggerModule, "duckvil_logger_init", (void**)&_duckvilLoggerInit);

        _pData->_loggerFTable = _duckvilLoggerInit();
        _pData->_loggerData = _pData->_loggerFTable.m_fnInitLogger(_pData->m_heap);

        {
            PlugNPlay::__module _module;

            PlugNPlay::module_init(&_module);

            PlugNPlay::__module_information _loggerModule("Logger");

            _module.load(&_loggerModule);

            duckvil_logger_channel_init_callback _loggerInit;

            _module.get(_loggerModule, "duckvil_logger_channel_init", (void**)&_loggerInit);

            _pData->m_pLoggerChannel = _loggerInit();

            logger_make_current({ _pData->_loggerFTable, _pData->_loggerData, _pData->m_pLoggerChannel });
        }

        static std::string _outLog = (std::filesystem::path(DUCKVIL_OUTPUT) / "log.log").string();

        // _pData->m_logger = LoggerChannel(_pData->m_heap, _outLog.c_str(), _outLog.length(), (__logger_channel_flags)(__logger_flags_console_output | __logger_flags_file_output | __logger_flags_editor_console_output));

        _pData->m_pLoggerChannelData = logger_add(
            _pData->m_heap,
            { 
                _outLog.c_str(),
                _outLog.length(),
                (__logger_channel_flags)(__logger_flags_console_output | __logger_flags_file_output | __logger_flags_editor_console_output)
            },
            LoggerChannelID::Default
        );

        return true;
    }

    bool init_threading(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _threadModule("Thread");

        _pModule->load(&_threadModule);

        Thread::pool_ftable* (*init)();

        _pModule->get(_threadModule, "duckvil_thread_pool_init", (void**)&init);

        _pData->m_pThread = init();
        _pData->m_pThreadData = _pData->m_pThread->m_fnInit(_pData->m_heap);

        _pData->m_pThread->m_fnStart(_pData->m_pThreadData);

        RuntimeReflection::record_meta(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, RuntimeReflection::get_type<__data>(), "Thread", _pData->m_pThreadData);

        return true;
    }

    // bool init_renderer(__data* _pData, PlugNPlay::__module* _pModule)
    // {
    //     PlugNPlay::__module_information _rendererModule("Renderer");

    //     _pModule->load(&_rendererModule);

    //     Renderer::RendererFTable* (*init)(Memory::ftable*, Memory::free_list_allocator*);

    //     _pModule->get(_rendererModule, "duckvil_renderer_init", (void**)&init);

    //     _pData->m_pRenderer = init(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator());

    //     _pData->m_pRenderer->m_fnInitRenderer(&_pData->m_renderData, _pData->m_pRenderer, _pData->m_pWindow);

    //     return true;
    // }

    bool init_editor(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _editorModule("Editor");

        _pModule->load(&_editorModule);

        Editor::EditorFTable* (*init)();

        _pModule->get(_editorModule, "duckvil_editor_init", (void**)&init);

        _pData->m_pEditor = init();

        _pData->m_pEditorData = (Editor::ImGuiEditorData*)_pData->m_pEditor->m_fnInit(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), _pData->m_pWindow);

        return true;
    }

    bool init(__data* _pData, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator)
    {
        _pData->m_pHeap = _pAllocator;
        _pData->m_pMemory = _pMemoryInterface;
        _pData->m_aLoadedModules = nullptr;
        _pData->m_uiLoadedModulesCount = 0;
        _pData->m_dOneSecond = 0;
        _pData->m_time = time_init();
        _pData->m_heap = Memory::FreeList(_pData->m_pMemory, _pData->m_pHeap);

        _pData->m_heap.Allocate(_pData->m_objectsHeap, 1024);
        _pData->m_heap.Allocate(_pData->m_eventsHeap, 1024 * 2);
        _pData->m_heap.Allocate(_pData->m_aEngineSystems, 1);
        _pData->m_heap.Allocate(_pData->m_aRecordedTypes, 1);

        DUCKVIL_DEBUG_MEMORY(_pData->m_objectsHeap.GetAllocator(), "m_objectsHeap");
        DUCKVIL_DEBUG_MEMORY(_pData->m_eventsHeap.GetAllocator(), "m_eventsHeap");
        DUCKVIL_DEBUG_MEMORY(_pData->m_aEngineSystems.GetAllocator(), "m_aEngineSystems");
        DUCKVIL_DEBUG_MEMORY(_pData->m_aRecordedTypes.GetAllocator(), "m_aRecordedTypes");

        _pData->m_time.init(&_pData->m_timeData);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        init_runtime_reflection(_pData, &_module);

        _pData->m_eventPool = Event::Pool<Event::mode::immediate>(_pData->m_eventsHeap, _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData);
        _pData->m_windowEventPool = Event::Pool<Event::mode::buffered>(_pData->m_eventsHeap, _pData->m_pRuntimeReflectionData);

        PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

        _autoLoader.LoadAll(_pMemoryInterface, _pAllocator, &_pData->m_aLoadedModules, &_pData->m_uiLoadedModulesCount);

        // DUCKVIL_LOG_INFO_("Modules to load %i", _pData->m_uiLoadedModulesCount);

        for(uint32_t i = 0; i < _pData->m_uiLoadedModulesCount; ++i)
        {
            const PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];
            uint32_t (*get_recorder_count)();
            void (*make_current_runtime_reflection_context)(const duckvil_frontend_reflection_context&);
            // void (*make_current_logger_context)(const logger_channel_context&);

            make_current_runtime_reflection_context = nullptr;

            _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorder_count", (void**)&get_recorder_count);
            _module.get(_loadedModule, "duckvil_plugin_make_current_runtime_reflection_context", (void**)&make_current_runtime_reflection_context);
            // _module.get(_loadedModule, "duckvil_plugin_make_current_logger_context", (void**)&make_current_logger_context);

            if(get_recorder_count == nullptr)
            {
                // DUCKVIL_LOG_INFO_("No recorder for %s", _loadedModule.m_sName.m_sText);

                continue;
            }

            // DUCKVIL_LOG_INFO_("Module %s is present", _loadedModule.m_sName.m_sText);

            uint32_t _recordersCount = get_recorder_count();

            for(uint32_t j = 0; j < _recordersCount; ++j)
            {
                duckvil_recorderd_types (*record)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, RuntimeReflection::__recorder_ftable* _pRecorder, RuntimeReflection::__ftable* _pRuntimeReflection, RuntimeReflection::__data* _pData);

                _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), (void**)&record);

                if(record == nullptr)
                {
                    // TODO: Should return false?

                    continue;
                }

                if(_pData->m_aRecordedTypes.Full())
                {
                    _pData->m_aRecordedTypes.Resize(_pData->m_aRecordedTypes.Size() * 2);
                }

                _pData->m_aRecordedTypes.Allocate(record(_pMemoryInterface, _pAllocator, _pData->m_pRuntimeReflectionRecorder, _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData));
            }

            if(make_current_runtime_reflection_context != nullptr)
            {
                make_current_runtime_reflection_context(RuntimeReflection::get_current());
            }
        }

        init_logger(_pData, &_module);
        init_threading(_pData, &_module);

        RuntimeReflection::record_meta(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, RuntimeReflection::get_type<__data>(), "Time", &(_pData->m_timeData));

        for(uint32_t i = 0; i < _pData->m_uiLoadedModulesCount; ++i)
        {
            const PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];

            void (*make_current_logger_context)(const logger_context&);

            _module.get(_loadedModule, "duckvil_plugin_make_current_logger_context", (void**)&make_current_logger_context);

            if(make_current_logger_context != nullptr)
            {
                make_current_logger_context(logger_get_current());
            }
        }

        _pData->m_bRunning = false;

        RuntimeReflection::ReflectedType<> _windowType(_pData->m_heap, RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, "WindowSDL", "Duckvil", "Window"));

        _pData->m_pWindow = (Window::IWindow*)_windowType.Create<Event::Pool<Event::mode::buffered>*>(&_pData->m_windowEventPool);

        _pData->m_pWindow->Create("Duckvil", 1920, 1080);

        // init_renderer(_pData, &_module);
        init_editor(_pData, &_module);

        {
            auto _types = RuntimeReflection::get_types(_pData->m_heap);

            {
                auto _runtimeCompilerType = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>();

                RuntimeReflection::ReflectedType<HotReloader::RuntimeCompilerSystem> _type(_pData->m_heap);

                _pData->m_pRuntimeCompiler = (HotReloader::RuntimeCompilerSystem*)_type.Create<
                    const Memory::FreeList&,
                    Event::Pool<Event::mode::immediate>*
                >(
                    (const Memory::FreeList&)_pData->m_heap,
                    &_pData->m_eventPool
                );

                ((Event::Pool<Event::mode::immediate>*)_pData->m_pRuntimeReflectionData->m_pEvents)->Add<RuntimeReflection::TrackedObjectCreatedEvent>(_pData->m_pRuntimeCompiler);

                _pData->m_fnRuntimeCompilerUpdate = _type.GetFunctionCallback<ISystem>("Update")->m_fnFunction;
                _pData->m_fnRuntimeCompilerInit = _type.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction;

                _pData->m_pRuntimeCompiler->m_aRecordedTypes = _pData->m_aRecordedTypes;

                _pData->m_pEditor->m_fnAddDraw(_pData->m_pEditorData,
                    Editor::Draw
                    {
                        _type.GetFunctionCallback<Editor::Widget>("OnDraw")->m_fnFunction,
                        _type.GetFunctionCallback<Editor::Widget, void*>("InitEditor")->m_fnFunction,
                        _pData->m_pRuntimeCompiler,
                        _runtimeCompilerType
                    }
                );

                _type.Invoke<const Memory::FreeList&>("SetObjectsHeap", _pData->m_pRuntimeCompiler, _pData->m_objectsHeap);
            }

            for(uint32_t i = 0; i < _types.Size(); ++i)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _typeHandle = _types[i];
                const RuntimeReflection::__variant& _variant = RuntimeReflection::get_meta(_typeHandle, ReflectionFlags::ReflectionFlags_UserSystem);

                if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() &&
                    (uint8_t)_variant.m_traits & (uint8_t)RuntimeReflection::property_traits::is_bool)
                {
                    if(_pData->m_aEngineSystems.Full())
                    {
                        _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                    }

                    RuntimeReflection::ReflectedType<> _type(_pData->m_heap, _typeHandle);

                    void* _testSystem = _type.CreateTracked<
                        const Memory::FreeList&
                    >(
                        _pData->m_heap
                    );

                    system _system = {};

                    _system.m_type = _typeHandle;
                    _system.m_pTrackKeeper = DUCKVIL_TRACK_KEEPER_CAST(ISystem, _testSystem);
                    _system.m_fnUpdateCallback = _type.GetFunctionCallback<ISystem>("Update")->m_fnFunction;
                    _system.m_fnInitCallback = _type.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction;

                    if(RuntimeReflection::get_meta(_typeHandle, ReflectionFlags_AutoEventsAdding).m_ullTypeID != -1)
                    {
                        const auto& _functions = RuntimeReflection::get_functions(_pData->m_heap, _typeHandle);

                        for(const auto& _functionHandle : _functions)
                        {
                            const auto& _func = RuntimeReflection::get_function(_typeHandle, _functionHandle);

                            if(_func.m_ullArgumentsTypeID == DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(const HotReloader::SwapEvent&) && strcmp(_func.m_sFunctionName, "OnEvent") == 0)
                            {
                                _pData->m_eventPool.Add<HotReloader::SwapEvent>(DUCKVIL_TRACK_KEEPER_CAST(void, _testSystem), _typeHandle);
                            }
                        }
                    }

                    _pData->m_aEngineSystems.Allocate(_system);

                    if(RuntimeReflection::inherits<Editor::Widget>(_typeHandle))
                    {
#ifdef DUCKVIL_HOT_RELOADING
                        _pData->m_pEditor->m_fnAddHotDraw(_pData->m_pEditorData,
                            Editor::HotDraw
                            {
                                _type.GetFunctionCallback<Editor::Widget>("OnDraw")->m_fnFunction,
                                _type.GetFunctionCallback<Editor::Widget, void*>("InitEditor")->m_fnFunction,
                                DUCKVIL_TRACK_KEEPER_CAST(Editor::Widget, _testSystem), _typeHandle
                            }
                        );
#else
                        _pData->m_pEditor->m_fnAddDraw(_pData->m_pEditorData,
                            Editor::Draw
                            {
                                _type.GetFunctionCallback<Editor::Widget>("OnDraw")->m_fnFunction,
                                _type.GetFunctionCallback<Editor::Widget, void*>("InitEditor")->m_fnFunction,
                                _testSystem, _typeHandle
                            }
                        );
#endif
                    }

                    _pData->m_eventPool.AddA<HotReloader::SwapEvent>([_pData, _typeHandle](const HotReloader::SwapEvent& _event)
                    {
                        for(uint32_t j = 0; j < _pData->m_aEngineSystems.Size(); ++j)
                        {
                            system& _system = _pData->m_aEngineSystems[j];

                            if(_system.m_type.m_ID == _event.m_pTrackKeeper->GetTypeHandle().m_ID)
                            {
                                RuntimeReflection::ReflectedType<> _systemType(_pData->m_heap, _system.m_type);

                                _system.m_fnUpdateCallback = _systemType.GetFunctionCallback<ISystem>("Update")->m_fnFunction;
                                _system.m_fnInitCallback = _systemType.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction;
                            }
                        }
                    });

                    if(!((ISystem*)DUCKVIL_TRACK_KEEPER_GET_OBJECT(_system.m_pTrackKeeper)->*_system.m_fnInitCallback)())
                    {
                        return false;
                    }
                }
            }

            _pData->m_pEditor->m_fnPostInit(_pData->m_heap, _pData->m_pEditorData, _pData->m_pEditor, &_pData->m_eventPool, _pData->m_pRuntimeCompiler);

            if(!(_pData->m_pRuntimeCompiler->*_pData->m_fnRuntimeCompilerInit)())
            {
                return false;
            }
        }

        return true;
    }

    bool start(__data* _pData, __ftable* _pFTable)
    {
        if(_pData->m_bRunning)
        {
            return false;
        }

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _pData->m_pEditorData->m_pEditorEvents.Broadcast(Editor::HexEditorWidgetInitEvent{ _pData->m_pMemoryDebugger });
#endif
        _pData->m_bRunning = true;

        while(_pData->m_bRunning)
        {
            _pFTable->update(_pData, _pFTable);
        }

        return true;
    }

    bool stop(__data* _pData, __ftable* _pFTable)
    {
        if(!_pData->m_bRunning)
        {
            return false;
        }

        _pData->m_bRunning = false;

        return true;
    }

#ifdef DUCKVIL_MEMORY_DEBUGGER
    void recursive(duckvil_memory_debug_info* _pDebugInfo, uint32_t& _index)
    {
        for(uint32_t i = 0; i < _index; ++i)
        {
            printf(" ");
        }

        if(_pDebugInfo->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_free_list)
        {
            printf("Free list %s\n", _pDebugInfo->m_aLabel);
        }
        else if(_pDebugInfo->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_linear)
        {
            printf("Linear %s\n", _pDebugInfo->m_aLabel);
        }
        else if(_pDebugInfo->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_stack)
        {
            printf("Stack %s\n", _pDebugInfo->m_aLabel);
        }
        else if(_pDebugInfo->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_queue)
        {
            printf("Queue %s\n", _pDebugInfo->m_aLabel);
        }
        else if (_pDebugInfo->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_vector)
        {
            printf("Vector %s\n", _pDebugInfo->m_aLabel);
        }

        for(uint32_t i = 0; i < _index; ++i)
        {
            printf(" ");
        }

        printf("Size: %d\n", ((Memory::allocator*)_pDebugInfo->m_pAllocator)->m_ullUsed);

        for(uint32_t i = 0; i < _index; ++i)
        {
            printf(" ");
        }

        printf("Capacity: %d\n", ((Memory::allocator*)_pDebugInfo->m_pAllocator)->m_ullCapacity);

        _index++;

        for(const auto& a : _pDebugInfo->m_aOther)
        {
            recursive(a, _index);
        }

        _index--;
    }
#endif

    void update(__data* _pData, __ftable* _pFTable)
    {
        _pData->m_time.update(&_pData->m_timeData);

        _pData->m_dOneSecond += _pData->m_timeData.m_dDelta;

        for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); ++i)
        {
            system& _system = _pData->m_aEngineSystems[i];

            ((ISystem*)DUCKVIL_TRACK_KEEPER_GET_OBJECT(_system.m_pTrackKeeper)->*_system.m_fnUpdateCallback)();
        }

        if(_pData->m_ullLastTimeUsed != _pData->m_pHeap->m_ullUsed)
        {
            size_t _change = _pData->m_pHeap->m_ullUsed - _pData->m_ullLastTimeUsed;

            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "Memory change: %d", _change);

            _pData->m_ullLastTimeUsed = _pData->m_pHeap->m_ullUsed;
        }

        if(_pData->m_dOneSecond >= 1.0)
        {
#ifdef DUCKVIL_HOT_RELOADING
            (_pData->m_pRuntimeCompiler->*_pData->m_fnRuntimeCompilerUpdate)();
#endif

            // _pData->m_pLogger->dispatch_logs(/*_pData->m_pLogger, _pData->m_pLoggerData*/ logger_get_current().m_pLogger, logger_get_current().m_pLoggerData);
            // _pData->m_logger->Dispatch();
            _pData->m_pLoggerChannel->dispatch_logs(_pData->m_pLoggerChannel, _pData->m_pLoggerChannelData);

            // DUCKVIL_LOG_INFO("Delta: %f ms", _pData->m_timeData.m_dDelta * 1000.0);
            // DUCKVIL_LOG_INFO("Used memory: %d of %d", _pData->m_pHeap->m_ullUsed, _pData->m_pHeap->m_ullCapacity);

// #ifdef DUCKVIL_MEMORY_DEBUGGER
//             uint32_t _index = 0;
//             recursive(_pData->m_pMemoryDebugger, _index);
// #endif

            _pData->m_dOneSecond = 0.0;
        }

        while(_pData->m_windowEventPool.AnyEvents())
        {
            Window::CloseEvent _closeEvent;
            Window::ResizeEvent _resizeEvent;

            if(_pData->m_windowEventPool.GetMessage(&_closeEvent))
            {
                _pData->m_bRunning = false;

                _pData->m_windowEventPool.EventHandled<Window::CloseEvent>();
            }
            else if(_pData->m_windowEventPool.GetMessage(&_resizeEvent))
            {
                printf("RESIZE ME!!!!\n");

                _pData->m_windowEventPool.EventHandled<Window::ResizeEvent>();
            }
        }

        _pData->m_windowEventPool.Reset();

        // _pData->m_pRenderer->m_fnRender(&_pData->m_renderData, 0);

        _pData->m_pEditor->m_fnRender(_pData->m_pEditorData, _pData->m_pWindow);

        _pData->m_pWindow->Refresh();
    }

}

Duckvil::__ftable* duckvil_init()
{
    static Duckvil::__ftable _ftable = { 0 };

    _ftable.init = Duckvil::init;
    _ftable.start = Duckvil::start;
    _ftable.stop = Duckvil::stop;
    _ftable.update = Duckvil::update;

    return &_ftable;
}