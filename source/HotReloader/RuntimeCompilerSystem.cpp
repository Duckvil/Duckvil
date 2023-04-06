#include "HotReloader/RuntimeCompilerSystem.h"

#include "Serializer/Runtime/Serializer.h"

#include "Utils/AST.h"

#include <cassert>
#include <filesystem>
#include <vector>

#include "RuntimeReflection/Meta.h"

#include "ImGui/imgui.h"

#undef max

#include "tracy/Tracy.hpp"

#include "RuntimeReflection/PrepareObjectEvent.h"

#include "HotReloader/Events/InternalSwapEvent.h"

#undef GetObject
#undef GetMessage;

namespace Duckvil { namespace HotReloader {

    void generate(std::ofstream& _hFile, std::ofstream& _sFile, void* _pUserData)
    {
        RuntimeCompilerSystem* _system = static_cast<RuntimeCompilerSystem*>(_pUserData);
        std::string _fileID = _system->m_path.string();

        _fileID = Duckvil::Utils::replace_all(_fileID, "\\", "_");
        _fileID = Duckvil::Utils::replace_all(_fileID, ".", "_");

        _hFile << "#include \"Serializer/Runtime/ISerializer.h\"\n\n";
        _hFile << "#include \"RuntimeReflection/Markers.h\"\n\n";

        std::vector<std::pair<uint32_t, std::vector<std::string>>> _generated;

        for(auto& _module : _system->m_aModules)
        {
            Duckvil::RuntimeReflection::invoke_member<std::ofstream&, std::ofstream&, Duckvil::RuntimeReflection::GeneratedVector&>(_module.m_typeHandle, _module.m_generateCustomFunctionHandle, _module.m_pObject, _hFile, _sFile, _generated);
        }

        for(const auto& _generated2 : _generated)
        {
            _hFile << "#define " << _fileID << "_" << _generated2.first << "_GENERATED_BODY";

            if(_generated2.second.size())
            {
                _hFile << " \\\n";
            }

            for(uint32_t i = 0; i < _generated2.second.size(); ++i)
            {
                _hFile << _fileID << "_" << _generated2.first << "_REFLECTION_MODULE_" << _generated2.second[i];

                if(i < _generated2.second.size() - 1)
                {
                    _hFile << " \\\n";
                }
                else
                {
                    _hFile << "\n\n";
                }
            }
        }

        _hFile << "#undef DUCKVIL_CURRENT_FILE_ID\n";
        _hFile << "#define DUCKVIL_CURRENT_FILE_ID " << _fileID << "\n";
    }

    void RuntimeCompilerSystem::HotReload(const Utils::string& _sModuleFilename, void (*_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&))
    {
        FrameMarkStart("LoadModule");

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _testModule(_sModuleFilename, DUCKVIL_SWAP_OUTPUT);
        uint32_t (*get_recorder_index)();
        RuntimeReflection::RecordFunction record = nullptr;

        _module.load(&_testModule);

        if(_testModule.m_pModule == nullptr)
        {
            printf("Failed to load hot module!\n");

            return;
        }

        FrameMarkEnd("LoadModule");

        FrameMarkStart("PrepareForSwap");
        void (*make_current_runtime_reflection_context)(const duckvil_frontend_reflection_context&);
        void (*make_current_logger_context)(const logger_context&);

        _module.get(_testModule, "duckvil_get_recorder_index", reinterpret_cast<void**>(&get_recorder_index));
        _module.get(_testModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(get_recorder_index())).c_str(), reinterpret_cast<void**>(&record));
        _module.get(_testModule, "duckvil_plugin_make_current_runtime_reflection_context", reinterpret_cast<void**>(&make_current_runtime_reflection_context));
        _module.get(_testModule, "duckvil_plugin_make_current_logger_context", reinterpret_cast<void**>(&make_current_logger_context));

        FrameMarkStart("SetupContexts");

        make_current_runtime_reflection_context(RuntimeReflection::get_current());
        make_current_logger_context(logger_get_current());

        FrameMarkEnd("SetupContexts");
        FrameMarkStart("RecordTypeReflection");

