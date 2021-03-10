#include "HotReloader/RuntimeCompilerSystem.h"

#include "Serializer/Runtime/Serializer.h"

#include "Utils/AST.h"

#include <cassert>

#include "RuntimeReflection/Meta.h"

#undef max

namespace Duckvil { namespace HotReloader {

    void generate(std::ofstream& _file, void* _pUserData)
    {
        _file << "#include \"Serializer/Runtime/ISerializer.h\"\n\n";
        _file << "#define DUCKVIL_GENERATED_BODY";

        RuntimeCompilerSystem* _system = (RuntimeCompilerSystem*)_pUserData;

        for(auto& _module : _system->m_aModules)
        {
            Duckvil::RuntimeReflection::invoke_member<std::ofstream&>(_module.m_typeHandle, _module.m_generateCustomFunctionHandle, _module.m_pObject, _file);
        }
    }

    RuntimeCompilerSystem::RuntimeCompilerSystem(
        const Memory::FreeList& _heap,
        Event::Pool<Event::mode::immediate>* _pEventPool
    ) :
        m_heap(_heap),
        m_pEventPool(_pEventPool)
    {
        _heap.Allocate(m_aHotObjects, 1);
        _heap.Allocate(m_aModules, 1);

        DUCKVIL_DEBUG_MEMORY(m_aHotObjects.GetAllocator(), "m_aHotObjects");
        DUCKVIL_DEBUG_MEMORY(m_aModules.GetAllocator(), "m_aModules");

        m_userData.m_pRuntimeCompiler = this;

        m_pFileWatcher = _heap.Allocate<FileWatcher, FileWatcher::ActionCallback, void*>(&Action, (void*)&m_userData);

        RuntimeReflection::__duckvil_resource_type_t _runtimeCompilerHandle = RuntimeReflection::get_type<RuntimeCompiler::Compiler>();

        m_pCompiler = (RuntimeCompiler::Compiler*)RuntimeReflection::create<const Memory::FreeList&>(_heap.GetMemoryInterface(), _heap.GetAllocator(), g_duckvilFrontendReflectionContext.m_pReflectionData, _runtimeCompilerHandle, _heap);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _threadModule("Thread");

        _module.load(&_threadModule);

        Thread::pool_ftable* (*init)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

        _module.get(_threadModule, "duckvil_thread_pool_init", (void**)&init);

        m_pThread = init(_heap.GetMemoryInterface(), _heap.GetAllocator());
    }

    RuntimeCompilerSystem::~RuntimeCompilerSystem()
    {

    }

