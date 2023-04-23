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

#include "Engine/Events/InjectConstructorArgumentEvent.h"

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

#include "Utils/RuntimeDependencyInjector.h"

// #include "TestSystem/TestSystem.h"

#include "Graphics/ModelLoader.h"

#include "Network/NetworkSystem.h"

#include "Engine/UUIDComponent.h"

#include "DependencyInjection/DependencyInjection.h"

#include "Editor/ReflectionFlag.h"

#include "Engine/runtime_reflection.h"

namespace Duckvil {

    bool init_logger(__data* _pData, PlugNPlay::__module* _pModule)
    {
        logger_ftable(*_duckvilLoggerInit)();

        PlugNPlay::__module_information _loggerModule("Logger");

        _pModule->load(&_loggerModule);
        _pModule->get(_loggerModule, "duckvil_logger_init", reinterpret_cast<void**>(&_duckvilLoggerInit));

        _pData->_loggerFTable = _duckvilLoggerInit();
        _pData->_loggerData = _pData->_loggerFTable.m_fnInitLogger(_pData->m_heap);

        {
            PlugNPlay::__module_information _loggerModule("Logger");

            _pModule->load(&_loggerModule);

            duckvil_logger_channel_init_callback _loggerInit;

            _pModule->get(_loggerModule, "duckvil_logger_channel_init", reinterpret_cast<void**>(&_loggerInit));

            _pData->m_pLoggerChannel = _loggerInit();

            logger_make_current(logger_context
    		{
	            .m_logger = _pData->_loggerFTable,
	            .m_loggerData = _pData->_loggerData,
	            .m_pLoggerChannel = _pData->m_pLoggerChannel
    		});
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

        for(uint32_t i = 0; i < _pData->m_aLoadedModules.Size(); ++i)
        {
            const PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];

            void (*make_current_logger_context)(const logger_context&);

            _pModule->get(_loadedModule, "duckvil_plugin_make_current_logger_context", reinterpret_cast<void**>(&make_current_logger_context));

            if(make_current_logger_context == nullptr)
            {
                continue;
            }

                make_current_logger_context(logger_get_current());
            }

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

        duckvil_runtime_reflection_recorder_stuff _stuff =
        {
            ._pMemoryInterface = _pData->m_heap.GetMemoryInterface(),
            ._pAllocator = _pData->m_heap.GetAllocator(),
            ._pFunctions = RuntimeReflection::get_current().m_pRecorder,
            ._pData = RuntimeReflection::get_current().m_pReflectionData
        };

        RuntimeReflection::record_meta(_stuff, RuntimeReflection::get_type<__data>(), "Thread", _pData->m_pThreadData);

        return true;
    }

#ifndef DUCKVIL_HEADLESS_SERVER
    bool init_renderer(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _rendererModule("Graphics");

        _pModule->load(&_rendererModule);

        Graphics::Renderer::renderer_ftable* (*init)();

        _pModule->get(_rendererModule, "duckvil_graphics_renderer_init", reinterpret_cast<void**>(&init));

        _pData->m_pRenderer = init();

        _pData->m_pRendererData.m_ecs = &_pData->m_ecs;

        _pData->m_pRenderer->m_fnInit(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pWindow, &_pData->m_pRendererData);

        return true;
    }

    bool init_editor(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _editorModule("Editor");

        _pModule->load(&_editorModule);

        Editor::EditorFTable* (*init)();

        _pModule->get(_editorModule, "duckvil_editor_init", reinterpret_cast<void**>(&init));

        _pData->m_pEditor = init();

        _pData->m_pEditorData = static_cast<Editor::ImGuiEditorData*>(_pData->m_pEditor->m_fnInit(_pData->m_pEditor, _pData->m_heap, _pData->m_pWindow, _pData->m_pRenderer, &_pData->m_pRendererData, static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_pRuntimeReflectionData->m_pEvents), &_pData->m_eventPool));

        _pData->m_pEditorData->m_pECS = &_pData->m_ecs;

        _pData->m_pEditor->m_fnSetWindowEventPool(_pData->m_pEditorData, &_pData->m_windowEventPool);

        return true;
    }
