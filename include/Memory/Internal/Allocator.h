#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    bool allocate(__allocator* _pAllocator, size_t _ullSize);

    __linear_allocator* allocate_linear_allocator(__allocator* _pAllocator, size_t _ullSize);
    __fixed_stack_allocator* allocate_fixed_stack_allocator(__allocator* _pAllocator, size_t _ullSize, size_t _ullTypeSize);
    __stack_allocator* allocate_stack_allocator(__allocator* _pAllocator, size_t _ullSize);

}}