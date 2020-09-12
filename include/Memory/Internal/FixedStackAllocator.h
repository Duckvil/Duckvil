#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const char* _pData);

    void* fixed_stack_allocator_top(__fixed_stack_allocator* _pAllocator);
    void fixed_stack_allocator_pop(__fixed_stack_allocator* _pAllocator);
    bool fixed_stack_allocator_empty(__fixed_stack_allocator* _pAlloctor);
    bool fixed_stack_allocator_full(__fixed_stack_allocator* _pAllocator);

    void fixed_stack_clear(__fixed_stack_allocator* _pAllocator);

}}