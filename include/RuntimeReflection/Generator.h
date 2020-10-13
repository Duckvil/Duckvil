#pragma once

#include "Utils/Macro.h"

#include "Memory/Memory.h"

#include "Parser/AST.h"

#include <stack>
#include <queue>

#define DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX 256

namespace Duckvil { namespace RuntimeReflection {

    struct __generator_namespace
    {
        char m_sName[32];
        std::vector<Parser::__ast_meta> m_aMeta;
    };

    struct __generator_data
    {
        // Memory::Vector<const char*> m_aNamespaces;
    // TODO: Change to Duckvil vector
        std::queue<__generator_namespace> m_aNamespaces;
        const Parser::__ast_entity_structure* m_pCurrent;
        bool m_bWasNamespaces;
    };

    struct __generator_ftable
    {
        __generator_data* (*init)(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator);
        void (*generate)(__generator_data* _pData, const char _sPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast);
    };

}}

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);