#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_stack_allocate(fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    void* impl_fixed_stack_allocate_size(fixed_stack_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_fixed_stack_allocator_top(fixed_stack_allocator* _pAllocator);
    void impl_fixed_stack_allocator_pop(fixed_stack_allocator* _pAllocator);
    bool impl_fixed_stack_allocator_empty(fixed_stack_allocator* _pAllocator);
    bool impl_fixed_stack_allocator_full(fixed_stack_allocator* _pAllocator);
    std::size_t impl_fixed_stack_allocator_size(fixed_stack_allocator* _pAllocator);
    std::size_t impl_fixed_stack_allocator_capacity(fixed_stack_allocator* _pAllocator);

    void impl_fixed_stack_clear(fixed_stack_allocator* _pAllocator);

}}