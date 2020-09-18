#include "Memory/Internal/FixedStackAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->capacity < _pAllocator->used + _ullSize || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        // Flawfinder: ignore
        memcpy(_memory, _pData, _ullSize);

        _pAllocator->used += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    const char* fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const char* _pData)
    {
        std::size_t _len = strlen(_pData);
        void* _memory = nullptr;

        if(_pAllocator->capacity < _pAllocator->used + _len)
        {
            return (const char*)_memory;
        }

        _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        // Flawfinder: ignore
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
        if(_pAllocator->used == 0)
        {
            return;
        }

        uint8_t* _current_memory = _pAllocator->memory + _pAllocator->used;

        memset(_current_memory - _pAllocator->m_ullBlockSize, 0, _pAllocator->m_ullBlockSize);

        _pAllocator->used -= _pAllocator->m_ullBlockSize;
    }

    bool fixed_stack_allocator_empty(__fixed_stack_allocator* _pAlloctor)
    {
        return _pAlloctor->used == 0;
    }

    bool fixed_stack_allocator_full(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    void fixed_stack_clear(__fixed_stack_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}