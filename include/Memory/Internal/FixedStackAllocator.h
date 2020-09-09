#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const char* _pData);

    void* fixed_stack_allocator_top(__fixed_stack_allocator* _pAllocator);
    void fixed_stack_allocator_pop(__fixed_stack_allocator* _pAllocator);

    void fixed_stack_clear(__fixed_stack_allocator* _pAllocator);

}}