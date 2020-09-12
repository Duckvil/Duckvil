#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    struct __stack_node
    {
        std::size_t m_ullBlockSize;
        uint8_t m_ucPadding;
    };

    static const std::size_t g_ullStackNodeSize = sizeof(__stack_node);

    void* stack_allocate(__stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* stack_allocate(__stack_allocator* _pAllocator, const char* _pData);

    void* stack_allocator_top(__stack_allocator* _pAllocator);
    void stack_allocator_pop(__stack_allocator* _pAllocator);
    bool stack_allocator_empty(__stack_allocator* _pAllocator);
    bool stack_allocator_full(__stack_allocator* _pAllocator);

    void stack_clear(__stack_allocator* _pAllocator);

}}