    bool RuntimeCompilerSystem::Init()
    {
        m_pThreadData = *(Thread::pool_data**)RuntimeReflection::get_meta_value_ptr(RuntimeReflection::get_type("__data", { "Duckvil" }), "Thread");

        {
            auto _types = Duckvil::RuntimeReflection::get_types(m_heap);

            for(auto& _typeHandle : _types)
            {
                const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(_typeHandle, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule);

                if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() && (uint8_t)_variant.m_traits & (uint8_t)Duckvil::RuntimeReflection::__traits::is_bool)
                {
                    reflection_module _module = {};

                    _module.m_pObject = Duckvil::RuntimeReflection::create<const Duckvil::Memory::FreeList&, Duckvil::RuntimeReflection::__ftable*, Duckvil::RuntimeReflection::__data*>(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), g_duckvilFrontendReflectionContext.m_pReflectionData, _typeHandle, m_heap, g_duckvilFrontendReflectionContext.m_pReflection, g_duckvilFrontendReflectionContext.m_pReflectionData);
                    _module.m_typeHandle = _typeHandle;
                    _module.m_generateCustomFunctionHandle = Duckvil::RuntimeReflection::get_function_handle<std::ofstream&>(_typeHandle, "GenerateCustom");
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

        m_compilerTypeHandle = RuntimeReflection::get_type<RuntimeCompiler::Compiler>();

        RuntimeReflection::__function<bool(RuntimeCompiler::Compiler::*)()>* _setup =
            RuntimeReflection::get_function_callback<bool, RuntimeCompiler::Compiler>(m_compilerTypeHandle, "Setup");
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>* _addFlag =
            RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddFlag");
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(RuntimeCompiler::Flag)>* _addFlag2 =
            RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, RuntimeCompiler::Flag>(m_compilerTypeHandle, "AddFlag");
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>* _addDefine =
            RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddDefine");
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>* _addInclude =
            RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddInclude");
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>* _addLibraryPath =
            RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddLibraryPath");
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>* _addLibrary =
            RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, const std::string&>(m_compilerTypeHandle, "AddLibrary");

        (m_pCompiler->*_setup->m_fnFunction)();

        (m_pCompiler->*_addFlag2->m_fnFunction)(RuntimeCompiler::Flag::Flag_SharedLibrary);
        (m_pCompiler->*_addFlag2->m_fnFunction)(RuntimeCompiler::Flag::Flag_DebugInfo);

        (m_pCompiler->*_addFlag->m_fnFunction)("/std:c++latest");

#ifdef DUCKVIL_PLATFORM_WINDOWS
        // (m_pCompiler->*_addFlag->m_fnFunction)("/Zi");
        (m_pCompiler->*_addFlag->m_fnFunction)("/MDd");
        // (m_pCompiler->*_addFlag->m_fnFunction)("/LD");
        (m_pCompiler->*_addFlag->m_fnFunction)("/FC");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        // (m_pCompiler->*_addFlag->m_fnFunction)("-g");
        // (m_pCompiler->*_addFlag->m_fnFunction)("-shared");
        (m_pCompiler->*_addFlag->m_fnFunction)("-fPIC");
#endif
#endif
        std::filesystem::path _includePath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include";
        std::filesystem::path _externalPath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "external";
        std::filesystem::path _generatedIncludePath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__";

        (m_pCompiler->*_addInclude->m_fnFunction)(_includePath.string());
        (m_pCompiler->*_addInclude->m_fnFunction)(_externalPath.string());
        (m_pCompiler->*_addInclude->m_fnFunction)(_generatedIncludePath.string());

#ifdef DUCKVIL_PLATFORM_WINDOWS
        (m_pCompiler->*_addDefine->m_fnFunction)("DUCKVIL_PLATFORM_WINDOWS");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        (m_pCompiler->*_addDefine->m_fnFunction)("DUCKVIL_PLATFORM_LINUX");
#endif
#endif
        (m_pCompiler->*_addDefine->m_fnFunction)("DUCKVIL_RUNTIME_COMPILE");
        (m_pCompiler->*_addDefine->m_fnFunction)(std::string("DUCKVIL_OUTPUT=\"") + DUCKVIL_OUTPUT + "\"");

        (m_pCompiler->*_addLibraryPath->m_fnFunction)(DUCKVIL_OUTPUT);

#ifdef DUCKVIL_PLATFORM_WINDOWS
        (m_pCompiler->*_addLibrary->m_fnFunction)("Utils.lib");
        (m_pCompiler->*_addLibrary->m_fnFunction)("UniTestFramework.lib");
        (m_pCompiler->*_addLibrary->m_fnFunction)("PlugNPlay.lib");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        (m_pCompiler->*_addLibrary->m_fnFunction)("Utils.a");
        (m_pCompiler->*_addLibrary->m_fnFunction)("UniTestFramework.a");
        (m_pCompiler->*_addLibrary->m_fnFunction)("PlugNPlay.a");
#endif
#endif

        m_pFileWatcher->Watch(std::filesystem::path(DUCKVIL_CWD) / "source");

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _reflectionModule("RuntimeReflection");
        PlugNPlay::__module_information _parser("Parser");

        _module.load(&_reflectionModule);
        _module.load(&_parser);

        {
            RuntimeReflection::__generator_ftable* (*_runtime_reflection_generator)(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator);

            _module.get(_reflectionModule, "duckvil_runtime_reflection_generator_init", (void**)&_runtime_reflection_generator);

            m_pReflectionGenerator = _runtime_reflection_generator(m_heap.GetMemoryInterface(), m_heap.GetAllocator());
        }

        {
            Parser::__lexer_ftable* (*_lexer_init)(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator);
            Parser::__ast_ftable* (*_ast_init)(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator);

            _module.get(_parser, "duckvil_lexer_init", (void**)&_lexer_init);
            _module.get(_parser, "duckvil_ast_init", (void**)&_ast_init);

            m_pAST_FTable = _ast_init(m_heap.GetMemoryInterface(), m_heap.GetAllocator());
            m_pLexerFTable = _lexer_init(m_heap.GetMemoryInterface(), m_heap.GetAllocator());
        }

        return true;
    }

    void RuntimeCompilerSystem::Update()
    {
        m_pFileWatcher->Update();
    }

    void RuntimeCompilerSystem::Compile(const std::string& _sFile)
    {
        DUCKVIL_LOG_INFO("Hot reload started");

        {
            DUCKVIL_LOG_INFO("Generating reflection");

            std::filesystem::path _path = std::filesystem::relative(_sFile, std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "source");
            std::filesystem::path _generatePath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__" / _path;
            std::filesystem::path _source = _generatePath;
            std::filesystem::path _header = _generatePath;

        // TODO: Solve problem with reflection recorder count

            _path = _path.replace_extension(".h");

            std::string _toCmp = _path.string();
            uint32_t _foundRecorderID = -1;
            RuntimeReflection::__generator_data _generatorData;

            _generatorData.m_uiRecorderIndex = -1;

            std::replace(_toCmp.begin(), _toCmp.end(), '\\', '/');

            for(size_t i = 0; i < m_aRecordedTypes.Size(); ++i)
            {
                const duckvil_recorderd_types& _types = m_aRecordedTypes[i];

                if(strcmp(_types.m_sFile, _toCmp.c_str()) == 0)
                {
                    _generatorData.m_uiRecorderIndex = _types.m_uiRecorderID;

                    break;
                }
            }

            if(_path.string().size() < DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX)
            {
                strcpy(_generatorData.m_sInclude, _path.string().c_str());
            }
            else
            {
                assert(false && "Path is too long!");
            }

            _source.replace_extension(".generated.cpp");
            _header.replace_extension(".generated.h");

            _path = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include" / _path;

            Parser::__ast _astData;
            Parser::__lexer_data _data;

            m_pLexerFTable->load_file(&_data, _path.string().c_str());

            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_EXPORT", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "slot", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_RESOURCE_DECLARE", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_GENERATED_BODY", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_RESOURCE", &Utils::user_define_resource_behavior });

            m_pAST_FTable->ast_generate(&_astData, m_pLexerFTable, _data);

            for(auto& _reflectionModule : m_aModules)
            {
                Duckvil::RuntimeReflection::invoke_member<Duckvil::Parser::__ast*>(_reflectionModule.m_typeHandle, _reflectionModule.m_processAST_FunctionHandle, _reflectionModule.m_pObject, &_astData);
            }

            m_pReflectionGenerator->generate(&_generatorData, _source.string().c_str(), _header.string().c_str(), _astData, &generate, this);

            for(auto& _module : m_aModules)
            {
                Duckvil::RuntimeReflection::invoke_member(_module.m_typeHandle, _module.m_clearFunctionHandle, _module.m_pObject);
            }

            DUCKVIL_LOG_INFO("Generating reflection finished");
        }

