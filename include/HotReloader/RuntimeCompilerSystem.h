#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"

#include "Memory/FreeList.h"

#include "HotReloader/FileWatcher.h"
#include "HotReloader/HotObject.h"
#include "HotReloader/TrackKeeper.h"

#include "HotReloader/Events/HotReloadEvent.h"

#include "Process/Process.h"

#include "RuntimeReflection/Generator.h"

#include "Parser/AST.h"
#include "Parser/Lexer.h"

#include "RuntimeCompiler/RuntimeCompiler.h"

#include "Event/ImmediatePool.h"

namespace Duckvil { namespace HotReloader {

    // struct hot_object
    // {
    //     void** m_pObject;
    //     RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    // };

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem)
    class RuntimeCompilerSystem : public ISystem
    {
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

        static void Action(const std::filesystem::path& _file, FileWatcher::FileStatus _status, void* _pUserData)
        {
            user_data* _userData = (user_data*)_pUserData;

            if(_status == FileWatcher::FileStatus::FileStatus_Modified)
            {
                printf("Modified: %s\n", _file.string().c_str());

                if(_file.extension() == ".cpp")
                {
                    _userData->m_pRuntimeCompiler->Compile(_file.string());
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
        void Update();

        void Compile(const std::string& _sFile);

        void AddHotObject(ITrackKeeper* _pTrackKeeper);

        void SetObjectsHeap(const Memory::FreeList& _heap);
    };

}}