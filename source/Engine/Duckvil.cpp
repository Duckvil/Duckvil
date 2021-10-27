#define STB_IMAGE_IMPLEMENTATION
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
#undef min
#undef GetObject
#undef allocator
#undef GetMessage

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "tracy/Tracy.hpp"

#include "Utils/FunctionArgumentsPusher.h"

#include "TestSystem/TestSystem.h"

#include "Graphics/ModelLoader.h"

namespace Duckvil {

    bool init_runtime_reflection(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

        _pModule->load(&_runtimeReflectionModule);

        duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
        duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", reinterpret_cast<void**>(&_runtimeReflectionInit));
        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", reinterpret_cast<void**>(&_runtimeReflectionRecorderInit));

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
        _pModule->get(_loggerModule, "duckvil_logger_init", reinterpret_cast<void**>(&_duckvilLoggerInit));

        _pData->_loggerFTable = _duckvilLoggerInit();
        _pData->_loggerData = _pData->_loggerFTable.m_fnInitLogger(_pData->m_heap);

        {
            PlugNPlay::__module _module = { };

            PlugNPlay::module_init(&_module);

            PlugNPlay::__module_information _loggerModule("Logger");

            _module.load(&_loggerModule);

            duckvil_logger_channel_init_callback _loggerInit;

            _module.get(_loggerModule, "duckvil_logger_channel_init", reinterpret_cast<void**>(&_loggerInit));

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
                __logger_channel_flags::__logger_flags_console_output | __logger_channel_flags::__logger_flags_file_output | __logger_channel_flags::__logger_flags_editor_console_output
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

        _pModule->get(_threadModule, "duckvil_thread_pool_init", reinterpret_cast<void**>(&init));

        _pData->m_pThread = init();
        _pData->m_pThreadData = _pData->m_pThread->m_fnInit(_pData->m_heap);

        _pData->m_pThread->m_fnStart(_pData->m_pThreadData);

        RuntimeReflection::record_meta(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, RuntimeReflection::get_type<__data>(), "Thread", _pData->m_pThreadData);

        return true;
    }

    bool init_renderer(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _rendererModule("Graphics");

        _pModule->load(&_rendererModule);

        Graphics::Renderer::renderer_ftable* (*init)();

        _pModule->get(_rendererModule, "duckvil_graphics_renderer_init", reinterpret_cast<void**>(&init));

        _pData->m_pRenderer = init();

        _pData->m_pRendererData.m_ecs = &_pData->m_ecs;

        _pData->m_pRenderer->m_fnInit(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pWindow, &_pData->m_pRendererData);

        const auto& _modelLoaderHandle = RuntimeReflection::get_type("ModelLoader", { "Duckvil", "Graphics" });
        void* _modelLoader = RuntimeReflection::create<const char*>(_pData->m_heap, _modelLoaderHandle, false, "F:/Projects/C++/Duckvil/resource/object/test.obj");

        const auto& _modelLoaderGetRawHandle = RuntimeReflection::get_function_handle(_modelLoaderHandle, "GetRaw");
        const Graphics::ModelLoader::Raw& _raw = RuntimeReflection::invoke_member_result<const Graphics::ModelLoader::Raw&>(_modelLoaderHandle, _modelLoaderGetRawHandle, _modelLoader);

        // glm::vec3 _vertices[] =
        // {
        //     glm::vec3(-0.5, -0.5, 0),
        //     glm::vec3(0, 0.5, 0),
        //     glm::vec3(0.5, -0.5, 0)
        // };

        // glm::vec2 _texCoords[] =
        // {
        //     glm::vec2(0, 0),
        //     glm::vec2(0.5, 1),
        //     glm::vec2(1, 0)
        // };

        // uint32_t _indices[] =
        // {
        //     0, 1, 2
        // };

        Graphics::Renderer::vertex_buffer_object_descriptor _desc[] =
        {
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aVertices, _raw.m_aVertices.size(), 3), // size of vertices should be specified here
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aTexCoords, _raw.m_aTexCoords.size(), 2),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ELEMENT_ARRAY_BUFFER, _raw.m_aIndices, _raw.m_aIndices.size(), 1)
        };

        ecs_os_set_api_defaults();

        for(uint32_t i = 0; i < 1; ++i)
        {
            for(uint32_t j = 0; j < 1; ++j)
            {
                _pData->m_ecs.entity().set([_pData, &_desc, i, j, _raw](Graphics::MeshComponent& _mesh, Graphics::TransformComponent& _transform)
                {
                    _mesh.m_uiID = _pData->m_pRenderer->m_fnCreateVAO(
                        _pData->m_pMemory,
                        _pData->m_pHeap,
                        &_pData->m_pRendererData,
                        Graphics::Renderer::vertex_array_object_descriptor
                        {
                            sizeof(_desc) / sizeof(_desc[0]),
                            _desc
                        }
                    );

                    _transform.m_position = glm::vec3(0, j * 2, i * 2);
                    _transform.m_rotation = glm::quat(0, 0, 0, 1);
                    _transform.m_scale = glm::vec3(1, 1, 1);
                });
            }
        }

        _pData->m_rendererQuery = _pData->m_ecs.query<Graphics::TransformComponent>();

        return true;
    }

    bool init_editor(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _editorModule("Editor");

        _pModule->load(&_editorModule);

        Editor::EditorFTable* (*init)();

        _pModule->get(_editorModule, "duckvil_editor_init", reinterpret_cast<void**>(&init));

        _pData->m_pEditor = init();

        _pData->m_pEditorData = static_cast<Editor::ImGuiEditorData*>(_pData->m_pEditor->m_fnInit(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), _pData->m_pWindow, _pData->m_pRenderer, &_pData->m_pRendererData));

        _pData->m_pEditorData->m_pECS = &_pData->m_ecs;

        _pData->m_pEditor->m_fnSetWindowEventPool(_pData->m_pEditorData, &_pData->m_windowEventPool);

        _pData->m_windowEventPool.Add<Window::SetMousePositionEvent>();

        return true;
    }

    bool init_project_manager(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _projectManagerModule("ProjectManager");

        _pModule->load(&_projectManagerModule);

        void (*_initProjectManager)(ProjectManager::ftable*);

        _pModule->get(_projectManagerModule, "duckvil_project_manager_init", reinterpret_cast<void**>(&_initProjectManager));

        _initProjectManager(&_pData->m_projectManager);

        if(!_pData->m_projectManager.m_fnInitProjectManager(&_pData->m_projectManagerData, _pData->m_heap))
        {
            return false;
        }

        return true;
    }

    bool init(__data* _pData, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator)
    {
        TracyMessageL("Initializing engine");

        _pData->m_pHeap = _pAllocator;
        _pData->m_pMemory = _pMemoryInterface;
        // _pData->m_aLoadedModules = nullptr;
        // _pData->m_uiLoadedModulesCount = 0;
        _pData->m_dOneSecond = 0;
        _pData->m_time = time_init();
        _pData->m_heap = Memory::FreeList(_pData->m_pMemory, _pData->m_pHeap);

        _pData->m_heap.Allocate(_pData->m_globalHeap, 1024 * 4);

        Memory::heap_make_current(Memory::free_list_context(_pData->m_globalHeap));

        FrameMarkStart("Initializing memory");

        _pData->m_heap.Allocate(_pData->m_objectsHeap, 1024);
        _pData->m_heap.Allocate(_pData->m_eventsHeap, 1024 * 4);
        _pData->m_heap.Allocate(_pData->m_aEngineSystems, 1);
        _pData->m_heap.Allocate(_pData->m_aRecordedTypes, 1);
        _pData->m_heap.Allocate(_pData->m_aLoadedModules, 1);

        FrameMarkEnd("Initializing memory");

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

        FrameMarkStart("Loading engine modules");

        // _autoLoader.LoadAll(_pMemoryInterface, _pAllocator, &_pData->m_aLoadedModules, &_pData->m_uiLoadedModulesCount);
        _autoLoader.LoadAll(_pMemoryInterface, _pAllocator, &_pData->m_aLoadedModules);

        FrameMarkEnd("Loading engine modules");

        // DUCKVIL_LOG_INFO_("Modules to load %i", _pData->m_uiLoadedModulesCount);

        FrameMarkStart("Initializing reflection");

        for(uint32_t i = 0; i < _pData->m_aLoadedModules.Size(); ++i)
        {
            PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];
            uint32_t (*get_recorder_count)();
            void (*make_current_runtime_reflection_context)(const duckvil_frontend_reflection_context&);
            void (*make_current_heap_context)(const Memory::free_list_context&);

            make_current_runtime_reflection_context = nullptr;
            make_current_heap_context = nullptr;

            _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorder_count", reinterpret_cast<void**>(&get_recorder_count));
            _module.get(_loadedModule, "duckvil_plugin_make_current_runtime_reflection_context", reinterpret_cast<void**>(&make_current_runtime_reflection_context));
            _module.get(_loadedModule, "duckvil_plugin_make_current_heap_context", reinterpret_cast<void**>(&make_current_heap_context));

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

                _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

                if(record == nullptr)
                {
                    // TODO: Should return false?

                    continue;
                }

                duckvil_recorderd_types _types = record(_pMemoryInterface, _pAllocator, _pData->m_pRuntimeReflectionRecorder, _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData);

                _types.m_pModule = &_loadedModule;

                if(_pData->m_aRecordedTypes.Full())
                {
                    _pData->m_aRecordedTypes.Resize(_pData->m_aRecordedTypes.Size() * 2);
                }

                _pData->m_aRecordedTypes.Allocate(_types);
            }

            if(make_current_runtime_reflection_context != nullptr)
            {
                make_current_runtime_reflection_context(RuntimeReflection::get_current());
            }

            if(make_current_heap_context != nullptr)
            {
                make_current_heap_context(Memory::heap_get_current());
            }
        }

        FrameMarkEnd("Initializing reflection");

        init_logger(_pData, &_module);
        init_threading(_pData, &_module);

        RuntimeReflection::record_meta(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, RuntimeReflection::get_type<__data>(), "Time", &(_pData->m_timeData));

        for(uint32_t i = 0; i < _pData->m_aLoadedModules.Size(); ++i)
        {
            const PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];

            void (*make_current_logger_context)(const logger_context&);

            _module.get(_loadedModule, "duckvil_plugin_make_current_logger_context", reinterpret_cast<void**>(&make_current_logger_context));

            if(make_current_logger_context != nullptr)
            {
                make_current_logger_context(logger_get_current());
            }
        }

        _pData->m_bRunning = false;

        RuntimeReflection::ReflectedType<> _windowType(_pData->m_heap, RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, "WindowSDL", "Duckvil", "Window"));

        _pData->m_pWindow = (Window::IWindow*)_windowType.Create<Event::Pool<Event::mode::buffered>*>(&_pData->m_windowEventPool);

        _pData->m_pWindow->Create("Duckvil", 1920, 1080);

        init_renderer(_pData, &_module);
        init_editor(_pData, &_module);
        init_project_manager(_pData, &_module);

        _pData->m_eventPool.AddA<RequestSystemEvent>([_pData](RequestSystemEvent& _event)
        {
            if(_event.m_typeHandle.m_ID == RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>().m_ID)
            {
                _event.m_pRequestedSystem = _pData->m_pRuntimeCompiler;
            }
        });

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

                (static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_pRuntimeReflectionData->m_pEvents))->Add<RuntimeReflection::TrackedObjectCreatedEvent>(_pData->m_pRuntimeCompiler);

                _pData->m_fnRuntimeCompilerUpdate = _type.GetFunctionCallback<ISystem, double>("Update")->m_fnFunction;
                _pData->m_fnRuntimeCompilerInit = _type.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction;

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
                _type.Invoke<Memory::Vector<PlugNPlay::__module_information>*>("SetModules", _pData->m_pRuntimeCompiler, &_pData->m_aLoadedModules);
                _type.Invoke<Memory::Vector<duckvil_recorderd_types>*>("SetReflectedTypes", _pData->m_pRuntimeCompiler, &_pData->m_aRecordedTypes);
            }

            for(uint32_t i = 0; i < _types.Size(); ++i)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _typeHandle = _types[i];
                const RuntimeReflection::__variant& _variant = RuntimeReflection::get_meta(_typeHandle, ReflectionFlags::ReflectionFlags_UserSystem);

                if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() &&
                    static_cast<uint8_t>(_variant.m_traits) & static_cast<uint8_t>(RuntimeReflection::property_traits::is_bool))
                {
                    if(_pData->m_aEngineSystems.Full())
                    {
                        _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                    }

                    RuntimeReflection::ReflectedType<> _type(_pData->m_heap, _typeHandle);

                    auto _constructors = RuntimeReflection::get_constructors(_pData->m_heap, _typeHandle);
                    void* _testSystem = nullptr;

                    for(const auto& _constructorHandle : _constructors)
                    {
                        const auto& _constructor = RuntimeReflection::get_constructor(_typeHandle, _constructorHandle);
                        uint32_t _constructorArgumentsCount = DUCKVIL_SLOT_ARRAY_SIZE(_constructor.m_arguments);

                        if(_constructorArgumentsCount > 0)
                        {
                            FunctionArgumentsPusher c(5 + _constructorArgumentsCount);

                            c.Push(_pData->m_pMemory);
                            c.Push(_pData->m_pHeap);
                            c.Push(_pData->m_pRuntimeReflection);
                            c.Push(_pData->m_pRuntimeReflectionData);
                            c.Push(true);

                            for(uint32_t i = 0; i < _constructorArgumentsCount; ++i)
                            {
                                const RuntimeReflection::__argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, i);

                                if(typeid(Memory::FreeList).hash_code() == _argument.m_ullTypeID)
                                {
                                    c.Push(_pData->m_heap);
                                }

                                if(typeid(HotReloader::RuntimeCompilerSystem*).hash_code() == _argument.m_ullTypeID)
                                {
                                    c.Push(_pData->m_pRuntimeCompiler);
                                }
                            }

                            const auto& _h = RuntimeReflection::get_constructor_handle<const Memory::FreeList&>(_type.GetTypeHandle());
                            const auto& _hc = RuntimeReflection::get_constructor(_type.GetTypeHandle(), _h);

                            c.Call(_hc.m_pData);

                            _testSystem = c.getCode<void*(*)()>()();
                        }
                    }

                    // void* _testSystem = _type.CreateTracked<
                    //     const Memory::FreeList&
                    // >(
                    //     _pData->m_heap
                    // );

                    system _system = {};

                    _system.m_type = _typeHandle;
                    _system.m_pTrackKeeper = DUCKVIL_TRACK_KEEPER_CAST(ISystem, _testSystem);
                    _system.m_fnUpdateCallback = _type.GetFunctionCallback<ISystem, double>("Update")->m_fnFunction;
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

                                _system.m_fnUpdateCallback = _systemType.GetFunctionCallback<ISystem, double>("Update")->m_fnFunction;
                                _system.m_fnInitCallback = _systemType.GetFunctionCallback<bool, ISystem>("Init")->m_fnFunction;
                            }
                        }
                    });

                    if(!(static_cast<ISystem*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_system.m_pTrackKeeper))->*_system.m_fnInitCallback)())
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

        TracyMessageL("Initializing engine finished");

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
        ZoneScopedN("Update");

        double _delta = _pData->m_timeData.m_dDelta;

        _pData->m_dOneSecond += _delta;

        for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); ++i)
        {
            system& _system = _pData->m_aEngineSystems[i];

            (static_cast<ISystem*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_system.m_pTrackKeeper))->*_system.m_fnUpdateCallback)(_delta);
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
            (_pData->m_pRuntimeCompiler->*_pData->m_fnRuntimeCompilerUpdate)(_delta);