        std::filesystem::path _path = std::tmpnam(nullptr);

        m_sModuleName = _path.filename().string();

        {
            DUCKVIL_LOG_INFO("Compilation");

            std::filesystem::path _working = std::filesystem::path(DUCKVIL_OUTPUT).parent_path();
            std::filesystem::path _file = _sFile;
            std::string _filename = _file.filename().string();
            std::filesystem::path _relativePath = std::filesystem::relative(_file.parent_path(), _working / "source");
            std::size_t _dotPosition = _filename.find_first_of('.');

            if(_dotPosition != std::string::npos)
            {
                _filename.erase(_filename.begin() + _dotPosition, _filename.end());
            }

            std::string _generatedFilename = _filename + ".generated.cpp";
            std::filesystem::path _generatedFile = _working / "__generated_reflection__" / _relativePath / _generatedFilename;
            std::filesystem::path _pluginFile = _working / "__generated_reflection__" / *_relativePath.begin() / "plugin_info.cpp";
            std::filesystem::path _externalPath = _working / "external";

            RuntimeCompiler::Options _options = {};

#ifdef DUCKVIL_PLATFORM_WINDOWS
            _options.m_aFlags.push_back("/Fe" + std::string(DUCKVIL_SWAP_OUTPUT) + "/" + m_sModuleName + ".dll");
            _options.m_aFlags.push_back("/Fd" + std::string(DUCKVIL_SWAP_OUTPUT) + "/" + m_sModuleName + ".pdb");
#else
#ifdef DUCKVIL_PLATFORM_LINUX
            _options.m_aFlags.push_back("-o" + std::string(DUCKVIL_SWAP_OUTPUT) + "/" + m_sModuleName + ".so");
#endif
#endif

            RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::vector<std::string>&, const RuntimeCompiler::Options&)>* _compile = RuntimeReflection::get_function_callback<RuntimeCompiler::Compiler, const std::vector<std::string>&, const RuntimeCompiler::Options&>(m_compilerTypeHandle, "Compile");

            HotReloadedEvent _beforeCompileEvent;

            _beforeCompileEvent.m_stage = HotReloadedEvent::stage_before_compile;

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

            HotReloadedEvent _afterCompileEvent;

            _afterCompileEvent.m_stage = HotReloadedEvent::stage_after_compile;

