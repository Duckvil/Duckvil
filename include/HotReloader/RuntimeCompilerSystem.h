#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/TrackedObjectCreatedEvent.h"

#include "Memory/FreeList.h"
#include "Memory/ThreadsafeVector.h"

#include "HotReloader/FileWatcher.h"
#include "HotReloader/HotObject.h"
#include "HotReloader/TrackKeeper.h"

#include "HotReloader/Events/BeforeCompileEvent.h"
#include "HotReloader/Events/AfterCompileEvent.h"
#include "HotReloader/Events/SwapEvent.h"

#include "Process/Process.h"

#include "Parser/AST.h"
#include "Parser/Lexer.h"

#include "RuntimeCompiler/RuntimeCompiler.h"

#include "Event/ImmediatePool.h"
#include "Event/BufferedPool.h"

#include "Thread/ThreadPool.h"

#include "Editor/Widget.h"

#include "Utils/RuntimeDependencyInjector.h"

#include "DependencyInjection/ReflectionModule.h"

#include "HotReloader/RuntimeCompilerSystem.generated.h"

#define DUCKVIL_RUNTIME_REFLECTION_PAUSE
#define DUCKVIL_RUNTIME_REFLECTION_RESUME

namespace Duckvil { namespace HotReloader {

    // struct hot_object
    // {
    //     void** m_pObject;
    //     RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    // };

DUCKVIL_RUNTIME_REFLECTION_PAUSE

    namespace Network {

        class HotObjectSync;

    }

DUCKVIL_RUNTIME_REFLECTION_RESUME

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem, Duckvil::ReflectionFlags_AutoInstantiate = false, Duckvil::DependencyInjection::INJECTABLE)
    class RuntimeCompilerSystem : public ISystem, public Editor::Widget
    {
		DUCKVIL_GENERATED_BODY
        friend class Network::HotObjectSync;
    public:
        struct user_data
        {
            RuntimeCompilerSystem* m_pRuntimeCompiler;
        };

        struct reflection_module
        {
            void* m_pObject;
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
            RuntimeReflection::__duckvil_resource_function_t m_generateCustomFunctionHandle;
            RuntimeReflection::__duckvil_resource_function_t m_processAST_FunctionHandle;
            RuntimeReflection::__duckvil_resource_function_t m_clearFunctionHandle;
        };

        struct hot_object
        {
            ITrackKeeper* m_pObject;
            void* m_pSerializeFunction;

            template <typename A>
            hot_object(ITrackKeeper* _pObject, void (A::*_fnSerialize)(RuntimeSerializer::ISerializer*)) :
                m_pObject(_pObject),
                m_pSerializeFunction((void*&)_fnSerialize)
            {

            }

            hot_object(ITrackKeeper* _pObject, void* _pSerializeFunction) :
                m_pObject(_pObject),
                m_pSerializeFunction(_pSerializeFunction)
            {

            }
        };

