#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* impl_queue_allocate(queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    void* impl_queue_allocate(queue_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_queue_begin(queue_allocator* _pAllocator);
    void impl_queue_pop(queue_allocator* _pAllocator);
    bool impl_queue_empty(queue_allocator* _pAllocator);
    bool impl_queue_full(queue_allocator* _pAllocator);
    void impl_queue_resize(ftable* _pInterface, free_list_allocator* _pParentAllocator, queue_allocator** _pAllocator, std::size_t _ullNewSize);
    std::size_t impl_queue_size(queue_allocator* _pAllocator);
    std::size_t impl_queue_capacity(queue_allocator* _pAllocator);

    void impl_queue_clear(queue_allocator* _pAllocator);

}}