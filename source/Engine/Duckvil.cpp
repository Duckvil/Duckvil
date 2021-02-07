#include "Engine/Duckvil.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "Process/Process.h"

#include "HotReloader/FileWatcher.h"

#undef max
#undef GetObject
#undef __allocator

namespace Duckvil {

    bool init_runtime_reflection(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

        _pModule->load(&_runtimeReflectionModule);

        duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
        duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_runtimeReflectionInit);
        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", (void**)&_runtimeReflectionRecorderInit);

        _pData->m_pRuntimeReflection = _runtimeReflectionInit(_pData->m_pMemory, _pData->m_pHeap);
        _pData->m_pRuntimeReflectionRecorder = _runtimeReflectionRecorderInit(_pData->m_pMemory, _pData->m_pHeap);
        _pData->m_pRuntimeReflectionData = _pData->m_pRuntimeReflection->m_fnInit(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pRuntimeReflection);

        return true;
    }

    bool init_logger(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _loggerModule("Logger");

        _pModule->load(&_loggerModule);

        duckvil_logger_init_callback _loggerInit;

        _pModule->get(_loggerModule, "duckvil_logger_init", (void**)&_loggerInit);

        _pData->m_pLogger = _loggerInit(_pData->m_pMemory, _pData->m_pHeap);
        _pData->m_pLoggerData = _pData->m_pLogger->init(_pData->m_pMemory, _pData->m_pHeap);

        std::string _outLog = (std::filesystem::path(DUCKVIL_OUTPUT) / "log.log").string();

        memcpy(_pData->m_pLoggerData->m_sPathFile, _outLog.c_str(), _outLog.size());
        _pData->m_pLoggerData->m_flags = static_cast<Logger::__logger_flags>(Logger::__logger_flags::__logger_flags_console_output | Logger::__logger_flags::__logger_flags_file_output);

        DUCKVIL_LOG_INFO_("Logger initialization complete!");
        DUCKVIL_LOG_INFO_("Logger file output: %s", _outLog.c_str());

        return true;
    }

    bool init(__data* _pData, Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator)
    {
        _pData->m_pHeap = _pAllocator;
        _pData->m_pMemory = _pMemoryInterface;
        _pData->m_aLoadedModules = nullptr;
        _pData->m_uiLoadedModulesCount = 0;
        _pData->m_dOneSecond = 0;
        _pData->m_time = time_init();
        _pData->m_heap = Memory::FreeList(_pData->m_pMemory, _pData->m_pHeap);

        _pData->m_heap.Allocate(_pData->m_objectsHeap, 1024 * 8);
        _pData->m_heap.Allocate(_pData->m_eventsHeap, 1024 * 8);
        _pData->m_heap.Allocate(_pData->m_aEngineSystems, 1);

        DUCKVIL_DEBUG_MEMORY(_pData->m_objectsHeap.GetAllocator(), "m_objectsHeap");
        DUCKVIL_DEBUG_MEMORY(_pData->m_eventsHeap.GetAllocator(), "m_eventsHeap");
        DUCKVIL_DEBUG_MEMORY(_pData->m_aEngineSystems.GetAllocator(), "m_aEngineSystems");

        _pData->m_time.init(&_pData->m_timeData);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        init_logger(_pData, &_module);
        init_runtime_reflection(_pData, &_module);

        _pData->m_eventPool = Event::Pool<Event::mode::immediate>(_pData->m_eventsHeap, _pData->m_pRuntimeReflectionData);

        PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

        _autoLoader.LoadAll(_pMemoryInterface, _pAllocator, &_pData->m_aLoadedModules, &_pData->m_uiLoadedModulesCount);

        DUCKVIL_LOG_INFO_("Modules to load %i", _pData->m_uiLoadedModulesCount);

        for(uint32_t i = 0; i < _pData->m_uiLoadedModulesCount; i++)
        {
            const PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];
            uint32_t (*get_recorder_count)();

            _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorder_count", (void**)&get_recorder_count);

            if(get_recorder_count == nullptr)
            {
                DUCKVIL_LOG_INFO_("No recorder for %s", _loadedModule.m_sName.m_sText);

                continue;
            }

            DUCKVIL_LOG_INFO_("Module %s is present", _loadedModule.m_sName.m_sText);

            _pData->m_ullRecordedTypesCount = get_recorder_count();
            _pData->m_aRecordedTypes = new duckvil_recorderd_types[_pData->m_ullRecordedTypesCount];

            for(uint32_t j = 0; j < _pData->m_ullRecordedTypesCount; j++)
            {
                duckvil_recorderd_types (*record)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, RuntimeReflection::__recorder_ftable* _pRecorder, RuntimeReflection::__ftable* _pRuntimeReflection, RuntimeReflection::__data* _pData);

                _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), (void**)&record);

                if(record == nullptr)
                {
                    // TODO: Should return false?

                    continue;
                }

                _pData->m_aRecordedTypes[j] = record(_pMemoryInterface, _pAllocator, _pData->m_pRuntimeReflectionRecorder, _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData);
            }
        }

        _pData->m_bRunning = false;

        {
            auto _types = RuntimeReflection::get_types(_pData->m_pRuntimeReflectionData, _pData->m_pMemory, _pData->m_pHeap);

            {
                auto _runtimeCompilerType = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>(_pData->m_pRuntimeReflectionData);

                RuntimeReflection::ReflectedType _type(_pData->m_pRuntimeReflectionData, _pData->m_heap, RuntimeReflection::tag<HotReloader::RuntimeCompilerSystem>{});

                _pData->m_pRuntimeCompiler = (HotReloader::RuntimeCompilerSystem*)_type.Create<
                    const Memory::FreeList&,
                    RuntimeReflection::__data*,
                    RuntimeReflection::__recorder_ftable*,
                    RuntimeReflection::__ftable*,
                    Event::Pool<Event::mode::immediate>*
                >(
                    (const Memory::FreeList&)_pData->m_heap,
                    _pData->m_pRuntimeReflectionData,
                    _pData->m_pRuntimeReflectionRecorder,
                    _pData->m_pRuntimeReflection,
                    &_pData->m_eventPool
                );

                // _pData->m_pRuntimeCompiler = (HotReloader::RuntimeCompilerSystem*)RuntimeReflection::create<
                //     const Memory::FreeList&,
                //     RuntimeReflection::__data*,
                //     RuntimeReflection::__recorder_ftable*,
                //     RuntimeReflection::__ftable*,
                //     Event::Pool<Event::mode::immediate>*
                // >(
                //     _pData->m_heap,
                //     _pData->m_pRuntimeReflectionData,
                //     _runtimeCompilerType,
                //     _pData->m_heap,
                //     _pData->m_pRuntimeReflectionData,
                //     _pData->m_pRuntimeReflectionRecorder,
                //     _pData->m_pRuntimeReflection,
                //     &_pData->m_eventPool
                // );

                // _pData->m_fnRuntimeCompilerUpdate = RuntimeReflection::get_function_callback<ISystem>(_pData->m_pRuntimeReflectionData, _runtimeCompilerType, "Update")->m_fnFunction;
                _pData->m_fnRuntimeCompilerUpdate = _type.GetFunctionCallback<ISystem>("Update")->m_fnFunction;
                // _pData->m_fnRuntimeCompilerInit = RuntimeReflection::get_function_callback<bool, ISystem>(_pData->m_pRuntimeReflectionData, _runtimeCompilerType, "Init")->m_fnFunction;
                _pData->m_fnRuntimeCompilerInit = _type.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction;

                _pData->m_pRuntimeCompiler->m_aRecordedTypes = _pData->m_aRecordedTypes;
                _pData->m_pRuntimeCompiler->m_ullRecordedTypesCount = _pData->m_ullRecordedTypesCount;

                // auto _func = RuntimeReflection::get_function_handle<const Memory::FreeList&>(_pData->m_pRuntimeReflectionData, _runtimeCompilerType, "SetObjectsHeap");
                // RuntimeReflection::invoke_member<const Memory::FreeList&>(_pData->m_pRuntimeReflectionData, _runtimeCompilerType, _func, _pData->m_pRuntimeCompiler, _pData->m_objectsHeap);

                _type.Invoke<const Memory::FreeList&>("SetObjectsHeap", _pData->m_pRuntimeCompiler, _pData->m_objectsHeap);
            }

            for(uint32_t i = 0; i < _types.Size(); i++)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _typeHandle = _types[i];
                const RuntimeReflection::__variant& _variant = RuntimeReflection::get_meta(_pData->m_pRuntimeReflectionData, _typeHandle, ReflectionFlags::ReflectionFlags_UserSystem);

                if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max())
                {
                    if((uint8_t)_variant.m_traits & (uint8_t)RuntimeReflection::__traits::is_bool)
                    {
                        if(_pData->m_aEngineSystems.Full())
                        {
                            _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                        }

                        RuntimeReflection::ReflectedType _type(_pData->m_pRuntimeReflectionData, _pData->m_heap, _typeHandle);

                        void* _testSystem = _type.Create<
                            const Memory::FreeList&,
                            RuntimeReflection::__data*,
                            RuntimeReflection::__recorder_ftable*,
                            RuntimeReflection::__ftable*
                        >(
                            _pData->m_heap,
                            _pData->m_pRuntimeReflectionData,
                            _pData->m_pRuntimeReflectionRecorder,
                            _pData->m_pRuntimeReflection
                        );

                        // void* _testSystem = RuntimeReflection::create<
                        //     const Memory::FreeList&,
                        //     RuntimeReflection::__data*,
                        //     RuntimeReflection::__recorder_ftable*,
                        //     RuntimeReflection::__ftable*
                        // >(
                        //     _pData->m_objectsHeap,
                        //     _pData->m_pRuntimeReflectionData,
                        //     _typeHandle,
                        //     _pData->m_heap,
                        //     _pData->m_pRuntimeReflectionData,
                        //     _pData->m_pRuntimeReflectionRecorder,
                        //     _pData->m_pRuntimeReflection
                        // );
                        // RuntimeReflection::__duckvil_resource_function_t _castFunctionHandle = RuntimeReflection::get_function_handle<void*>(_pData->m_pRuntimeReflectionData, _typeHandle, "Cast");
                        // ISystem* _systemInheritance = (ISystem*)RuntimeReflection::invoke_static_result<void*, void*>(_pData->m_pRuntimeReflectionData, _typeHandle, _castFunctionHandle, _testSystem);

                        ISystem* _systemInheritance = (ISystem*)_type.InvokeStatic<void*, void*>("Cast", _testSystem);

                        _systemInheritance->m_pLogger = _pData->m_pLogger;
                        _systemInheritance->m_pLoggerData = _pData->m_pLoggerData;

                        RuntimeReflection::__duckvil_resource_type_t _trackKeeperHandle = RuntimeReflection::get_type<HotReloader::TrackKeeper>(_pData->m_pRuntimeReflectionData);
                        HotReloader::TrackKeeper* _trackKeeper = (HotReloader::TrackKeeper*)RuntimeReflection::create(_pData->m_heap, _pData->m_pRuntimeReflectionData, _trackKeeperHandle, _testSystem, _typeHandle);

                        system _system = {};

                        _system.m_type = _typeHandle;
                        _system.m_pTrackKeeper = _trackKeeper;
                        _system.m_fnUpdateCallback = _type.GetFunctionCallback<ISystem>("Update")->m_fnFunction; // RuntimeReflection::get_function_callback<ISystem>(_pData->m_pRuntimeReflectionData, _typeHandle, "Update")->m_fnFunction;
                        _system.m_fnInitCallback = _type.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction; // RuntimeReflection::get_function_callback<bool, ISystem>(_pData->m_pRuntimeReflectionData, _typeHandle, "Init")->m_fnFunction;
                        _system.m_pISystem = _systemInheritance;

                        _pData->m_aEngineSystems.Allocate(_system);

                        RuntimeReflection::__duckvil_resource_type_t _rcTypeHandle = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>(_pData->m_pRuntimeReflectionData);
                        RuntimeReflection::__duckvil_resource_function_t _addHotObjectHandle = RuntimeReflection::get_function_handle<HotReloader::ITrackKeeper*>(_pData->m_pRuntimeReflectionData, _rcTypeHandle, "AddHotObject");
                        RuntimeReflection::invoke_member(_pData->m_pRuntimeReflectionData, _rcTypeHandle, _addHotObjectHandle, _pData->m_pRuntimeCompiler, (HotReloader::ITrackKeeper*)_trackKeeper);

                        _pData->m_eventPool.Add<HotReloader::HotReloadedEvent>(_trackKeeper->GetObject(), _typeHandle);
                        _pData->m_eventPool.Add<HotReloader::HotReloadedEvent>(_trackKeeper);

                        _pData->m_eventPool.AddA<HotReloader::HotReloadedEvent>([_pData](const HotReloader::HotReloadedEvent& _event)
                        {
                            if(_event.m_stage == HotReloader::HotReloadedEvent::stage_after_swap)
                            {
                                RuntimeReflection::__duckvil_resource_function_t _castFunctionHandle = RuntimeReflection::get_function_handle<void*>(_pData->m_pRuntimeReflectionData, _event._typeHandle, "Cast");

                                for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); i++)
                                {
                                    system& _system = _pData->m_aEngineSystems[i];

                                    if(_system.m_type.m_ID == _event._typeHandle.m_ID)
                                    {
                                        _system.m_fnUpdateCallback = RuntimeReflection::get_function_callback<ISystem>(_pData->m_pRuntimeReflectionData, _system.m_type, "Update")->m_fnFunction;
                                        _system.m_pISystem = (ISystem*)RuntimeReflection::invoke_static_result<void*, void*>(_pData->m_pRuntimeReflectionData, _event._typeHandle, _castFunctionHandle, _event.m_pObject);
                                    }
                                }
                            }
                        });

                        if(!(_system.m_pISystem->*_system.m_fnInitCallback)())
                        {
                            return false;
                        }
                    }
                }
            }

            if(!(_pData->m_pRuntimeCompiler->*_pData->m_fnRuntimeCompilerInit)())
            {
                return false;
            }
        }

        RuntimeReflection::ReflectedType _windowType(_pData->m_pRuntimeReflectionData, _pData->m_heap, RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, "WindowSDL", "Duckvil", "Window"));

        _pData->m_pWindow = (Window::IWindow*)_windowType.Create();

        _pData->m_pWindow->Create();

        return true;
    }

    bool start(__data* _pData, __ftable* _pFTable)
    {
        if(_pData->m_bRunning)
        {
            return false;
        }

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
        for(uint32_t i = 0; i < _index; i++)
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

        for(uint32_t i = 0; i < _index; i++)
        {
            printf(" ");
        }

        printf("Size: %d\n", ((Memory::__allocator*)_pDebugInfo->m_pAllocator)->m_ullUsed);

        for(uint32_t i = 0; i < _index; i++)
        {
            printf(" ");
        }

        printf("Capacity: %d\n", ((Memory::__allocator*)_pDebugInfo->m_pAllocator)->m_ullCapacity);

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

        for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); i++)
        {
            system& _system = _pData->m_aEngineSystems[i];

            (_system.m_pISystem->*_system.m_fnUpdateCallback)();
        }

        (_pData->m_pRuntimeCompiler->*_pData->m_fnRuntimeCompilerUpdate)();

        if(_pData->m_ullLastTimeUsed != _pData->m_pHeap->m_ullUsed)
        {
            size_t _change = _pData->m_pHeap->m_ullUsed - _pData->m_ullLastTimeUsed;

            DUCKVIL_LOG_INFO_("Memory change: %d", _change);

            _pData->m_ullLastTimeUsed = _pData->m_pHeap->m_ullUsed;
        }

        if(_pData->m_dOneSecond >= 1.0)
        {
            _pData->m_pLogger->dispatch_logs(_pData->m_pLogger, _pData->m_pLoggerData);

            DUCKVIL_LOG_INFO_("Delta: %f ms", _pData->m_timeData.m_dDelta * 1000.0);
            DUCKVIL_LOG_INFO_("Used memory: %d of %d", _pData->m_pHeap->m_ullUsed, _pData->m_pHeap->m_ullCapacity);

#ifdef DUCKVIL_MEMORY_DEBUGGER
            uint32_t _index = 0;
            recursive(_pData->m_pMemoryDebugger, _index);
#endif

            _pData->m_dOneSecond = 0.0;
        }

#ifdef DUCKVIL_PLATFORM_WINDOWS
        // Sleep(1);
#endif

        _pData->m_pWindow->Refresh();
    }

}

Duckvil::__ftable* duckvil_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::__ftable* _ftable = (Duckvil::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::__ftable), alignof(Duckvil::__ftable));

    _ftable->init = Duckvil::init;
    _ftable->start = Duckvil::start;
    _ftable->stop = Duckvil::stop;
    _ftable->update = Duckvil::update;

    return _ftable;
}