#endif

    bool init_project_manager(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _projectManagerModule("ProjectManager");

        _pModule->load(&_projectManagerModule);

        void (*_initProjectManager)(ProjectManager::ftable*);

        _pModule->get(_projectManagerModule, "duckvil_project_manager_init", reinterpret_cast<void**>(&_initProjectManager));

        _initProjectManager(&_pData->m_projectManager);

        if(!_pData->m_projectManager.m_fnInitProjectManager(&_pData->m_projectManagerData, _pData->m_heap, &_pData->m_eventPool))
        {
            return false;
        }

        return true;
    }

#ifndef DUCKVIL_HEADLESS_SERVER
    void create_window(__data* _pData)
    {
        RuntimeReflection::ReflectedType _windowType(RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, "WindowSDL", "Duckvil", "Window"));

        _pData->m_pWindow = static_cast<Window::IWindow*>(_windowType.Create<Event::Pool<Event::mode::buffered>*>(_pData->m_heap, &_pData->m_windowEventPool));

        _pData->m_pWindow->Create("Duckvil", 2560, 1440);
    }
#endif

    void init_memory(__data* _pData)
    {
        _pData->m_heap.Allocate(_pData->m_objectsHeap, 1024 * 4);
        _pData->m_heap.Allocate(_pData->m_eventsHeap, 1024 * 16);
        _pData->m_heap.Allocate(_pData->m_aEngineSystems, 1);
        _pData->m_heap.Allocate(_pData->m_aLoadedModules, 1);

        _pData->m_aRecordedTypes = Memory::ThreadsafeVector<duckvil_recorderd_types>(_pData->m_heap);
    }

    bool init(__data* _pData, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator)
    {
        TracyMessageL("Initializing engine");

        _pData->m_bRunning = false;
        _pData->m_pHeap = _pAllocator;
        _pData->m_pMemory = _pMemoryInterface;
        // _pData->m_aLoadedModules = nullptr;
        // _pData->m_uiLoadedModulesCount = 0;
        _pData->m_dOneSecond = 0;
        _pData->m_time = time_init();
        _pData->m_heap = Memory::FreeList(_pData->m_pMemory, _pData->m_pHeap);

        _pData->m_heap.Allocate(_pData->m_globalHeap, 1024 * 16);

        Memory::heap_make_current(Memory::free_list_context(_pData->m_globalHeap));

        TracyMessageL("Initializing memory");
        init_memory(_pData);

        DUCKVIL_DEBUG_MEMORY(_pData->m_objectsHeap.GetAllocator(), "m_objectsHeap");
        DUCKVIL_DEBUG_MEMORY(_pData->m_eventsHeap.GetAllocator(), "m_eventsHeap");
        DUCKVIL_DEBUG_MEMORY(_pData->m_aEngineSystems.GetAllocator(), "m_aEngineSystems");
        // DUCKVIL_DEBUG_MEMORY(_pData->m_aRecordedTypes.GetAllocator(), "m_aRecordedTypes");

        _pData->m_time.init(&_pData->m_timeData);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        init_runtime_reflection_module(_pData, &_module);

        _pData->m_eventPool = Event::Pool<Event::mode::immediate>(_pData->m_eventsHeap, _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData);
        _pData->m_windowEventPool = Event::Pool<Event::mode::buffered>(_pData->m_eventsHeap, _pData->m_pRuntimeReflectionData);
        _pData->m_bufferedEventPool = Event::Pool<Event::mode::buffered>(_pData->m_eventsHeap, _pData->m_pRuntimeReflectionData);

        _pData->m_entityFactory.Init();

        PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

        TracyMessageL("Loading engine modules");
        _autoLoader.LoadAll(_pMemoryInterface, _pAllocator, &_pData->m_aLoadedModules);

        // DUCKVIL_LOG_INFO_("Modules to load %i", _pData->m_uiLoadedModulesCount);

        TracyMessageL("Initializing reflection");
        init_runtime_reflection(_pData, _module);

        init_logger(_pData, &_module);
        init_threading(_pData, &_module);
#ifndef DUCKVIL_HEADLESS_SERVER
        create_window(_pData);
        init_renderer(_pData, &_module);
        init_editor(_pData, &_module);
#endif
        init_project_manager(_pData, &_module);

        const auto& _CSharpTypeHandle = RuntimeReflection::get_type("CSharp", { "Duckvil", "CSharp" });

        _pData->m_pCSharpLanguage =
            static_cast<ScriptEngine::Language*>(
				RuntimeReflection::create<
					const Memory::FreeList&,
					EntityFactory*,
					Event::Pool<Event::mode::immediate>*
                >(_pData->m_heap, _CSharpTypeHandle, false, _pData->m_heap, &_pData->m_entityFactory, &_pData->m_eventPool)
			);

        _pData->m_pCSharpLanguage->Setup();
        _pData->m_pCSharpLanguage->Init();

#ifndef DUCKVIL_HEADLESS_SERVER
        if(_pData->m_bIsServer)
        {
            const auto& _serverTypeHandle = RuntimeReflection::get_type("Server", { "Duckvil", "Network" });

            _pData->m_pServer = static_cast<Network::IServer*>(RuntimeReflection::create<const Memory::FreeList&, uint16_t>(_pData->m_heap, _serverTypeHandle, false, _pData->m_heap, _pData->m_usPort));

            _pData->m_pServer->Start();
        }
        else if(_pData->m_bIsClient)
        {
            const auto& _clientTypeHandle = RuntimeReflection::get_type("Client", { "Duckvil", "Network" });

            _pData->m_pClient = static_cast<Network::IClient*>(RuntimeReflection::create<const Memory::FreeList&>(_pData->m_heap, _clientTypeHandle, false, _pData->m_heap));

            _pData->m_pClient->Connect(_pData->m_sIP, _pData->m_usPort);
        }
#else
        {
            const auto& _serverTypeHandle = RuntimeReflection::get_type("Server", { "Duckvil", "Network" });

            _pData->m_pServer = static_cast<Network::IServer*>(RuntimeReflection::create<const Memory::FreeList&, uint16_t>(_pData->m_heap, _serverTypeHandle, false, _pData->m_heap, _pData->m_usPort));
            _pData->m_bIsServer = true;

            _pData->m_pServer->Start();
        }
#endif

        _pData->m_eventPool.AddA<RequestSystemEvent>([_pData](RequestSystemEvent& _event)
        {
            if(_event.m_typeHandle.m_ID == RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>().m_ID)
            {
                _event.m_pRequestedSystem = _pData->m_pRuntimeCompiler;
            }

            if(_event.m_typeHandle.m_ID == RuntimeReflection::get_type<ProjectManager::ftable>().m_ID)
            {
                _event.m_pRequestedSystem = &_pData->m_projectManager;
            }

            if(_event.m_typeHandle.m_ID == RuntimeReflection::get_type<ProjectManager::data>().m_ID)
            {
                _event.m_pRequestedSystem = &_pData->m_projectManagerData;
            }

            if(_event.m_typeHandle.m_ID == RuntimeReflection::get_type<EntityFactory>().m_ID)
            {
                _event.m_pRequestedSystem = &_pData->m_entityFactory;
            }
        });

        (static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_pRuntimeReflectionData->m_pEvents))->AddA<RuntimeReflection::TrackedObjectCreatedEvent>([_pData](const RuntimeReflection::TrackedObjectCreatedEvent& _event)
        {
            RuntimeReflection::ReflectedType _type(_event.m_pTrackKeeper->GetTypeHandle());

            if(!_type.Inherits<ISystem>() || _type.GetTypeID() == typeid(HotReloader::RuntimeCompilerSystem).hash_code())
            {
                return;
            }

            if(_pData->m_pClient)
            {
                if(_type.Inherits<Network::NetworkSystem>())
                {
                    _pData->m_pClient->AddSystem(static_cast<Network::NetworkSystem*>(_event.m_pTrackKeeper->GetObject()));
                }
            }
            else if(_pData->m_pServer)
            {
                if(_type.Inherits<Network::NetworkSystem>())
                {
                    _pData->m_pServer->AddSystem(static_cast<Network::NetworkSystem*>(_event.m_pTrackKeeper->GetObject()));
                }
            }

            system _system = {};

            _system.m_type = _event.m_pTrackKeeper->GetTypeHandle();
            _system.m_pObject = _event.m_pTrackKeeper;
            _system.m_bIsHot = true;

            const auto& _update = _type.GetFunctionCallbackM<ISystem, double>("Update");
            const auto& _init = _type.GetFunctionCallbackMR<bool, ISystem>("Init");

            if(_update && _init)
            {
                _system.m_fnUpdateCallback = _update->m_fnFunction;
                _system.m_fnInitCallback = _init->m_fnFunction;

                if(_pData->m_aEngineSystems.Full())
                {
                    _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                }

                _pData->m_aEngineSystems.Allocate(_system);
            }

            const auto& _meta = _type.GetMeta(ReflectionFlags_AutoEventsAdding);

            if(_meta.m_ullTypeID != -1 && _meta.m_pData != nullptr)
            {
                const auto& _functions = _type.GetFunctions(_pData->m_heap);

                for(const auto& _functionHandle : _functions)
                {
                    if(_functionHandle.GetArgumentsTypeID() == DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(const HotReloader::SwapEvent&) && strcmp(_functionHandle.GetName(), "OnEvent") == 0)
                    {
                        _pData->m_eventPool.Add<HotReloader::SwapEvent>(_event.m_pTrackKeeper, _type.GetHandle());
                    }
                }
            }
        });

        (static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_pRuntimeReflectionData->m_pEvents))->AddA<RuntimeReflection::ObjectCreatedEvent>([_pData](const RuntimeReflection::ObjectCreatedEvent& _event)
        {
            DUCKVIL_RESOURCE(type_t) _typeHandle = RuntimeReflection::get_type(_event.m_ullTypeID);
            RuntimeReflection::ReflectedType _type(_typeHandle);

            if(!_type.Inherits<ISystem>() || _type.GetTypeID() == typeid(HotReloader::RuntimeCompilerSystem).hash_code())
            {
                return;
            }

            if(_pData->m_pClient)
            {
                if(_type.Inherits<Network::NetworkSystem>())
                {
                    _pData->m_pClient->AddSystem(static_cast<Network::NetworkSystem*>(_event.m_pObject));
                }
            }
            else if(_pData->m_pServer)
            {
                if(_type.Inherits<Network::NetworkSystem>())
                {
                    _pData->m_pServer->AddSystem(static_cast<Network::NetworkSystem*>(_event.m_pObject));
                }
            }

            system _system = {};

            _system.m_type = _typeHandle;
            _system.m_pObject = _event.m_pObject;
            _system.m_bIsHot = false;

            const auto& _update = _type.GetFunctionCallbackM<ISystem, double>("Update");
            const auto& _init = _type.GetFunctionCallbackMR<bool, ISystem>("Init");

            if(_update && _init)
            {
                _system.m_fnUpdateCallback = _update->m_fnFunction;
                _system.m_fnInitCallback = _init->m_fnFunction;

                if(_pData->m_aEngineSystems.Full())
                {
                    _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                }

                _pData->m_aEngineSystems.Allocate(_system);
            }

            const auto& _meta = _type.GetMeta(ReflectionFlags_AutoEventsAdding);

            if(_meta.m_ullTypeID != -1 && _meta.m_pData != nullptr)
            {
                const auto& _functions = _type.GetFunctions(_pData->m_heap);

                for(const auto& _functionHandle : _functions)
                {
                    if(_functionHandle.GetArgumentsTypeID() == DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(const HotReloader::SwapEvent&) && strcmp(_functionHandle.GetName(), "OnEvent") == 0)
                    {
                        _pData->m_eventPool.Add<HotReloader::SwapEvent>(_event.m_pObject, _type.GetHandle());
                    }
                }
            }
        });

        {
            auto _types = RuntimeReflection::get_types(_pData->m_heap);
        	auto _runtimeCompilerType = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>();

            // _pData->m_pDependencyInjector = _pData->m_heap.Allocate<DependencyInjection::

            const auto& _diTypeHandle = RuntimeReflection::get_type<DependencyInjection::DependencyResolver>();
            RuntimeReflection::ReflectedType _diType(_diTypeHandle);

            _pData->m_pDependencyInjector = static_cast<DependencyInjection::IDependencyResolver*>(_diType.Create<const Memory::FreeList&, Event::Pool<Event::mode::immediate>*>(_pData->m_heap, _pData->m_heap, &_pData->m_eventPool));

            {
                RuntimeReflection::ReflectedType _type(RuntimeReflection::ReflectedType::Tag<HotReloader::RuntimeCompilerSystem>{});

                HotReloader::RuntimeCompilerSystem::user_data* _actionData =
                    _pData->m_heap.Allocate<HotReloader::RuntimeCompilerSystem::user_data>();

                _pData->m_pRuntimeCompiler = (HotReloader::RuntimeCompilerSystem*)_type.Create<
                    const Memory::FreeList&,
                    Event::Pool<Event::mode::immediate>*,
                    Event::Pool<Event::mode::immediate>*,
                    HotReloader::FileWatcher::ActionCallback,
                    void*,
                    DependencyInjection::IDependencyResolver*
                >(
                    (const Memory::FreeList&)_pData->m_heap,
                    (const Memory::FreeList&)_pData->m_heap,
                    &_pData->m_eventPool,
                    static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_pRuntimeReflectionData->m_pEvents),
                    HotReloader::RuntimeCompilerSystem::Action,
                    static_cast<void*>(_actionData),
                    _pData->m_pDependencyInjector
                );

                _actionData->m_pRuntimeCompiler = _pData->m_pRuntimeCompiler;

                const auto& _setCWDHandle = RuntimeReflection::get_function_handle<const std::filesystem::path&>({ _runtimeCompilerType.m_ID }, "SetCWD");

                RuntimeReflection::invoke_member<const std::filesystem::path&>({ _runtimeCompilerType.m_ID }, _setCWDHandle, _pData->m_pRuntimeCompiler, DUCKVIL_CWD);

                (static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_pRuntimeReflectionData->m_pEvents))->Add<RuntimeReflection::TrackedObjectCreatedEvent>(_pData->m_pRuntimeCompiler);

                _pData->m_fnRuntimeCompilerUpdate = _type.GetFunctionCallbackM<ISystem, double>("Update")->m_fnFunction;
                _pData->m_fnRuntimeCompilerInit = _type.GetFunctionCallbackMR<bool, ISystem, bool>("Init")->m_fnFunction;

                _type.InvokeM<const Memory::FreeList&>("SetObjectsHeap", _pData->m_pRuntimeCompiler, _pData->m_objectsHeap);
                _type.InvokeM<Memory::Vector<PlugNPlay::__module_information>*>("SetModules", _pData->m_pRuntimeCompiler, &_pData->m_aLoadedModules);
                _type.InvokeM<Memory::ThreadsafeVector<duckvil_recorderd_types>*>("SetReflectedTypes", _pData->m_pRuntimeCompiler, &_pData->m_aRecordedTypes);
            }

            _pData->m_pDependencyInjector->Register(_pData->m_pRuntimeCompiler);
            _pData->m_pDependencyInjector->Register<Memory::FreeList>(Utils::lambda([&](const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle, const RuntimeReflection::__duckvil_resource_argument_t& _argumentHandle) -> void*
                {
                    if(RuntimeReflection::meta_has_value(_typeHandle, _constructorHandle, _argumentHandle, "Original"))
                    {
                        return &_pData->m_heap;
                    }

                    auto _m = _pData->m_heap.Allocate<Memory::FreeList>();
                    const auto _sizeMeta = RuntimeReflection::get_meta(_typeHandle, _constructorHandle, _argumentHandle.m_ID, "Size");
                    uint32_t _size = 1024 * 1024;

					if(_sizeMeta.m_pData != nullptr)
					{
                        _size = *static_cast<uint32_t*>(_sizeMeta.m_pData);
					}

					_pData->m_heap.Allocate(*_m, _size);

                    return _m;
                }), DependencyInjection::Scope::TRANSIENT);
            _pData->m_pDependencyInjector->Register(_pData->m_projectManager);
            _pData->m_pDependencyInjector->Register(_pData->m_projectManagerData);
            _pData->m_pDependencyInjector->Register(_pData->m_pClient);
            _pData->m_pDependencyInjector->Register(_pData->m_pServer);
            _pData->m_pDependencyInjector->Register(&_pData->m_ecs);
            _pData->m_pDependencyInjector->Register(&_pData->m_entityFactory);
            _pData->m_pDependencyInjector->Register(&_pData->m_eventPool, Utils::lambda([&](const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle, const RuntimeReflection::__duckvil_resource_argument_t& _argumentHandle)
                {
			        static const size_t _immediateEventPoolTypeID = typeid(Event::Pool<Event::mode::immediate>).hash_code();

                    const RuntimeReflection::__argument_t& _arg = RuntimeReflection::get_argument(_typeHandle, _constructorHandle, _argumentHandle);

					if(RuntimeReflection::meta_has_value(_typeHandle, _constructorHandle, _argumentHandle, "Engine") && _immediateEventPoolTypeID == _arg.m_ullTypeID)
					{
                        return true;
					}

            		return false;
				}));
            _pData->m_pDependencyInjector->Register(&_pData->m_windowEventPool, Utils::lambda([&](const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle, const RuntimeReflection::__duckvil_resource_argument_t& _argumentHandle)
                {
                    static const size_t _bufferedEventPoolTypeID = typeid(Event::Pool<Event::mode::buffered>).hash_code();

                    const RuntimeReflection::__argument_t& _arg = RuntimeReflection::get_argument(_typeHandle, _constructorHandle, _argumentHandle);

		            if(RuntimeReflection::meta_has_value(_typeHandle, _constructorHandle, _argumentHandle, "Window") && _bufferedEventPoolTypeID == _arg.m_ullTypeID)
		            {
		                return true;
		            }

                    return false;
				}));

            for(uint32_t i = 0; i < _types.Size(); ++i)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _typeHandle = _types[i];

                if(_runtimeCompilerType.m_ID == _typeHandle.m_ID)
                {
                    continue;
                }

                const RuntimeReflection::ReflectedType _type(_typeHandle);
                auto _autoInstantiateMetaFlag = _type.GetMeta(ReflectionFlags::ReflectionFlags_AutoInstantiate);
                auto _spawnableVariant = _type.GetMeta(Editor::ReflectionFlag::Spwanable);

                if((!_type.Inherits<Editor::Widget>() && !_type.Inherits<ISystem>()) ||
                    (_autoInstantiateMetaFlag.m_pData != nullptr && !*static_cast<bool*>(_autoInstantiateMetaFlag.m_pData)) ||
                    _spawnableVariant.m_ullTypeID != -1)
                {
                    continue;
                }

                if(_pData->m_aEngineSystems.Full())
                {
                    _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                }

                auto _constructors = RuntimeReflection::get_constructors(_pData->m_heap, _typeHandle);

            // TODO: Problem in release
                for(/*const auto& _constructorHandle : _constructors*/ uint32_t i = 0; i < _constructors.Size(); ++i)
                {
                    if(_pData->m_pDependencyInjector->Resolve(_typeHandle, _constructors[i]))
                    {
                        // Called the constructor

                        break;
                    }
                }
            }

            _pData->m_eventPool.AddA<HotReloader::SwapEvent>([_pData](const HotReloader::SwapEvent& _event)
            {
                for(uint32_t j = 0; j < _pData->m_aEngineSystems.Size(); ++j)
                {
                    system& _system = _pData->m_aEngineSystems[j];

                    if(_system.m_type.m_ID == _event.m_pTrackKeeper->GetTypeHandle().m_ID)
                    {
                        RuntimeReflection::ReflectedType _systemType(_system.m_type);

                        auto _updateCallback = _systemType.GetFunctionCallbackM<ISystem, double>("Update")->m_fnFunction;
                        auto _initCallback = _systemType.GetFunctionCallbackMR<bool, ISystem>("Init")->m_fnFunction;
                        void* _newObject = _event.m_pTrackKeeper->GetObject();

                        if(!(static_cast<ISystem*>(_newObject)->*_initCallback)())
                        {
                            continue;
                        }

                        _system.m_fnUpdateCallback = _updateCallback;
                        _system.m_fnInitCallback = _initCallback;
                    }
                }
            });