        static void Action(const std::filesystem::path& _file, FileWatcher::FileStatus _status, void* _pUserData)
        {
            user_data* _userData = (user_data*)_pUserData;

            if(_status == FileWatcher::FileStatus::FileStatus_Modified)
            {
                printf("Modified: %s\n", _file.string().c_str());

                if(_file.extension() == ".cpp")
                {
                    const auto& _typeHandle = RuntimeReflection::get_type<RuntimeCompilerSystem>();
                    const auto& _funcHandle = RuntimeReflection::get_function_handle<
                            const std::string&,
                            const RuntimeCompiler::Options&
                        >(
                            _typeHandle,
                            "CompileT"
                        );

                    RuntimeReflection::invoke_member<
                        const std::string&,
                        const RuntimeCompiler::Options&
                    >(
                        _typeHandle,
                        _funcHandle,
                        _userData->m_pRuntimeCompiler,
                        _file.string(),
                        RuntimeCompiler::Options()
                    );
                }
                else if(_file.extension() == ".h")
                {
                    const auto& _typeHandle = RuntimeReflection::get_type<RuntimeCompilerSystem>();
                    const auto& _funcHandle = RuntimeReflection::get_function_handle<
                            const std::filesystem::path&,
                            const std::filesystem::path&,
                            bool
                        >(
                            _typeHandle,
                            "GenerateReflection"
                        );

                    const auto& _getCWDHandle = RuntimeReflection::get_function_handle(_typeHandle, "GetCWD");
                    const auto& _cwd = RuntimeReflection::invoke_member_result<const std::filesystem::path&>(_typeHandle, _getCWDHandle, _userData->m_pRuntimeCompiler);

                    RuntimeReflection::invoke_member<
                        const std::filesystem::path&,
                        const std::filesystem::path&,
                        bool
                    >(
                        _typeHandle,
                        _funcHandle,
                        _userData->m_pRuntimeCompiler,
                        _cwd,
                        _file,
                        true
                    );
                }
            }
            else if(_status == FileWatcher::FileStatus::FileStatus_Created)
            {
                printf("Created: %s\n", _file.string().c_str());

                if(_file.extension() == ".h")
                {
                    const auto& _typeHandle = RuntimeReflection::get_type<RuntimeCompilerSystem>();
                    const auto& _funcHandle = RuntimeReflection::get_function_handle<
                            const std::filesystem::path&,
                            const std::filesystem::path&,
                            bool
                        >(
                            _typeHandle,
                            "GenerateReflection"
                        );

                    const auto& _getCWDHandle = RuntimeReflection::get_function_handle(_typeHandle, "GetCWD");
                    const auto& _cwd = RuntimeReflection::invoke_member_result<const std::filesystem::path&>(_typeHandle, _getCWDHandle, _userData->m_pRuntimeCompiler);

                    RuntimeReflection::invoke_member<
                        const std::filesystem::path&,
                        const std::filesystem::path&,
                        bool
                    >(
                        _typeHandle,
                        _funcHandle,
                        _userData->m_pRuntimeCompiler,
                        _cwd,
                        "",     // Regenerate whole reflection
                        true
                    );

                    // PlugNPlay::__module _module;
                    // Process::ftable _process;
                    // Process::data _processData;

                    // PlugNPlay::module_init(&_module);

                    // PlugNPlay::__module_information _processModuleInfo("Process");

                    // _module.load(&_processModuleInfo);

                    // void (*_duckvilProcessInit)(Process::ftable* _pFTable);

                    // _module.get(_processModuleInfo, "duckvil_process_init", reinterpret_cast<void**>(&_duckvilProcessInit));

                    // _duckvilProcessInit(&_process);

                    // _process.m_fnInit(_userData->m_pRuntimeCompiler->m_heap.GetMemoryInterface(), _userData->m_pRuntimeCompiler->m_heap.GetAllocator(), &_processData);
                    // _process.m_fnSetup(&_processData);

                    // // if(_status == HotReloader::FileWatcher::FileStatus_Modified)
                    // // {
                    // //     _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _data->m_loadedProject.m_sPath + " -is_absolute -file " + _file + "\n_COMPLETION_TOKEN_\n").c_str());
                    // // }
                    // // else if(_status == HotReloader::FileWatcher::FileStatus_Created)
                    // // {
                    // //     _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _data->m_loadedProject.m_sPath + "\n_COMPLETION_TOKEN_\n").c_str());
                    // // }

                    // _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _data->m_loadedProject.m_sPath + "\n_COMPLETION_TOKEN_\n").c_str());

                    // _process.m_fnWait(&_processData);
                    // _process.m_fnStop(&_processData);

                    // if(_process.m_fnTerminate(&_processData))
                    // {
                    //     _process.m_fnCleanup(_userData->m_pRuntimeCompiler->m_heap.GetMemoryInterface(), _userData->m_pRuntimeCompiler->m_heap.GetAllocator(), &_processData);
                    // }
                }
                else if(_file.extension() == ".cpp")
                {
                    const auto& _typeHandle = RuntimeReflection::get_type<RuntimeCompilerSystem>();
                    const auto& _funcHandle = RuntimeReflection::get_function_handle<
                        const std::string&,
                        const RuntimeCompiler::Options&
                    >(
                        _typeHandle,
                        "CompileT"
                    );

                    RuntimeReflection::invoke_member<
                        const std::string&,
                        const RuntimeCompiler::Options&
                    >(
                        _typeHandle,
                        _funcHandle,
                        _userData->m_pRuntimeCompiler,
                        _file.string(),
                        RuntimeCompiler::Options()
                    );
                }
            }
        }

    private:
        FileWatcher* m_pFileWatcher;
        user_data m_userData;

        Memory::FreeList m_heap;

        Memory::FreeList m_objectsHeap;

        RuntimeReflection::__duckvil_resource_type_t m_compilerTypeHandle;

        std::string m_sModuleName;

        RuntimeCompiler::Compiler* m_pCompiler;

        Event::Pool<Event::mode::immediate>* m_pEventPool;
        Event::Pool<Event::mode::immediate>* m_pRuntimeReflectionEventPool;

    // RuntimeReflection generator
        RuntimeReflection::__generator_ftable* m_pReflectionGenerator;
        Parser::__ast_ftable* m_pAST_FTable;
        Parser::__lexer_ftable* m_pLexerFTable;

