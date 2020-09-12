#include "Memory/Internal/FixedQueueAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* fixed_queue_allocate(__fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_pAllocator->used >= _pAllocator->capacity)
        {
            return 0;
        }
        else if(_pAllocator->m_ullHead >= _pAllocator->capacity && _pAllocator->m_ullTail > 0)
        {
            _pAllocator->m_ullHead = _pAllocator->m_ullTail - _pAllocator->m_ullBlockSize;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->m_ullHead, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullHead += _pAllocator->m_ullBlockSize + _padding;
        _pAllocator->used += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    const char* fixed_queue_allocate(__fixed_queue_allocator* _pAllocator, const char* _pData)
    {
        std::size_t _len = strlen(_pData);

        if(_pAllocator->used >= _pAllocator->capacity)
        {
            return 0;
        }
        else if(_pAllocator->m_ullHead >= _pAllocator->capacity && _pAllocator->m_ullTail > 0)
        {
            _pAllocator->m_ullHead = _pAllocator->m_ullTail - _pAllocator->m_ullBlockSize;
        }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->m_ullHead);

        memcpy(_memory, _pData, _len);

        _pAllocator->m_ullHead += _len + 1;

        return (const char*)_memory;
    }

    void* fixed_queue_begin(__fixed_queue_allocator* _pAllocator)
    {
        void* _memory = (void*)(_pAllocator->memory + _pAllocator->m_ullTail);

        return _memory;
    }

    void fixed_queue_pop(__fixed_queue_allocator* _pAllocator)
    {
        if(_pAllocator->used == 0)
        {
            // Underflow

            return;
        }

    // Maybe some macro for clearing memory
        memset(_pAllocator->memory + _pAllocator->m_ullTail, 0, _pAllocator->m_ullBlockSize);

        if(_pAllocator->m_ullTail >= _pAllocator->capacity - _pAllocator->m_ullBlockSize && _pAllocator->m_ullHead > 0)
        {
            _pAllocator->m_ullTail = _pAllocator->m_ullHead - _pAllocator->m_ullBlockSize;
        }
        else
        {
            _pAllocator->m_ullTail += _pAllocator->m_ullBlockSize;
        }

        _pAllocator->used -= _pAllocator->m_ullBlockSize;
    }

    bool fixed_queue_empty(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_queue_full(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    void fixed_queue_clear(__fixed_queue_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->m_ullHead = 0;
        _pAllocator->used = 0;
        _pAllocator->m_ullTail = 0;
    }

}}