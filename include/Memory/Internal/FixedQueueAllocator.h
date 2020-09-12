#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* fixed_queue_allocate(__fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* fixed_queue_allocate(__fixed_queue_allocator* _pAllocator, const char* _pData);

    void* fixed_queue_begin(__fixed_queue_allocator* _pAllocator);
    void fixed_queue_pop(__fixed_queue_allocator* _pAllocator);
    bool fixed_queue_empty(__fixed_queue_allocator* _pAllocator);
    bool fixed_queue_full(__fixed_queue_allocator* _pAllocator);

    void fixed_queue_clear(__fixed_queue_allocator* _pAllocator);

}}