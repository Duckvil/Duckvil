#include "Memory/Internal/FixedStackAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* fixed_array_allocate(__fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->used > _pAllocator->capacity || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->used += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    const char* fixed_array_allocate(__fixed_array_allocator* _pAllocator, const char* _pData, std::size_t _ullLength)
    {
        void* _memory = nullptr;

        if(_pAllocator->capacity < _pAllocator->used + _ullLength)
        {
            return 0;
        }

        _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _ullLength);

        _pAllocator->used += _ullLength;

        return (const char*)_memory;
    }

    void* fixed_array_begin(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->memory;
    }

    void* fixed_array_back(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->memory + _pAllocator->used - _pAllocator->m_ullBlockSize;
    }

    void* fixed_array_at(__fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pAllocator->memory + (_ullIndex * _pAllocator->m_ullBlockSize);
    }

    std::size_t fixed_array_size(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    bool fixed_array_empty(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_array_full(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    void fixed_array_clear(__fixed_array_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}