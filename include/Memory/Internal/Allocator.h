#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    bool allocate(__allocator* _pAllocator, std::size_t _ullSize);

    __linear_allocator* allocate_linear_allocator(__allocator* _pAllocator, std::size_t _ullSize);
    __fixed_stack_allocator* allocate_fixed_stack_allocator(__allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __stack_allocator* allocate_stack_allocator(__allocator* _pAllocator, std::size_t _ullSize);
    __fixed_queue_allocator* allocate_fixed_queue_allocator(__allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
    __queue_allocator* allocate_queue_allocator(__allocator* _pAllocator, std::size_t _ullSize);

}}