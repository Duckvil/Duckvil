#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* linear_allocate(__linear_allocator& _allocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* linear_allocate(__linear_allocator& _allocator, const char* _pData);
    void linear_clear(__linear_allocator& _allocator);

    void* linear_allocate(__linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* linear_allocate(__linear_allocator* _pAllocator, const char* _pData);
    void linear_clear(__linear_allocator* _pAllocator);

}}