#ifndef DUCKVIL_HEADLESS_SERVER
            _pData->m_pEditor->m_fnPostInit(_pData->m_pEditorData, _pData->m_pEditor, &_pData->m_eventPool, _pData->m_pRuntimeCompiler);
#endif

            if(!(_pData->m_pRuntimeCompiler->*_pData->m_fnRuntimeCompilerInit)(false))
            {
                return false;
            }
        }

        _pData->m_eventPool.AddA<ProjectManager::LoadProjectEvent>([_pData](ProjectManager::LoadProjectEvent& _event)
        {
        	_pData->m_projectManagerData.m_pDependencyResolver = _pData->m_pDependencyInjector;

            _pData->m_projectManager.m_fnLoadProject(&_pData->m_projectManagerData, _event.m_sProjectName, Utils::string(DUCKVIL_PROJECTS_PATH) / _event.m_sProjectName / "bin");
        });

        for(const system& _system : _pData->m_aEngineSystems)
        {
            if(_system.m_bIsHot)
            {
	            if(!(static_cast<ISystem*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_system.m_pObject))->*_system.m_fnInitCallback)())
	            {
	                return false;
	            }
            }
            else
            {
	            if(!(static_cast<ISystem*>(_system.m_pObject)->*_system.m_fnInitCallback)())
	            {
	                return false;
	            }
            }
        }

        TracyMessageL("Initializing engine finished");

        {
            const auto& _modelLoaderHandle = RuntimeReflection::get_type("ModelLoader", { "Duckvil", "Graphics" });
        
            std::filesystem::path _cwd = DUCKVIL_CWD;

            void* _modelLoader = RuntimeReflection::create<const char*>(_pData->m_heap, _modelLoaderHandle, false, (_cwd / "resource/object/test.obj").string().c_str());

            const auto& _modelLoaderGetRawHandle = RuntimeReflection::get_function_handle(_modelLoaderHandle, "GetRaw");
            const Graphics::ModelLoader::Raw& _raw = RuntimeReflection::invoke_member_result<const Graphics::ModelLoader::Raw&>(_modelLoaderHandle, _modelLoaderGetRawHandle, _modelLoader);

            ecs_os_set_api_defaults();

            _pData->m_entityFactory.GetEventPool()->Add(
                Utils::lambda(
                    [_pData, _raw](EntityCreatedEvent& _e)
                    {
                        std::vector<int> _id(_raw.m_aVertices.size(), _e.m_entity.ID());

                        Graphics::Renderer::vertex_buffer_object_descriptor _desc[] =
                        {
                            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aVertices, 4), // size of vertices should be specified here
                            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aTexCoords, 2),
                            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _id, 1),
                            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ELEMENT_ARRAY_BUFFER, _raw.m_aIndices, 1)
                        };

                        _desc[0].m_type = GL_FLOAT;
                        _desc[1].m_type = GL_FLOAT;
                        _desc[2].m_type = GL_UNSIGNED_INT;
                        _desc[3].m_type = GL_FLOAT;

                        _e.m_entity.Add(
                            Graphics::MeshComponent
                            {
                                _pData->m_pRenderer->m_fnCreateVAO(
                                    _pData->m_pMemory,
                                    _pData->m_pHeap,
                                    &_pData->m_pRendererData,
                                    Graphics::Renderer::vertex_array_object_descriptor
                                    {
                                        sizeof(_desc) / sizeof(_desc[0]),
                                        _desc
                                    }
                                )
                            }
                        );
                    }
                )
            );

            _pData->m_rendererQuery = _pData->m_ecs.query<Graphics::TransformComponent>();
            _pData->m_scriptsQuery = _pData->m_ecs.query<ScriptComponent>();
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
#ifndef DUCKVIL_HEADLESS_SERVER
        _pData->m_pEditorData->m_pEditorEvents.Broadcast(Editor::HexEditorWidgetInitEvent{ _pData->m_pMemoryDebugger });