            m_pEventPool->Broadcast(_afterCompileEvent);

            DUCKVIL_LOG_INFO("Compilation finished");
        }

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _testModule(Utils::string(m_sModuleName.c_str(), strlen(m_sModuleName.c_str()) + 1), DUCKVIL_SWAP_OUTPUT);
        uint32_t (*get_recorder_index)();
        duckvil_recorderd_types (*record)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, RuntimeReflection::__recorder_ftable* _pRecorder, RuntimeReflection::__ftable* _pRuntimeReflection, RuntimeReflection::__data* _pData);

        _module.load(&_testModule);

        if(_testModule.m_pModule == nullptr)
        {
            printf("Failed to load hot module!\n");

            return;
        }

        void (*make_current_runtime_reflection_context)(const duckvil_frontend_reflection_context&);
        void (*make_current_logger_context)(const logger_context&);

        _module.get(_testModule, "duckvil_get_recorder_index", (void**)&get_recorder_index);
        _module.get(_testModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(get_recorder_index())).c_str(), (void**)&record);
        _module.get(_testModule, "duckvil_plugin_make_current_runtime_reflection_context", (void**)&make_current_runtime_reflection_context);
        _module.get(_testModule, "duckvil_plugin_make_current_logger_context", (void**)&make_current_logger_context);

        make_current_runtime_reflection_context(RuntimeReflection::get_current());
        make_current_logger_context(logger_get_current());

        duckvil_recorderd_types _types = record(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), g_duckvilFrontendReflectionContext.m_pRecorder, g_duckvilFrontendReflectionContext.m_pReflection, g_duckvilFrontendReflectionContext.m_pReflectionData);

        DUCKVIL_LOG_INFO("Swapping objects");

        for(uint32_t i = 0; i < m_aHotObjects.Size(); ++i)
        {
            ITrackKeeper* _trackKeeper = m_aHotObjects[i];

            for(size_t j = 0; j < _types.m_ullCount; ++j)
            {
                const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _types.m_aTypes[j];

                if(_trackKeeper->GetTypehandle().m_ID == _type.m_ID)
                {
                    RuntimeSerializer::Serializer _serializer;

                    _serializer.SetLoading(false);

                    RuntimeReflection::__duckvil_resource_function_t _serializeFunctionHandle = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(_type, "Serialize");
                    RuntimeReflection::__function<void(HotObject::*)(RuntimeSerializer::ISerializer*)>* _func =
                        RuntimeReflection::get_function_callback<HotObject, RuntimeSerializer::ISerializer*>(g_duckvilFrontendReflectionContext.m_pReflectionData, _type, _serializeFunctionHandle);

                    _serializer.Serialize(_trackKeeper->GetObject(), _func);

                    _serializer.SetLoading(true);

                    void* _oldObject = _trackKeeper->GetObject();
                    void* _newObject = RuntimeReflection::create(m_objectsHeap, g_duckvilFrontendReflectionContext.m_pReflectionData, _type);

                    _serializer.Serialize(_newObject, _func);

                    HotReloadedEvent _swapEvent = {};

                    _swapEvent.m_stage = HotReloadedEvent::stage_after_swap;
                    _swapEvent.m_pObject = _newObject;
                    _swapEvent._typeHandle = _trackKeeper->GetTypehandle();
                    _swapEvent.m_pTrackKeeper = _trackKeeper;
                    _swapEvent.m_pOldObject = _oldObject;

                    _trackKeeper->SetObject(_newObject);

                    m_pEventPool->Broadcast(_swapEvent);

                    m_objectsHeap.Free(_oldObject);
                }
            }
        }

        DUCKVIL_LOG_INFO("Swapping objects finished");
    }

    void RuntimeCompilerSystem::AddHotObject(ITrackKeeper* _pTrackKeeper)
    {
        // RuntimeReflection::__duckvil_resource_function_t _castHotObjectFunctionHandle = RuntimeReflection::get_function_handle<void*>(m_pReflectionData, _pTrackKeeper->GetTypehandle(), "Cast");
        // HotReloader::HotObject* _systemInheritance2 = (HotReloader::HotObject*)RuntimeReflection::invoke_static_result<void*, void*>(m_pReflectionData, _pTrackKeeper->GetTypehandle(), _castHotObjectFunctionHandle, _pTrackKeeper->GetObject());

        // _systemInheritance2->m_ullHotObjectID++;

        m_aHotObjects.Allocate(_pTrackKeeper);
    }

    void RuntimeCompilerSystem::SetObjectsHeap(const Memory::FreeList& _heap)
    {
        m_objectsHeap = _heap;
    }

}}