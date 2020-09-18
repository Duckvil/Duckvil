#include "Memory/Internal/QueueAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* queue_allocate(__queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        __queue_node* _current_memory = (__queue_node*)(_pAllocator->memory + _pAllocator->m_ullTail);
        std::size_t _block_size = _current_memory->m_ullBlockSize;
        uint8_t _padding = _current_memory->m_ucPadding;
        void* _memory = 0;

        if(_pAllocator->used + g_ullQueueNodeSize >= _pAllocator->capacity)
        {
            return _memory;
        }
        else if(_pAllocator->m_ullHead + g_ullQueueNodeSize + _block_size + _padding >= _pAllocator->capacity && _pAllocator->m_ullTail > 0)
        {
            _pAllocator->m_ullHead = 0;
        }

        _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->m_ullHead, _ucAlignment, _padding);

        __queue_node* _node = (__queue_node*)((uint8_t*)_memory);

        _node->m_ullBlockSize = _ullSize;
        _node->m_ucPadding = _padding;

        memcpy((uint8_t*)_memory + g_ullQueueNodeSize, _pData, _ullSize);

        _pAllocator->m_ullHead += _ullSize + g_ullQueueNodeSize + _padding;
        _pAllocator->used += _ullSize + g_ullQueueNodeSize + _padding;

        return _memory;
    }

    const char* queue_allocate(__queue_allocator* _pAllocator, const char* _pData)
    {
        void* _memory = 0;

        return (const char*)_memory;
    }

    void* queue_begin(__queue_allocator* _pAllocator)
    {
        __queue_node* _current_memory = (__queue_node*)(_pAllocator->memory + _pAllocator->m_ullTail);

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->m_ullTail + g_ullQueueNodeSize + _current_memory->m_ucPadding);

        return _memory;
    }

    void queue_pop(__queue_allocator* _pAllocator)
    {
        if(_pAllocator->used == 0)
        {
            // Underflow

            return;
        }

        __queue_node* _current_memory = (__queue_node*)(_pAllocator->memory + _pAllocator->m_ullTail);
        std::size_t _block_size = _current_memory->m_ullBlockSize;
        uint8_t _padding = _current_memory->m_ucPadding;

        memset(_current_memory, 0, _block_size + g_ullQueueNodeSize + _padding);

        if(_pAllocator->m_ullTail + g_ullQueueNodeSize + _block_size + _padding >= _pAllocator->capacity && _pAllocator->m_ullHead > 0)
        {
            _pAllocator->m_ullTail -= _pAllocator->m_ullHead;
            _pAllocator->m_ullTail -= _block_size;
            _pAllocator->m_ullTail -= g_ullQueueNodeSize;
            _pAllocator->m_ullTail -= _padding;
        }
        else if(_pAllocator->m_ullTail + _pAllocator->m_ullTail + g_ullQueueNodeSize > _pAllocator->capacity)
        {
            _pAllocator->m_ullTail = 0;
        }
        else
        {
            _pAllocator->m_ullTail += _block_size + _padding + g_ullQueueNodeSize;
        }

        _pAllocator->used -= _block_size;
        _pAllocator->used -= g_ullQueueNodeSize;
        _pAllocator->used -= _padding;
    }

    bool queue_empty(__queue_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool queue_full(__queue_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    void queue_clear(__queue_allocator* _pAllocator)
    {

    }

}}