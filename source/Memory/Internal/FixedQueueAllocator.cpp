#include "Memory/Internal/FixedQueueAllocator.h"

#include <cstring>

#include "Memory/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_queue_allocate(fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_fixed_queue_allocate_size(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_fixed_queue_allocate_size(fixed_queue_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullUsed >= _pAllocator->m_ullCapacity)
        {
            return _memory;
        }
        else if(_pAllocator->m_ullHead >= _pAllocator->m_ullCapacity)
        {
            _pAllocator->m_ullHead = 0;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_queue_allocator) + _pAllocator->m_ullHead, _ucAlignment, _padding);

        _pAllocator->m_ullHead += _pAllocator->m_ullBlockSize + _padding;
        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_queue_begin(fixed_queue_allocator* _pAllocator)
    {
        void* _memory = static_cast<void*>(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_queue_allocator) + _pAllocator->m_ullTail);

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
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_queue_allocator) + _pAllocator->m_ullTail, 0, _pAllocator->m_ullBlockSize);

        _pAllocator->m_ullTail += _pAllocator->m_ullBlockSize;

        if(_pAllocator->m_ullTail >= _pAllocator->m_ullCapacity)
        {
            _pAllocator->m_ullTail = 0;
        }

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
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_queue_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullHead = 0;
        _pAllocator->m_ullUsed = 0;
        _pAllocator->m_ullTail = 0;
    }

    void impl_fixed_queue_resize(ftable* _pInterface, free_list_allocator* _pParentAllocator, fixed_queue_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        if(_pParentAllocator->m_ullCapacity < _ullNewSize + _pParentAllocator->m_ullUsed)
        {
            return;
        }

        fixed_queue_allocator* _allocator = _pInterface->m_fnFreeListAllocateFixedQueueAllocator(_pInterface, _pParentAllocator, _ullNewSize * (*_pAllocator)->m_ullBlockSize, (*_pAllocator)->m_ullBlockSize);

        _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;
        _allocator->m_ullTail = (*_pAllocator)->m_ullTail;
        _allocator->m_ullHead = (*_pAllocator)->m_ullHead;

        memcpy(reinterpret_cast<uint8_t*>(_allocator) + sizeof(fixed_queue_allocator), reinterpret_cast<uint8_t*>(*_pAllocator) + sizeof(fixed_queue_allocator), (*_pAllocator)->m_ullUsed);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif

        fixed_queue_allocator* _ptr = *_pAllocator;

        *_pAllocator = _allocator;

        free_list_free(_pInterface, _pParentAllocator, _ptr);
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