        Thread::pool_ftable* m_pThread;
        Thread::pool_data* m_pThreadData;

        Memory::Vector<PlugNPlay::__module_information>* m_aLoadedModules;
        Memory::ThreadsafeVector<duckvil_recorderd_types>* m_aReflectedTypes;

        RuntimeReflection::__function<bool(RuntimeCompiler::Compiler::*)()>*                        m_fnInternalCompilerSetup;
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>*      m_fnInternalCompilerAddFlag;
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(RuntimeCompiler::Flag)>*   m_fnInternalCompilerAddFlag2;
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>*      m_fnInternalCompilerAddDefine;
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>*      m_fnInternalCompilerAddInclude;
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>*      m_fnInternalCompilerAddLibraryPath;
        RuntimeReflection::__function<void(RuntimeCompiler::Compiler::*)(const std::string&)>*      m_fnInternalCompilerAddLibrary;

        RuntimeReflection::__function<const std::vector<std::string>&(RuntimeCompiler::Compiler::*)()>* m_fnInternalCompilerGetFlags;
        RuntimeReflection::__function<const std::vector<std::string>&(RuntimeCompiler::Compiler::*)()>* m_fnInternalCompilerGetDefines;
        RuntimeReflection::__function<const std::vector<std::string>&(RuntimeCompiler::Compiler::*)()>* m_fnInternalCompilerGetIncludes;
        RuntimeReflection::__function<const std::vector<std::string>&(RuntimeCompiler::Compiler::*)()>* m_fnInternalCompilerGetLibrariesPaths;
        RuntimeReflection::__function<const std::vector<std::string>&(RuntimeCompiler::Compiler::*)()>* m_fnInternalCompilerGetLibraries;

        Event::Pool<Event::mode::buffered> m_eventPool;

        std::filesystem::path m_CWD;
        bool m_bIsSingleModule;

        void Compile(const std::string& _sFile, void (*_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&), const RuntimeCompiler::Options& _compileOptions = { });

        void HotReload(const Utils::string& _sModuleFilename, void (*_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&));
        void HotReload(const Utils::string& _sModuleFilename);
        void Swap(Memory::Vector<RuntimeCompilerSystem::hot_object>* _pHotObjects, duckvil_recorderd_types& _newTypes);

    public:
        RuntimeCompilerSystem(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEventPool, Event::Pool<Event::mode::immediate>* _pRuntimeReflectionEventPool, FileWatcher::ActionCallback _fnAction, void* _pActionData);
        RuntimeCompilerSystem(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEventPool, Event::Pool<Event::mode::immediate>* _pRuntimeReflectionEventPool);
        ~RuntimeCompilerSystem();

        // Memory::Vector<ITrackKeeper*> m_aHotObjects;
        Memory::Vector<RuntimeCompilerSystem::hot_object> m_aHotObjects;

        Memory::Vector<RuntimeCompilerSystem::reflection_module> m_aModules;

        std::filesystem::path m_path;

        bool Init(bool _bIsSingleModule = false);
        bool Init(const std::vector<std::filesystem::path>& _sDirectoriesToWatch, bool _bIsSingleModule = false);
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void Compile(const std::string& _sFile, const RuntimeCompiler::Options& _compileOptions = { });

        // void CompileT(const std::filesystem::path& _CWD, const std::string& _sFile, void (*_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&), const RuntimeCompiler::Options& _compileOptions = { });
        void CompileT(const std::string& _sFile, const RuntimeCompiler::Options& _compileOptions = { });

        void GenerateReflection(const std::filesystem::path& _CWD, const std::filesystem::path& _file = "", bool _bIsAbsolute = false);

        bool Swap(RuntimeCompilerSystem::hot_object* _pHotObject, const RuntimeReflection::__duckvil_resource_type_t& _typeHandle);

        void AddHotObject(const RuntimeCompilerSystem::hot_object& _hotObject);

        void SetObjectsHeap(const Memory::FreeList& _heap);
        void SetModules(Memory::Vector<PlugNPlay::__module_information>* _aLoaded);
        void SetReflectedTypes(Memory::ThreadsafeVector<duckvil_recorderd_types>* _aReflected);
        void SetCWD(const std::filesystem::path& _CWD);
        const std::filesystem::path& GetCWD() const;

        inline RuntimeCompiler::ICompiler* GetCompiler() const { return m_pCompiler; }

        void OnEvent(const RuntimeReflection::TrackedObjectCreatedEvent& _event);
    };

}}