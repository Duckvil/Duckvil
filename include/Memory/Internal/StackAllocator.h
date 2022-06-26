#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* impl_stack_allocate(stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    void* impl_stack_allocate(stack_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_stack_allocator_top(stack_allocator* _pAllocator);
    void impl_stack_allocator_pop(stack_allocator* _pAllocator);
    bool impl_stack_allocator_empty(stack_allocator* _pAllocator);
    bool impl_stack_allocator_full(stack_allocator* _pAllocator);
    std::size_t impl_stack_allocator_size(stack_allocator* _pAllocator);
    std::size_t impl_stack_allocator_capacity(stack_allocator* _pAllocator);

    void impl_stack_clear(stack_allocator* _pAllocator);

}}