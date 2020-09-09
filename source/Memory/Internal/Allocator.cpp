#include "Memory/Internal/Allocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    bool allocate(__allocator* _pAllocator, size_t _ullSize)
    {
        if(_pAllocator->memory == NULL)
        {
            _pAllocator->memory = (uint8_t*)malloc(_ullSize);

            if(_pAllocator->memory == NULL)
            {
                return false;
            }

            memset(_pAllocator->memory, 0, _ullSize);

            _pAllocator->capacity = _ullSize;
        }

        return true;
    }

    __linear_allocator* allocate_linear_allocator(__allocator* _pAllocator, size_t _ullSize)
    {
         __linear_allocator* _memory = (__linear_allocator*)(_pAllocator->memory + _pAllocator->used);
        size_t _size = sizeof(__linear_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __fixed_stack_allocator* allocate_fixed_stack_allocator(__allocator* _pAllocator, size_t _ullSize, size_t _ullTypeSize)
    {
         __fixed_stack_allocator* _memory = (__fixed_stack_allocator*)(_pAllocator->memory + _pAllocator->used);
        size_t _size = sizeof(__fixed_stack_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __stack_allocator* allocate_stack_allocator(__allocator* _pAllocator, size_t _ullSize)
    {
         __stack_allocator* _memory = (__stack_allocator*)(_pAllocator->memory + _pAllocator->used);
        size_t _size = sizeof(__stack_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

}}