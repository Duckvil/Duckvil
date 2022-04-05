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

#include "Thread/ThreadPool.h"

#include "Editor/Widget.h"

namespace Duckvil { namespace HotReloader {

    // struct hot_object
    // {
    //     void** m_pObject;
    //     RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    // };

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem)
    class RuntimeCompilerSystem : public ISystem, public Editor::Widget
    {
    public:
        struct user_data
        {
            RuntimeCompilerSystem* m_pRuntimeCompiler;
            std::filesystem::path m_file;
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

        static void Action(const std::filesystem::path& _file, FileWatcher::FileStatus _status, void* _pUserData)
        {
            user_data* _userData = (user_data*)_pUserData;

            if(_status == FileWatcher::FileStatus::FileStatus_Modified)
            {
                printf("Modified: %s\n", _file.string().c_str());

                if(_file.extension() == ".cpp")
                {
                    // Thread::order_task(_userData->m_pRuntimeCompiler->m_pThread, _userData->m_pRuntimeCompiler->m_pThreadData, [](RuntimeCompilerSystem* _pData, const std::filesystem::path& _file)
                    // {
                    //     _pData->Compile(_file.string());
                    // }, _userData->m_pRuntimeCompiler, _file);

                    _userData->m_file = _file;

                    _userData->m_pRuntimeCompiler->m_pThread->m_fnOrderTask(_userData->m_pRuntimeCompiler->m_pThreadData, Utils::lambda([_userData](void*)
                    {
                        _userData->m_pRuntimeCompiler->Compile(_userData->m_file.string(), true);
                    }));
                }
            }
        }

    public:
        RuntimeCompilerSystem(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEventPool, Event::Pool<Event::mode::immediate>* _pRuntimeReflectionEventPool);
        ~RuntimeCompilerSystem();

        // Memory::Vector<ITrackKeeper*> m_aHotObjects;
        Memory::Vector<RuntimeCompilerSystem::hot_object> m_aHotObjects;

        Memory::Vector<RuntimeCompilerSystem::reflection_module> m_aModules;

        std::filesystem::path m_path;

        bool Init();
        bool Init(const std::filesystem::path& _sDirectoryToWatch);
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void Compile(const std::filesystem::path& _CWD, const std::string& _sFile, void (*_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&), bool _bGenerateReflection = true, const RuntimeCompiler::Options& _compileOptions = { });
        void Compile(const std::string& _sFile, bool _bGenerateReflection = true, const RuntimeCompiler::Options& _compileOptions = { });

        void Swap(RuntimeCompilerSystem::hot_object* _pHotObject, const RuntimeReflection::__duckvil_resource_type_t& _typeHandle);

        void AddHotObject(const RuntimeCompilerSystem::hot_object& _hotObject);

        void SetObjectsHeap(const Memory::FreeList& _heap);
        void SetModules(Memory::Vector<PlugNPlay::__module_information>* _aLoaded);
        void SetReflectedTypes(Memory::ThreadsafeVector<duckvil_recorderd_types>* _aReflected);

        void OnEvent(const RuntimeReflection::TrackedObjectCreatedEvent& _event);
    };

}}