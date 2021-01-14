#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_array_allocate(__fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_fixed_array_at(__fixed_array_allocator* _pAllocator, std::size_t _ullIndex);
    void* impl_fixed_array_begin(__fixed_array_allocator* _pAllocator);
    void* impl_fixed_array_back(__fixed_array_allocator* _pAllocator);
    std::size_t impl_fixed_array_size(__fixed_array_allocator* _pAllocator);
    bool impl_fixed_array_empty(__fixed_array_allocator* _pAllocator);
    bool impl_fixed_array_full(__fixed_array_allocator* _pAllocator);

    void impl_fixed_array_clear(__fixed_array_allocator* _pAllocator);

}}