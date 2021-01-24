#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_fixed_stack_allocator_top(__fixed_stack_allocator* _pAllocator);
    void impl_fixed_stack_allocator_pop(__fixed_stack_allocator* _pAllocator);
    bool impl_fixed_stack_allocator_empty(__fixed_stack_allocator* _pAllocator);
    bool impl_fixed_stack_allocator_full(__fixed_stack_allocator* _pAllocator);
    std::size_t impl_fixed_stack_allocator_size(__fixed_stack_allocator* _pAllocator);
    std::size_t impl_fixed_stack_allocator_capacity(__fixed_stack_allocator* _pAllocator);

    void impl_fixed_stack_clear(__fixed_stack_allocator* _pAllocator);

}}