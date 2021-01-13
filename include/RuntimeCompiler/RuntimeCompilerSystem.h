#pragma once

#include "Duckvil/ISystem.h"
#include "Duckvil/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"

#include "Memory/FreeList.h"

#include "RuntimeCompiler/FileWatcher.h"
#include "RuntimeCompiler/HotObject.h"

#include "Process/Process.h"

#include "RuntimeReflection/Generator.h"

#include "Parser/AST.h"
#include "Parser/Lexer.h"

namespace Duckvil { namespace RuntimeCompiler {

    struct hot_object
    {
        void** m_pObject;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    };

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_EngineSystem)
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
        Process::ftable m_processFTable;
        Process::data m_processData;

        Memory::FreeList m_heap;
        RuntimeReflection::__recorder_ftable* m_pReflectionRecorderFTable;
        RuntimeReflection::__ftable* m_pReflectionFTable;

        std::vector<std::string> m_aFlags;
        std::vector<std::string> m_aDefines;
        std::vector<std::string> m_aIncludes;
        std::vector<std::string> m_aLibrariesPaths;
        std::vector<std::string> m_aLibraries;

        std::string m_sModuleName;

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

                // _userData->m_pProcessFTable->m_fnWrite(_userData->m_pProcessData, "cl\n_COMPLETION_TOKEN_\n");

                if(_file.extension() == ".cpp")
                {
                    _userData->m_pRuntimeCompiler->Compile(_file.string());
                }
            }
        }

    public:
        RuntimeCompilerSystem(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData, RuntimeReflection::__recorder_ftable* _pReflectionRecorderFTable, RuntimeReflection::__ftable* _pReflectionFTable);
        ~RuntimeCompilerSystem();

        RuntimeReflection::__data* m_pReflectionData;

        duckvil_recorderd_types* m_aRecordedTypes;
        size_t m_ullRecordedTypesCount;

        Memory::Vector<hot_object> m_aHotObjects;

        Memory::Vector<RuntimeCompilerSystem::reflection_module> m_aModules;

        bool Init();
        void Update();

        void Compile(const std::string& _sFile);

        void AddHotObject(void** _pHotObject, RuntimeReflection::__duckvil_resource_type_t _typeHandle);
    };

}}