#endif

            // _pData->m_pLogger->dispatch_logs(/*_pData->m_pLogger, _pData->m_pLoggerData*/ logger_get_current().m_pLogger, logger_get_current().m_pLoggerData);
            // _pData->m_logger->Dispatch();
            _pData->m_pLoggerChannel->dispatch_logs(_pData->m_pLoggerChannel, _pData->m_pLoggerChannelData);

            // DUCKVIL_LOG_INFO("Delta: %f ms", _delta * 1000.0);
            // DUCKVIL_LOG_INFO("Used memory: %d of %d", _pData->m_pHeap->m_ullUsed, _pData->m_pHeap->m_ullCapacity);

// #ifdef DUCKVIL_MEMORY_DEBUGGER
//             uint32_t _index = 0;
//             recursive(_pData->m_pMemoryDebugger, _index);
// #endif

            _pData->m_dOneSecond = 0.0;
        }

        _pData->m_rendererQuery.each([_delta](Graphics::TransformComponent& _transform)
        {
            _transform.m_rotation = _transform.m_rotation * glm::angleAxis((float)_delta, glm::vec3(0, 0, 1));
        });

        _pData->m_pRenderer->m_fnUpdate(_pData->m_pMemory, &_pData->m_pRendererData);

        _pData->m_pRenderer->m_fnBindAsRenderTarget();

        _pData->m_pEditor->m_fnRender(_pData->m_pEditorData, _pData->m_pWindow);

        _pData->m_windowEventPool.Reset();

        while(_pData->m_windowEventPool.AnyEvents())
        {
            Window::CloseEvent _closeEvent;
            Window::ResizeEvent _resizeEvent;
            Window::SetMousePositionEvent _setMousePositionEvent;

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
            else if(_pData->m_windowEventPool.GetMessage(&_setMousePositionEvent))
            {
                _pData->m_pWindow->SetMousePosition(_setMousePositionEvent.m_iX, _setMousePositionEvent.m_iY);

                _pData->m_windowEventPool.EventHandled<Window::SetMousePositionEvent>();
            }
        }

        _pData->m_pWindow->Refresh();
        _pData->m_time.update(&_pData->m_timeData);
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