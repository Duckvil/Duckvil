#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    bool impl_allocate(__linear_allocator* _pAllocator, std::size_t _ullSize);

    __linear_allocator* impl_linear_allocate_linear_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize);
    __fixed_stack_allocator* impl_linear_allocate_fixed_stack_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __fixed_array_allocator* impl_linear_allocate_fixed_array_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __free_list_allocator* impl_linear_allocate_free_list_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize);
    __fixed_vector_allocator* impl_linear_allocate_fixed_vector_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

// TODO: Implement rest of allocation types

    __fixed_queue_allocator* impl_free_list_allocate_fixed_queue_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __free_list_allocator* impl_free_list_allocate_free_list_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize);
    __fixed_vector_allocator* impl_free_list_allocate_fixed_vector_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

}}