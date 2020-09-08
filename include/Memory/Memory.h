#pragma once

#include <cstdint>

#include "Utils/Macro.h"

namespace Duckvil { namespace Memory {

    struct __memory
    {
        uint8_t* memory = 0;
        size_t capacity = 0;
        size_t used = 0;
    };

    struct IMemory
    {
        typedef bool (*_basic_allocate)(__memory* _pMemory, size_t _ullSize);
        typedef void* (*_linear_allocate)(__memory* _pMemory, const void* _pData, size_t _ullSize);

        _basic_allocate m_fnBasicAllocate;
        _linear_allocate m_fnLinearAllocate;
    };

}}

DUCKVIL_EXPORT Duckvil::Memory::IMemory* duckvil_memory_init();