#include "Memory/Internal/FixedQueueAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* impl_fixed_queue_allocate(fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullUsed >= _pAllocator->m_ullCapacity)
        {
            return _memory;
        }
        else if(_pAllocator->m_ullHead + _pAllocator->m_ullBlockSize >= _pAllocator->m_ullCapacity && _pAllocator->m_ullTail == _pAllocator->m_ullHead)
        {
            _pAllocator->m_ullTail = 0;
            _pAllocator->m_ullHead = 0;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer((uint8_t*)_pAllocator + sizeof(fixed_queue_allocator) + _pAllocator->m_ullHead, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullHead += _pAllocator->m_ullBlockSize + _padding;
        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_queue_begin(fixed_queue_allocator* _pAllocator)
    {
        void* _memory = (void*)((uint8_t*)_pAllocator + sizeof(fixed_queue_allocator) + _pAllocator->m_ullTail);

        return _memory;
    }

    void impl_fixed_queue_pop(fixed_queue_allocator* _pAllocator)
    {
        if(_pAllocator->m_ullUsed == 0)
        {
            // Underflow

            return;
        }

    // Maybe some macro for clearing memory
        memset((uint8_t*)_pAllocator + sizeof(fixed_queue_allocator) + _pAllocator->m_ullTail, 0, _pAllocator->m_ullBlockSize);

        _pAllocator->m_ullTail += _pAllocator->m_ullBlockSize;

        _pAllocator->m_ullUsed -= _pAllocator->m_ullBlockSize;
    }

    bool impl_fixed_queue_empty(fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_queue_full(fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_queue_clear(fixed_queue_allocator* _pAllocator)
    {
        memset((uint8_t*)_pAllocator + sizeof(fixed_queue_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullHead = 0;
        _pAllocator->m_ullUsed = 0;
        _pAllocator->m_ullTail = 0;
    }

    std::size_t impl_fixed_queue_size(fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_fixed_queue_capacity(fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

}}