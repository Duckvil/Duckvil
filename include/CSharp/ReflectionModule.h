#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Parser/AST.h"

#include "Engine/ReflectionFlags.h"

#include "Memory/FreeList.h"
#include "Utils/Function.h"

namespace Duckvil { namespace CSharp {

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule)
    class ReflectionModule
    {
    public:
        struct CSharpCall
        {
            std::string m_sName;
            std::vector<std::string> m_aArgs;
            std::string m_sReturnType;
        };

        struct InternalCall
        {
            std::string m_sName;
            std::vector<Parser::__ast_entity_argument> m_aArgs;
        };

        struct Variable
        {
            std::string m_sName;
            std::string m_sType;
        };

        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            std::vector<ReflectionModule::InternalCall> m_aInternalCalls;
            std::vector<ReflectionModule::CSharpCall> m_aCSharpCalls;
            std::vector<ReflectionModule::Variable> m_aVariables;
            std::vector<ReflectionModule::Context*> m_aChildren;
            ReflectionModule::Context* m_pParent;
            std::vector<std::string> m_aNamespaces;
            std::string m_sClassName;
        };

        const char* m_sReflectionModuleName = "CSHARP";

    private:

    public:
        ReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData);
        ~ReflectionModule();

        RuntimeReflection::__data* m_pRuntimeReflectionData;
        RuntimeReflection::__ftable* m_pRuntimeReflection;
        Memory::FreeList m_heap;

        std::filesystem::path m_sFile;

        std::vector<ReflectionModule::Context*> m_aContexts;
        ReflectionModule::Context* m_pCurrentContext;

        void ProcessAST(Parser::__ast* _ast);
        void GenerateCustom(std::ofstream& _hFile, std::ofstream& _sFile, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated);
        void Clear();
    };

}}
