#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    struct __queue_node
    {
        std::size_t m_ullBlockSize;
        uint8_t m_ucPadding;
    };

    static const std::size_t g_ullQueueNodeSize = sizeof(__queue_node);

    void* queue_allocate(__queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* queue_allocate(__queue_allocator* _pAllocator, const char* _pData, std::size_t _ullLength);

    void* queue_begin(__queue_allocator* _pAllocator);
    void queue_pop(__queue_allocator* _pAllocator);
    bool queue_empty(__queue_allocator* _pAllocator);
    bool queue_full(__queue_allocator* _pAllocator);

    void queue_clear(__queue_allocator* _pAllocator);

}}