        duckvil_runtime_reflection_recorder_stuff _stuff =
        {
            ._pMemoryInterface = m_heap.GetMemoryInterface(),
            ._pAllocator = m_heap.GetAllocator(),
            ._pFunctions = RuntimeReflection::get_current().m_pRecorder,
            ._pData = RuntimeReflection::get_current().m_pReflectionData
        };

        duckvil_recorderd_types _types = record(_stuff);

        FrameMarkEnd("RecordTypeReflection");

        FrameMarkEnd("PrepareForSwap");
        DUCKVIL_LOG_INFO(LoggerChannelID::Default, "Swapping objects");
        FrameMarkStart("Swapping");

        m_eventPool.Broadcast(InternalSwapEvent{ &m_aHotObjects, _types, _fnSwap });

        FrameMarkEnd("Swapping");
        DUCKVIL_LOG_INFO(LoggerChannelID::Default, "Swapping objects finished");
    }

    void RuntimeCompilerSystem::HotReload(const Utils::string& _sModuleFilename)
    {
        HotReload(_sModuleFilename,
            Utils::lambda([this](Memory::Vector<RuntimeCompilerSystem::hot_object>* _pHotObjects, duckvil_recorderd_types& _newTypes)
                {
                    Swap(_pHotObjects, _newTypes);
                }
            )
        );
    }

    void RuntimeCompilerSystem::Swap(Memory::Vector<RuntimeCompilerSystem::hot_object>* _pHotObjects, duckvil_recorderd_types& _newTypes)
    {
        bool _found = false;

        
	    for(size_t j = 0; j < _newTypes.m_ullCount; ++j)
	    {
	        const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _newTypes.m_aTypes[j];

            for(uint32_t i = 0; i < _pHotObjects->Size(); ++i)
            {
                RuntimeCompilerSystem::hot_object& _hot = _pHotObjects->At(i);

                if(/*RuntimeReflection::get_meta(_type, ReflectionFlags_Hot).m_ullTypeID != -1 && */_hot.m_pObject->GetTypeHandle().m_ID == _type.m_ID)
                {
                    _found = true;

                    if(!Swap(&_hot, _type))
                    {
                        break;
                    }

                    for(size_t k = 0; k < m_aReflectedTypes->Size(); ++k)
                    {
                        const duckvil_recorderd_types& _types2 = m_aReflectedTypes->At(k);

                        if(strcmp(_types2.m_sFile, _newTypes.m_sFile) == 0)
                        {
                            m_heap.Free(_types2.m_aTypes);

                            // _moduleToRelease = _types2.m_pModule;

                            m_aReflectedTypes->Erase(k);
                            const duckvil_recorderd_types& aa = m_aReflectedTypes->At(k);
                            m_aReflectedTypes->Allocate(_newTypes);

                            break;
                        }
                    }
                }
            }
        }

        if(_found)
        {
            return;
        }

        for(size_t j = 0; j < _newTypes.m_ullCount; ++j)
        {
            const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _typeInfo = _newTypes.m_aTypes[j];
            RuntimeReflection::ReflectedType _type(_typeInfo);
            auto _autoInstantiateMetaFlag = _type.GetMeta(Duckvil::ReflectionFlags::ReflectionFlags_AutoInstantiate);

            if((!_type.Inherits<Editor::Widget>() && !_type.Inherits<ISystem>()) ||
                (_autoInstantiateMetaFlag.m_pData != nullptr && !*static_cast<bool*>(_autoInstantiateMetaFlag.m_pData)))
            {
                continue;
            }

            auto _constructors = RuntimeReflection::get_constructors(m_heap, { _typeInfo.m_ID });

            for(/*const auto& _constructorHandle : _constructors*/ uint32_t i = 0; i < _constructors.Size(); ++i)
            {
                const auto& _constructorHandle = _constructors[i];
                const auto& _constructor = RuntimeReflection::get_constructor({ _typeInfo.m_ID }, _constructorHandle);
                uint32_t _constructorArgumentsCount = DUCKVIL_SLOT_ARRAY_SIZE(_constructor.m_arguments);

                RuntimeDependencyInjector _fap(5 + _constructorArgumentsCount);

                _fap.Push(m_heap.GetMemoryInterface());
                _fap.Push(m_heap.GetAllocator());
                _fap.Push(RuntimeReflection::get_current().m_pReflection);
                _fap.Push(RuntimeReflection::get_current().m_pReflectionData);
                _fap.Push(true);

                _fap.Call(_constructor.m_pData);
                _fap.getCode<void*(*)()>()();
            }
        }
    }

