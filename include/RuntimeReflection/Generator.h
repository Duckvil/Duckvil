#pragma once

#include "Utils/Macro.h"

#include "Memory/Memory.h"

#include "Parser/AST.h"

#include <stack>
#include <queue>
#include <fstream>

#define DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX 256

namespace Duckvil { namespace RuntimeReflection {

    struct __generator_namespace
    {
        char m_sName[32];
        std::vector<Parser::__ast_meta> m_aMeta;
    };

    struct __generator_data
    {
    // TODO: Change to Duckvil vector
        std::deque<__generator_namespace> m_aNamespaces;
        // const Parser::__ast_entity_structure* m_pCurrent = 0;
        bool m_bWasNamespaces = false;
        char m_sInclude[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX] = { 0 };
        uint32_t m_uiRecorderIndex = 0;
    };

    struct __generator_ftable
    {
        __generator_data* (*init)(Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator);
        void (*generate)(__generator_data* _pData, const char _sSourcePath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const char _sHeaderPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast, void (*_fnGenerate)(std::ofstream& _hFile, std::ofstream& _sFile, void* _pUserData), void* _pUserData);
    };

    typedef std::vector<std::pair<uint32_t, std::vector<std::string>>> GeneratedVector;

}}

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init();