#endif
#endif
        _pData->m_bRunning = true;

        _pData->m_bufferedEventPool.Reset();

        while(_pData->m_bufferedEventPool.AnyEvents())
        {
            static ProjectManager::LoadProjectEvent _loadProjectEvent;

            if(_pData->m_bufferedEventPool.GetMessage(&_loadProjectEvent))
            {
                _pData->m_eventPool.Broadcast(_loadProjectEvent);

                _pData->m_bufferedEventPool.EventHandled<ProjectManager::LoadProjectEvent>();
            }
            else
            {
                _pData->m_bufferedEventPool.Skip();
            }
        }

        while(_pData->m_bRunning)
        {
            _pFTable->update(_pData, _pFTable);
        }

        if(_pData->m_bIsServer)
        {
            _pData->m_pServer->Stop();

            _pData->m_heap.Free(_pData->m_pServer);
        }
        else if(_pData->m_bIsClient)
        {
            _pData->m_pClient->Disconnect();

            _pData->m_heap.Free(_pData->m_pClient);
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
        FrameMark;

#ifndef DUCKVIL_HEADLESS_SERVER
        _pData->m_pWindow->PopulateEvents();
#endif

        ZoneScopedN("Update");

        double _delta = _pData->m_timeData.m_dDelta;

        _pData->m_dOneSecond += _delta;

        {
            ZoneScopedN("Update engine systems");

            for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); ++i)
            {
                system& _system = _pData->m_aEngineSystems[i];

                if(_system.m_bIsHot)
                {
					(static_cast<ISystem*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_system.m_pObject))->*_system.m_fnUpdateCallback)(_delta);
                }
                else
                {
                    (static_cast<ISystem*>(_system.m_pObject)->*_system.m_fnUpdateCallback)(_delta);
                }
            }
        }

