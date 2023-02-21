#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Parser/AST.h"

#include "Engine/ReflectionFlags.h"

#include "Memory/FreeList.h"
#include "Utils/Function.h"

#include "DependencyInjection/ReflectionFlags.h"

namespace Duckvil { namespace DependencyInjection {

    struct Injectable
    {

    };

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule)
    class ReflectionModule
    {
    public:
        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            std::vector<ReflectionModule::Context*> m_aChildren;
            ReflectionModule::Context* m_pParent;
            std::vector<std::string> m_aNamespaces;
            std::string m_sClassName;
        };

        const char* m_sReflectionModuleName = "DEPENDENCY_INJECTION";

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
        void GenerateCustom(std::ofstream& _hFile, std::ofstream& _sFile, RuntimeReflection::GeneratedVector& _aGenerated);
        void Clear();
    };

}}