    RuntimeCompilerSystem::RuntimeCompilerSystem(
        const Memory::FreeList& _heap,
        Event::Pool<Event::mode::immediate>* _pEventPool,
        Event::Pool<Event::mode::immediate>* _pRuntimeReflectionEventPool,
        FileWatcher::ActionCallback _fnAction,
        void* _pActionData
    ) :
        m_heap(_heap),
        m_pEventPool(_pEventPool),
        m_pRuntimeReflectionEventPool(_pRuntimeReflectionEventPool)
    {
        _heap.Allocate(m_aHotObjects, 1);
        _heap.Allocate(m_aModules, 1);

        m_eventPool = Event::Pool<Event::mode::buffered>(_heap, RuntimeReflection::get_current().m_pReflectionData);

        DUCKVIL_DEBUG_MEMORY(m_aHotObjects.GetAllocator(), "m_aHotObjects");
        DUCKVIL_DEBUG_MEMORY(m_aModules.GetAllocator(), "m_aModules");

        m_pFileWatcher = _heap.Allocate<FileWatcher>(_fnAction, _pActionData);

        RuntimeReflection::__duckvil_resource_type_t _runtimeCompilerHandle = RuntimeReflection::get_type<RuntimeCompiler::Compiler>();

        m_pCompiler = static_cast<RuntimeCompiler::Compiler*>(
            RuntimeReflection::create<
                const Memory::FreeList&
            >(
                _heap.GetMemoryInterface(),
                _heap.GetAllocator(),
                g_duckvilFrontendReflectionContext.m_pReflection,
                g_duckvilFrontendReflectionContext.m_pReflectionData,
                _runtimeCompilerHandle,
                false,
                _heap
            )
        );

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _threadModule("Thread");

        _module.load(&_threadModule);

        Thread::pool_ftable* (*init)();

        _module.get(_threadModule, "duckvil_thread_pool_init", reinterpret_cast<void**>(&init));

        m_pThread = init();

        m_compilerTypeHandle = RuntimeReflection::get_type<RuntimeCompiler::Compiler>();

        m_fnInternalCompilerSetup = RuntimeReflection::get_function_callback_mr<bool, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "Setup");
        m_fnInternalCompilerAddFlag = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddFlag");
        m_fnInternalCompilerAddFlag2 = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, RuntimeCompiler::Flag>(m_compilerTypeHandle, "AddFlag");
        m_fnInternalCompilerAddDefine = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddDefine");
        m_fnInternalCompilerAddInclude = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddInclude");
        m_fnInternalCompilerAddLibraryPath = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddLibraryPath");
        m_fnInternalCompilerAddLibrary = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddLibrary");

        m_fnInternalCompilerGetFlags =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetFlags");
        m_fnInternalCompilerGetDefines =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetDefines");
        m_fnInternalCompilerGetIncludes =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetIncludes");
        m_fnInternalCompilerGetLibrariesPaths =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetLibrariesPaths");
        m_fnInternalCompilerGetLibraries =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetLibraries");

