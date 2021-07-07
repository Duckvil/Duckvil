#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_array_allocate(fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    void* impl_fixed_array_allocate_size(fixed_array_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_fixed_array_at(fixed_array_allocator* _pAllocator, std::size_t _ullIndex);
    void* impl_fixed_array_begin(fixed_array_allocator* _pAllocator);
    void* impl_fixed_array_back(fixed_array_allocator* _pAllocator);
    std::size_t impl_fixed_array_size(fixed_array_allocator* _pAllocator);
    bool impl_fixed_array_empty(fixed_array_allocator* _pAllocator);
    bool impl_fixed_array_full(fixed_array_allocator* _pAllocator);

    void impl_fixed_array_clear(fixed_array_allocator* _pAllocator);

}}