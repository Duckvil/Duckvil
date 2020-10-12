#pragma once

#include "Utils/Macro.h"

#include "Memory/Memory.h"

#include "Parser/AST.h"

#define DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX 256

namespace Duckvil { namespace RuntimeReflection {

    struct __generator_ftable
    {
        void (*generate)(const char _sPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast);
    };

}}

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);