        (m_pCompiler->*m_fnInternalCompilerSetup->m_fnFunction)();
    }

    RuntimeCompilerSystem::RuntimeCompilerSystem(
        const Memory::FreeList& _heap,
        Event::Pool<Event::mode::immediate>* _pEventPool,
        Event::Pool<Event::mode::immediate>* _pRuntimeReflectionEventPool
    ) :
        m_heap(_heap),
        m_pEventPool(_pEventPool),
        m_pRuntimeReflectionEventPool(_pRuntimeReflectionEventPool)
    {
        _heap.Allocate(m_aHotObjects, 1);
        _heap.Allocate(m_aModules, 1);

        m_eventPool = Event::Pool<Event::mode::buffered>(_heap, RuntimeReflection::get_current().m_pReflectionData);

        DUCKVIL_DEBUG_MEMORY(m_aHotObjects.GetAllocator(), "m_aHotObjects");
        DUCKVIL_DEBUG_MEMORY(m_aModules.GetAllocator(), "m_aModules");

        m_userData.m_pRuntimeCompiler = this;

        m_pFileWatcher = _heap.Allocate<FileWatcher, FileWatcher::ActionCallback, void*>(&Action, (void*)&m_userData);

        RuntimeReflection::__duckvil_resource_type_t _runtimeCompilerHandle = RuntimeReflection::get_type<RuntimeCompiler::Compiler>();

        m_pCompiler = static_cast<RuntimeCompiler::Compiler*>(
            RuntimeReflection::create<
                const Memory::FreeList&
            >(
                _heap.GetMemoryInterface(),
                _heap.GetAllocator(),
                g_duckvilFrontendReflectionContext.m_pReflection,
                g_duckvilFrontendReflectionContext.m_pReflectionData,
                _runtimeCompilerHandle,
                false,
                _heap
            )
        );

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _threadModule("Thread");

        _module.load(&_threadModule);

        Thread::pool_ftable* (*init)();

        _module.get(_threadModule, "duckvil_thread_pool_init", reinterpret_cast<void**>(&init));

        m_pThread = init();

        m_compilerTypeHandle = RuntimeReflection::get_type<RuntimeCompiler::Compiler>();

        m_fnInternalCompilerSetup = RuntimeReflection::get_function_callback_mr<bool, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "Setup");
        m_fnInternalCompilerAddFlag = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddFlag");
        m_fnInternalCompilerAddFlag2 = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, RuntimeCompiler::Flag>(m_compilerTypeHandle, "AddFlag");
        m_fnInternalCompilerAddDefine = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddDefine");
        m_fnInternalCompilerAddInclude = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddInclude");
        m_fnInternalCompilerAddLibraryPath = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddLibraryPath");
        m_fnInternalCompilerAddLibrary = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddLibrary");

        m_fnInternalCompilerGetFlags =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetFlags");
        m_fnInternalCompilerGetDefines =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetDefines");
        m_fnInternalCompilerGetIncludes =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetIncludes");
        m_fnInternalCompilerGetLibrariesPaths =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetLibrariesPaths");
        m_fnInternalCompilerGetLibraries =
            RuntimeReflection::get_function_callback_mr<const std::vector<std::string>&, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "GetLibraries");

        (m_pCompiler->*m_fnInternalCompilerSetup->m_fnFunction)();
    }

    RuntimeCompilerSystem::~RuntimeCompilerSystem()
    {

    }

    bool RuntimeCompilerSystem::Init()
    {
        m_pFileWatcher->Watch(std::filesystem::path(DUCKVIL_CWD) / "include");

        return Init({ std::filesystem::path(DUCKVIL_CWD) / "source" });
    }

    bool RuntimeCompilerSystem::Init(const std::vector<std::filesystem::path>& _sDirectoriesToWatch)
    {
        m_pThreadData = *static_cast<Thread::pool_data**>(RuntimeReflection::get_meta_value_ptr(RuntimeReflection::get_type("__data", { "Duckvil" }), "Thread"));

        {
            auto _types = Duckvil::RuntimeReflection::get_types(m_heap);

            for(auto& _typeHandle : _types)
            {
                const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(_typeHandle, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule);

                if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() && (uint8_t)_variant.m_traits & (uint8_t)Duckvil::RuntimeReflection::property_traits::is_bool)
                {
                    reflection_module _module = {};

                    _module.m_pObject =
                        Duckvil::RuntimeReflection::create<
                            const Duckvil::Memory::FreeList&,
                            Duckvil::RuntimeReflection::__ftable*,
                            Duckvil::RuntimeReflection::__data*
                        >(
                            m_heap.GetMemoryInterface(),
                            m_heap.GetAllocator(),
                            g_duckvilFrontendReflectionContext.m_pReflection,
                            g_duckvilFrontendReflectionContext.m_pReflectionData,
                            _typeHandle,
                            false,
                            m_heap,
                            g_duckvilFrontendReflectionContext.m_pReflection,
                            g_duckvilFrontendReflectionContext.m_pReflectionData
                        );
                    _module.m_typeHandle = _typeHandle;
                    _module.m_generateCustomFunctionHandle = Duckvil::RuntimeReflection::get_function_handle<std::ofstream&, std::ofstream&, Duckvil::RuntimeReflection::GeneratedVector&>(_typeHandle, "GenerateCustom");
                    _module.m_clearFunctionHandle = Duckvil::RuntimeReflection::get_function_handle(_typeHandle, "Clear");
                    _module.m_processAST_FunctionHandle = Duckvil::RuntimeReflection::get_function_handle<Duckvil::Parser::__ast*>(_typeHandle, "ProcessAST");

                    if(m_aModules.Full())
                    {
                        m_aModules.Resize(m_aModules.Size() * 2);
                    }

                    m_aModules.Allocate(_module);
                }
            }
        }

        (m_pCompiler->*m_fnInternalCompilerAddFlag2->m_fnFunction)(RuntimeCompiler::Flag::Flag_SharedLibrary);
        (m_pCompiler->*m_fnInternalCompilerAddFlag2->m_fnFunction)(RuntimeCompiler::Flag::Flag_DebugInfo);

        (m_pCompiler->*m_fnInternalCompilerAddFlag->m_fnFunction)("/std:c++latest");

