#include "Memory/Internal/StackAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    void* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_pAllocator->capacity < _pAllocator->used + _ullSize || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return 0;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->used += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    const char* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const char* _pData)
    {
        size_t _len = strlen(_pData);

        if(_pAllocator->capacity < _pAllocator->used + _len)
        {
            return 0;
        }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _len);

        _pAllocator->used += _len + 1;

        return (const char*)_memory;
    }

    void* fixed_stack_allocator_top(__fixed_stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = _pAllocator->memory + _pAllocator->used;

        return _current_memory - _pAllocator->m_ullBlockSize;
    }

    void fixed_stack_allocator_pop(__fixed_stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = _pAllocator->memory + _pAllocator->used;

        memset(_current_memory - _pAllocator->m_ullBlockSize, 0, _pAllocator->m_ullBlockSize);

        _pAllocator->used -= _pAllocator->m_ullBlockSize;
    }

    void fixed_stack_clear(__fixed_stack_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }



    void* stack_allocate(__stack_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_pAllocator->capacity < _pAllocator->used + _ullSize + sizeof(__stack_node))
        {
            return 0;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        ((__stack_node*)((uint8_t*)_memory + _ullSize))->m_ullBlockSize = _ullSize;

        _pAllocator->used += _ullSize + _padding + sizeof(__stack_node);

        return _memory;
    }

    const char* stack_allocate(__stack_allocator* _pAllocator, const char* _pData)
    {
        size_t _len = strlen(_pData);

        if(_pAllocator->capacity < _pAllocator->used + _len + sizeof(__stack_node))
        {
            return 0;
        }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _len);

        ((__stack_node*)((uint8_t*)_memory + _len + 1))->m_ullBlockSize = _len + 1;

        _pAllocator->used += _len + 1 + sizeof(__stack_node);

        return (const char*)_memory;
    }

    void* stack_allocator_top(__stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = _pAllocator->memory + _pAllocator->used - sizeof(__stack_node);
        __stack_node* _node = (__stack_node*)_current_memory;

        return _current_memory - _node->m_ullBlockSize;
    }

    void stack_allocator_pop(__stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = _pAllocator->memory + _pAllocator->used - sizeof(__stack_node);
        size_t _block_size = ((__stack_node*)_current_memory)->m_ullBlockSize;

        memset(_current_memory - _block_size, 0, _block_size + sizeof(__stack_node));

        _pAllocator->used -= _block_size;
        _pAllocator->used -= sizeof(__stack_node);
    }

    void stack_clear(__stack_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}