#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    void* fixed_array_allocate(__fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* fixed_array_allocate(__fixed_array_allocator* _pAllocator, const char* _pData, std::size_t _ullLength);

    void* fixed_array_at(__fixed_array_allocator* _pAllocator, std::size_t _ullIndex);
    void* fixed_array_begin(__fixed_array_allocator* _pAllocator);
    void* fixed_array_back(__fixed_array_allocator* _pAllocator);
    std::size_t fixed_array_size(__fixed_array_allocator* _pAllocator);
    bool fixed_array_empty(__fixed_array_allocator* _pAllocator);
    bool fixed_array_full(__fixed_array_allocator* _pAllocator);

    void fixed_array_clear(__fixed_array_allocator* _pAllocator);

}}