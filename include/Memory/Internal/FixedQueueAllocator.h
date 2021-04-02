#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_queue_allocate(fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_fixed_queue_begin(fixed_queue_allocator* _pAllocator);
    void impl_fixed_queue_pop(fixed_queue_allocator* _pAllocator);
    bool impl_fixed_queue_empty(fixed_queue_allocator* _pAllocator);
    bool impl_fixed_queue_full(fixed_queue_allocator* _pAllocator);
    std::size_t impl_fixed_queue_size(fixed_queue_allocator* _pAllocator);
    std::size_t impl_fixed_queue_capacity(fixed_queue_allocator* _pAllocator);

    void impl_fixed_queue_clear(fixed_queue_allocator* _pAllocator);

}}