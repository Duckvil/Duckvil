#include "Memory/Internal/StackAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* stack_allocate(__stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->capacity < _pAllocator->used + _ullSize + g_ullStackNodeSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        __stack_node* _node = (__stack_node*)((uint8_t*)_memory + _ullSize);

        _node->m_ullBlockSize = _ullSize;
        _node->m_ucPadding = _padding;

        _pAllocator->used += _ullSize + _padding + g_ullStackNodeSize;

        return _memory;
    }

    const char* stack_allocate(__stack_allocator* _pAllocator, const char* _pData)
    {
        std::size_t _len = strlen(_pData);
        void* _memory = nullptr;

        if(_pAllocator->capacity < _pAllocator->used + _len + g_ullStackNodeSize)
        {
            return (const char*)_memory;
        }

        _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _len);

        __stack_node* _node = (__stack_node*)((uint8_t*)_memory + _len + 1);

        _node->m_ullBlockSize = _len + 1;
        _node->m_ucPadding = 0;

        _pAllocator->used += _len + 1 + g_ullStackNodeSize;

        return (const char*)_memory;
    }

    void* stack_allocator_top(__stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = _pAllocator->memory + _pAllocator->used - g_ullStackNodeSize;
        __stack_node* _node = (__stack_node*)_current_memory;

        return _current_memory - _node->m_ullBlockSize;
    }

    void stack_allocator_pop(__stack_allocator* _pAllocator)
    {
        if(_pAllocator->used == 0)
        {
            return;
        }

        __stack_node* _current_memory = (__stack_node*)(_pAllocator->memory + _pAllocator->used - g_ullStackNodeSize);
        std::size_t _block_size = _current_memory->m_ullBlockSize;
        uint8_t _padding = _current_memory->m_ucPadding;

        memset(_current_memory, 0, _block_size + g_ullStackNodeSize);

        _pAllocator->used -= _block_size;
        _pAllocator->used -= g_ullStackNodeSize;
        _pAllocator->used -= _padding;
    }

    bool stack_allocator_empty(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool stack_allocator_full(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    void stack_clear(__stack_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}