#ifndef DUCKVIL_HEADLESS_SERVER
        if(_pData->m_bIsClient)
        {
            _pData->m_pClient->Update();
        }
#endif

        {
            ZoneScopedN("Update project");

            _pData->m_projectManager.m_fnUpdate(&_pData->m_projectManagerData, _delta);
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

#ifndef DUCKVIL_HEADLESS_SERVER

	    {
            ZoneScopedN("Update native scripts");

            _pData->m_scriptsQuery.each([_delta, _pData](flecs::entity _entity, const ScriptComponent& _c)
            {
            	const uint32_t _size = Memory::fixed_vector_size(_pData->m_pMemory, _c.m_pScripts) / 8;

                for(uint32_t _i = 0; _i < _size; ++_i)
                {
                    const auto _trackKeeper = *static_cast<HotReloader::ITrackKeeper**>(Memory::fixed_vector_at(_pData->m_pMemory, _c.m_pScripts, _i));
                    const auto _object = static_cast<NativeScriptBase*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_trackKeeper));
                    const auto _start = std::chrono::high_resolution_clock::now().time_since_epoch().count();

                    _object->Update(_delta);

                    const auto _end = std::chrono::high_resolution_clock::now().time_since_epoch().count();

                    _object->SetDelta(std::chrono::duration<double, std::milli>(_end - _start).count());
                }
            });
        }

        {
            ZoneScopedN("Update C#");

            _pData->m_pCSharpLanguage->Update();
        }

        {
            ZoneScopedN("Renderer");

            _pData->m_pRenderer->m_fnUpdate(_pData->m_pMemory, &_pData->m_pRendererData);

            _pData->m_pRenderer->m_fnBindAsRenderTarget();
        }

        {
            ZoneScopedN("Editor");

            _pData->m_pEditor->m_fnRender(_pData->m_pEditorData, _pData->m_pWindow);
        }

        {
            ZoneScopedN("ProcessWindowEventPool");

            _pData->m_windowEventPool.Reset();

            while(_pData->m_windowEventPool.AnyEvents())
            {
                static Window::CloseEvent _closeEvent;
                static Window::ResizeEvent _resizeEvent;
                static Window::SetMousePositionEvent _setMousePositionEvent;

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
                    _pData->m_pWindow->SetMousePosition(_setMousePositionEvent.m_iX, _setMousePositionEvent.m_iY, _setMousePositionEvent.m_bGlobal);

                    _pData->m_windowEventPool.EventHandled<Window::SetMousePositionEvent>();
                }
                else
                {
                    _pData->m_windowEventPool.Skip();
                }
            }
        }

#endif

#ifndef DUCKVIL_HEADLESS_SERVER
        if(_pData->m_bIsServer)
        {
            _pData->m_pServer->Update();
        }
#else
        _pData->m_pServer->Update();
#endif

#ifndef DUCKVIL_HEADLESS_SERVER
        _pData->m_pWindow->Refresh();
#endif
        _pData->m_time.update(&_pData->m_timeData);

#ifndef DUCKVIL_HEADLESS_SERVER
        _pData->m_windowEventPool.Clear();
#endif

//         {
//             ZoneScopedN("Sleeping, zzz...");

// #ifdef DUCKVIL_PLATFORM_WINDOWS
//             Sleep(33);
// #endif
//         }
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