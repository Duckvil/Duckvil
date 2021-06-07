#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/TrackedObjectCreatedEvent.h"

#include "Memory/FreeList.h"

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

    private:
        FileWatcher* m_pFileWatcher;
        user_data m_userData;

        Memory::FreeList m_heap;

        Memory::FreeList m_objectsHeap;

        RuntimeReflection::__duckvil_resource_type_t m_compilerTypeHandle;

        std::string m_sModuleName;

        RuntimeCompiler::Compiler* m_pCompiler;

        Event::Pool<Event::mode::immediate>* m_pEventPool;

    // RuntimeReflection generator
        RuntimeReflection::__generator_ftable* m_pReflectionGenerator;
        Parser::__ast_ftable* m_pAST_FTable;
        Parser::__lexer_ftable* m_pLexerFTable;

        Thread::pool_ftable* m_pThread;
        Thread::pool_data* m_pThreadData;

        Memory::Vector<PlugNPlay::__module_information>* m_aLoadedModules;
        Memory::Vector<duckvil_recorderd_types>* m_aReflectedTypes;

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
                        _userData->m_pRuntimeCompiler->Compile(_userData->m_file.string());
                    }));
                }
            }
        }

    public:
        RuntimeCompilerSystem(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEventPool);
        ~RuntimeCompilerSystem();

        // duckvil_recorderd_types* m_aRecordedTypes;
        // size_t m_ullRecordedTypesCount;
        Memory::Vector<duckvil_recorderd_types> m_aRecordedTypes;

        Memory::Vector<ITrackKeeper*> m_aHotObjects;

        Memory::Vector<RuntimeCompilerSystem::reflection_module> m_aModules;

        bool Init();
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void Compile(const std::string& _sFile);

        void AddHotObject(ITrackKeeper* _pTrackKeeper);

        void SetObjectsHeap(const Memory::FreeList& _heap);
        void SetModules(Memory::Vector<PlugNPlay::__module_information>* _aLoaded);
        void SetReflectedTypes(Memory::Vector<duckvil_recorderd_types>* _aReflected);

        void OnEvent(const RuntimeReflection::TrackedObjectCreatedEvent& _event);
    };

}}