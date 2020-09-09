#include "Memory/Internal/FixedQueueAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    void* fixed_queue_allocate(__fixed_queue_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory;

        if(_pAllocator->m_ullTail == g_ullMax)
        {
            _pAllocator->m_ullTail = 0;

            uint8_t _padding = 0;
            _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

            memcpy(_memory, _pData, _ullSize);
        }
        else if(_pAllocator->m_ullTail == 0 && _pAllocator->used == _pAllocator->capacity)
        {
            _memory = 0;
        }
        else if(_pAllocator->used == _pAllocator->capacity && _pAllocator->m_ullTail != 0)
        {
            _pAllocator->used = 0;

            uint8_t _padding = 0;
            _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

            memcpy(_memory, _pData, _ullSize);
        }
        else
        {
            uint8_t _padding = 0;
            _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

            memcpy(_memory, _pData, _ullSize);

            _pAllocator->used += _pAllocator->m_ullBlockSize + _padding;
        }

        return _memory;
    }

    const char* fixed_queue_allocate(__fixed_queue_allocator* _pAllocator, const char* _pData)
    {
        size_t _len = strlen(_pData);

        // if(_pAllocator->capacity < _pAllocator->used + _len)
        // {
        //     return 0;
        // }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _len);

        _pAllocator->used += _len + 1;

        return (const char*)_memory;
    }

    void* fixed_queue_begin(__fixed_queue_allocator* _pAllocator)
    {
        // void* _memory = (void*)(_pAllocator->memory + _pAllocator->m_ullTail);

        // _pAllocator->m_ullTail += _pAllocator->m_ullBlockSize;

        void* _memory;

        if(_pAllocator->m_ullTail == g_ullMax)
        {
            _memory = 0;
        }

        _memory = (void*)(_pAllocator->memory + _pAllocator->m_ullTail);

        if(_pAllocator->m_ullTail == _pAllocator->used)
        {
            _pAllocator->m_ullTail = g_ullMax;
            _pAllocator->used = g_ullMax;
        }
        else if(_pAllocator->m_ullTail == _pAllocator->capacity - _pAllocator->m_ullBlockSize)
        {
            _pAllocator->m_ullTail = 0;
        }
        else
        {
            _pAllocator->m_ullTail += _pAllocator->m_ullBlockSize;
        }

        return _memory;
    }

    void fixed_queue_clear(__fixed_queue_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}