#ifdef DUCKVIL_PLATFORM_WINDOWS
        // (m_pCompiler->*_addFlag->m_fnFunction)("/Zi");
        (m_pCompiler->*m_fnInternalCompilerAddFlag->m_fnFunction)("/MDd");
        // (m_pCompiler->*_addFlag->m_fnFunction)("/LD");
        (m_pCompiler->*m_fnInternalCompilerAddFlag->m_fnFunction)("/FC");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        // (m_pCompiler->*_addFlag->m_fnFunction)("-g");
        // (m_pCompiler->*_addFlag->m_fnFunction)("-shared");
        (m_pCompiler->*m_fnInternalCompilerAddFlag->m_fnFunction)("-fPIC");
#endif
#endif
        std::filesystem::path _includePath = m_CWD / "include";
        std::filesystem::path _externalPath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "external";
        std::filesystem::path _generatedIncludePath = m_CWD / "__generated_reflection__";

        (m_pCompiler->*m_fnInternalCompilerAddInclude->m_fnFunction)(_includePath.string());
        (m_pCompiler->*m_fnInternalCompilerAddInclude->m_fnFunction)(_externalPath.string());
        (m_pCompiler->*m_fnInternalCompilerAddInclude->m_fnFunction)((_externalPath / "json" / "include").string());
        (m_pCompiler->*m_fnInternalCompilerAddInclude->m_fnFunction)(_generatedIncludePath.string());
        (m_pCompiler->*m_fnInternalCompilerAddInclude->m_fnFunction)((_externalPath / "glm").string());
        (m_pCompiler->*m_fnInternalCompilerAddInclude->m_fnFunction)((_externalPath / "xbyak").string());

#ifdef DUCKVIL_PLATFORM_WINDOWS
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)("DUCKVIL_PLATFORM_WINDOWS");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)("DUCKVIL_PLATFORM_LINUX");
#endif
#endif
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)("DUCKVIL_RUNTIME_COMPILE");
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)("DUCKVIL_COMPILE_REFLECTION_SOURCE");
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)(std::string("DUCKVIL_OUTPUT=\"") + DUCKVIL_OUTPUT + "\"");
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)(std::string("DUCKVIL_CWD=\"") + DUCKVIL_CWD + "\"");
        (m_pCompiler->*m_fnInternalCompilerAddDefine->m_fnFunction)(std::string("DUCKVIL_PROJECTS_PATH=\"") + DUCKVIL_PROJECTS_PATH + "\"");

        (m_pCompiler->*m_fnInternalCompilerAddLibraryPath->m_fnFunction)(DUCKVIL_OUTPUT);

#ifdef DUCKVIL_PLATFORM_WINDOWS
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("Utils.lib");
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("UniTestFramework.lib");
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("PlugNPlay.lib");
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("flecs_static.lib");
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("RuntimeReflectionStatic.lib");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("Utils.a");
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("UniTestFramework.a");
        (m_pCompiler->*m_fnInternalCompilerAddLibrary->m_fnFunction)("PlugNPlay.a");
