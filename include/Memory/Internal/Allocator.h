#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    bool impl_allocate(linear_allocator** _pAllocator, std::size_t _ullSize);

    linear_allocator* impl_linear_allocate_linear_allocator(linear_allocator* _pAllocator, std::size_t _ullSize);
    fixed_stack_allocator* impl_linear_allocate_fixed_stack_allocator(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    fixed_array_allocator* impl_linear_allocate_fixed_array_allocator(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    free_list_allocator* impl_linear_allocate_free_list_allocator(linear_allocator* _pAllocator, std::size_t _ullSize);
    fixed_vector_allocator* impl_linear_allocate_fixed_vector_allocator(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

// TODO: Implement rest of allocation types

    fixed_queue_allocator* impl_free_list_allocate_fixed_queue_allocator(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    free_list_allocator* impl_free_list_allocate_free_list_allocator(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);
    fixed_vector_allocator* impl_free_list_allocate_fixed_vector_allocator(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    fixed_stack_allocator* impl_free_list_allocate_fixed_stack_allocator(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    fixed_array_allocator* impl_free_list_allocate_fixed_array_allocator(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

}}