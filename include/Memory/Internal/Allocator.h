#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    bool allocate(__linear_allocator* _pAllocator, std::size_t _ullSize);

    __linear_allocator* allocate_linear_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize);
    __fixed_stack_allocator* allocate_fixed_stack_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __stack_allocator* allocate_stack_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize);
    __queue_allocator* allocate_queue_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize);
    __fixed_array_allocator* allocate_fixed_array_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __free_list_allocator* allocate_free_list_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize);
    __fixed_vector_allocator* allocate_fixed_vector_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

// TODO: Implement rest of allocation types

    __fixed_queue_allocator* allocate_fixed_queue_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

}}