#endif
#endif

        // m_pFileWatcher->Watch(_sDirectoryToWatch);
        for(const auto& _dirToWatch : _sDirectoriesToWatch)
        {
            m_pFileWatcher->Watch(_dirToWatch);
        }

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _reflectionModule("RuntimeReflection");
        PlugNPlay::__module_information _parser("Parser");

        _module.load(&_reflectionModule);
        _module.load(&_parser);

        {
            RuntimeReflection::__generator_ftable* (*_runtime_reflection_generator)();

            _module.get(_reflectionModule, "duckvil_runtime_reflection_generator_init", reinterpret_cast<void**>(&_runtime_reflection_generator));

            m_pReflectionGenerator = _runtime_reflection_generator();
        }

        {
            Parser::__lexer_ftable* (*_lexer_init)();
            Parser::__ast_ftable* (*_ast_init)();

            _module.get(_parser, "duckvil_lexer_init", reinterpret_cast<void**>(&_lexer_init));
            _module.get(_parser, "duckvil_ast_init", reinterpret_cast<void**>(&_ast_init));

            m_pAST_FTable = _ast_init();
            m_pLexerFTable = _lexer_init();
        }

        return true;
    }

    void RuntimeCompilerSystem::Update(double _dDelta)
    {
        ZoneScopedN("Hot-reloader update")

        m_eventPool.Reset();

        while(m_eventPool.AnyEvents())
        {
            InternalSwapEvent _swapEvent;

            if(m_eventPool.GetMessage(&_swapEvent))
            {
                _swapEvent.m_fnSwap(_swapEvent.m_pHotObjects, _swapEvent.m_recordedTypes);

                m_eventPool.EventHandled<InternalSwapEvent>();
            }
        }

        m_pFileWatcher->Update();
    }

    void RuntimeCompilerSystem::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    void RuntimeCompilerSystem::OnDraw()
    {
        ImGui::Begin("RuntimeCompilerSystem");

        ImGui::Text("Hot types:");

        for(uint32_t i = 0; i < m_aHotObjects.Size(); ++i)
        {
            auto _type = RuntimeReflection::get_type(m_aHotObjects[i].m_pObject->GetTypeHandle());

            ImGui::Text("  %s", _type.m_sTypeName);
        }

        ImGui::End();
    }

    void RuntimeCompilerSystem::Compile(
        const std::string& _sFile,
        void (*_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&),
        const RuntimeCompiler::Options& _compileOptions)
    {
        FrameMarkStart("Compile");

        DUCKVIL_LOG_INFO(LoggerChannelID::Default, "Hot reload started");

        std::filesystem::path _path = std::tmpnam(nullptr);

        m_sModuleName = _path.filename().string();

        {
            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "Compilation");
            FrameMarkStart("Compilation");

            // std::filesystem::path _working = std::filesystem::path(DUCKVIL_OUTPUT).parent_path();
            std::filesystem::path _file = _sFile;
            std::string _filename = _file.filename().string();
            std::filesystem::path _relativePath = std::filesystem::relative(_file.parent_path(), m_CWD / "source");
            std::size_t _dotPosition = _filename.find_first_of('.');

            if(_dotPosition != std::string::npos)
            {
                _filename.erase(_filename.begin() + _dotPosition, _filename.end());
            }

            std::string _generatedFilename = _filename + ".generated.cpp";
            std::filesystem::path _generatedFile = m_CWD / "__generated_reflection__" / _relativePath / _generatedFilename;
            std::filesystem::path _pluginFile = m_CWD / "__generated_reflection__" / *_relativePath.begin() / "plugin_info.cpp";
            std::filesystem::path _externalPath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "external";

            RuntimeCompiler::Options _options = {};

#ifdef DUCKVIL_PLATFORM_WINDOWS
            _options.m_aFlags.push_back("/Fe" + std::string(DUCKVIL_SWAP_OUTPUT) + "/" + m_sModuleName + ".dll");
            _options.m_aFlags.push_back("/Fd" + std::string(DUCKVIL_SWAP_OUTPUT) + "/" + m_sModuleName + ".pdb");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
            _options.m_aFlags.push_back("-o" + std::string(DUCKVIL_SWAP_OUTPUT) + "/" + m_sModuleName + ".so");
#endif
#endif

#ifdef DUCKVIL_HOT_RELOADING
            _options.m_aDefines.push_back("DUCKVIL_HOT_RELOADING");
#endif

            for(const std::string& _option : _compileOptions.m_aDefines)
            {
                _options.m_aDefines.push_back(_option);
            }

            for(const std::string& _option : _compileOptions.m_aFlags)
            {
                _options.m_aFlags.push_back(_option);
            }

            for(const std::string& _option : _compileOptions.m_aIncludes)
            {
                _options.m_aIncludes.push_back(_option);
            }

            for(const std::string& _option : _compileOptions.m_aLibrariesPaths)
            {
                _options.m_aLibrariesPaths.push_back(_option);
            }

            for(const std::string& _option : _compileOptions.m_aLibraries)
            {
                _options.m_aLibraries.push_back(_option);
            }

            RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::vector<std::string>&, const RuntimeCompiler::Options&)>* _compile = RuntimeReflection::get_function_callback_m<RuntimeCompiler::Compiler, const std::vector<std::string>&, const RuntimeCompiler::Options&>(m_compilerTypeHandle, "Compile");

            BeforeCompileEvent _beforeCompileEvent;

            m_pEventPool->Broadcast(_beforeCompileEvent);

            (m_pCompiler->*_compile->m_fnFunction)(
                {
                    _sFile,
                    _generatedFile.string(),
                    _pluginFile.string(),
                    (_externalPath / "imgui/imgui.cpp").string(),
                    (_externalPath / "imgui/imgui_draw.cpp").string(),
                    (_externalPath / "imgui/imgui_tables.cpp").string(),
                    (_externalPath / "imgui/imgui_widgets.cpp").string(),
                },
            _options);

            AfterCompileEvent _afterCompileEvent;

            m_pEventPool->Broadcast(_afterCompileEvent);

            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "Compilation finished");
            FrameMarkEnd("Compilation");
        }

        HotReload(m_sModuleName, _fnSwap);

        FrameMarkStart("Releasing module");

        // if(_moduleToRelease != nullptr)
        // {
        //     // TODO: Here we should release global contexts

        //     void (*free_current_logger_context)();

        //     _module.get(_testModule, "duckvil_plugin_free_current_logger_context", reinterpret_cast<void**>(&free_current_logger_context));

        //     // free_current_logger_context();

        //     _module.free(_moduleToRelease);
        // }

        FrameMarkEnd("Releasing module");

        FrameMarkEnd("Compile");
    }

    void RuntimeCompilerSystem::CompileT(const std::string& _sFile, const RuntimeCompiler::Options& _compileOptions)
    {
        struct thread_data
        {
            RuntimeCompilerSystem* m_pCompiler;
            std::string m_sFile;
            RuntimeCompiler::Options m_options;
        };

        thread_data* _threadData = new thread_data
        {
            this,
            _sFile,
            _compileOptions
        };

        m_pThread->m_fnOrderDataTask(m_pThreadData, Utils::lambda([](void* _pData)
        {
            thread_data* _data = static_cast<thread_data*>(_pData);

            _data->m_pCompiler->Compile(_data->m_sFile, _data->m_options);

            delete _pData;
        }), _threadData);
    }

    void RuntimeCompilerSystem::Compile(const std::string& _sFile, const RuntimeCompiler::Options& _compileOptions)
    {
        Compile(
            _sFile,
            Utils::lambda(
                [this](Memory::Vector<RuntimeCompilerSystem::hot_object>* _pHotObjects, duckvil_recorderd_types& _newTypes)
                {
                    Swap(_pHotObjects, _newTypes);
                }
            ),
            _compileOptions
        );
    }

    bool RuntimeCompilerSystem::Swap(hot_object* _pHotObject, const RuntimeReflection::__duckvil_resource_type_t& _typeHandle)
    {
        RuntimeReflection::__duckvil_resource_function_t _serializeFunctionHandle = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(_typeHandle, "Serialize");

        if(_serializeFunctionHandle.m_ID == -1)
        {
            DUCKVIL_LOG_FATAL(LoggerChannelID::Default, "Hot-reloader swap failed: 'Serialize' function not found!");

            return false;
        }

        RuntimeSerializer::Serializer _serializer;

        void* _oldObject = DUCKVIL_TRACK_KEEPER_GET_OBJECT(_pHotObject->m_pObject);

        _serializer.SetLoading(false);
        _serializer.Serialize(_oldObject, _pHotObject->m_pSerializeFunction);

        m_pRuntimeReflectionEventPool->AddA<RuntimeReflection::PrepareObjectEvent>([](const RuntimeReflection::PrepareObjectEvent& _event)
        {
            // printf("AAAA\n");
            RuntimeReflection::add_object_meta(_event.m_pObject, "IsHot", true);
        });

        void* _newObject = RuntimeReflection::create(m_objectsHeap, g_duckvilFrontendReflectionContext.m_pReflection, g_duckvilFrontendReflectionContext.m_pReflectionData, _typeHandle, false);

        m_pRuntimeReflectionEventPool->Remove<RuntimeReflection::PrepareObjectEvent>();

        RuntimeReflection::__function_t _func = RuntimeReflection::get_function(_typeHandle, _serializeFunctionHandle);

        _serializer.SetLoading(true);
        _serializer.Serialize(_newObject, _func.m_pRawFunction);

        _pHotObject->m_pSerializeFunction = _func.m_pRawFunction;

        _pHotObject->m_pObject->SetObject(_newObject);

        m_pEventPool->Broadcast(SwapEvent{ _oldObject, _pHotObject->m_pObject, m_sModuleName });

        m_objectsHeap.Free(_oldObject);

        return true;
    }

    void RuntimeCompilerSystem::GenerateReflection(const std::filesystem::path& _CWD, const std::filesystem::path& _file, bool _bIsAbsolute)
    {
        PlugNPlay::__module _module;
        Process::ftable _process;
        Process::data _processData;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _processModuleInfo("Process");

        _module.load(&_processModuleInfo);

        void (*_duckvilProcessInit)(Process::ftable* _pFTable);

        _module.get(_processModuleInfo, "duckvil_process_init", reinterpret_cast<void**>(&_duckvilProcessInit));

        _duckvilProcessInit(&_process);

        _process.m_fnInit(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), &_processData);
        _process.m_fnSetup(&_processData);

        _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _CWD.string() + (_file != "" ? (" -file " + _file.string() + (_bIsAbsolute ? " -is_absolute" : " -is_relative")) : "") + "\n_COMPLETION_TOKEN_\n").c_str());
        _process.m_fnWait(&_processData);
        _process.m_fnStop(&_processData);

        if(_process.m_fnTerminate(&_processData))
        {
            _process.m_fnCleanup(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), &_processData);
        }
    }

    void RuntimeCompilerSystem::AddHotObject(const hot_object& _hotObject)
    {
        if(m_aHotObjects.Full())
        {
            m_aHotObjects.Resize(m_aHotObjects.Size() * 2);
        }

        m_aHotObjects.Allocate(_hotObject);
    }

    void RuntimeCompilerSystem::SetObjectsHeap(const Memory::FreeList& _heap)
    {
        m_objectsHeap = _heap;
    }

    void RuntimeCompilerSystem::SetModules(Memory::Vector<PlugNPlay::__module_information>* _aLoaded)
    {
        m_aLoadedModules = _aLoaded;
    }

    void RuntimeCompilerSystem::SetReflectedTypes(Memory::ThreadsafeVector<duckvil_recorderd_types>* _aReflected)
    {
        m_aReflectedTypes = _aReflected;
    }

    void RuntimeCompilerSystem::SetCWD(const std::filesystem::path& _CWD)
    {
        m_CWD = _CWD;
    }

    const std::filesystem::path& RuntimeCompilerSystem::GetCWD() const
    {
        return m_CWD;
    }

    void RuntimeCompilerSystem::OnEvent(const RuntimeReflection::TrackedObjectCreatedEvent& _event)
    {
        RuntimeReflection::__duckvil_resource_function_t _a = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(_event.m_pTrackKeeper->GetTypeHandle(), "Serialize");
        RuntimeReflection::__function_t _func = RuntimeReflection::get_function(_event.m_pTrackKeeper->GetTypeHandle(), _a);

        AddHotObject(hot_object(_event.m_pTrackKeeper, _func.m_pRawFunction));
    }

}}