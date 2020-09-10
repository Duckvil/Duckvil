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
        uint8_t _padding = 0;
        __linear_allocator* _memory = (__linear_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__linear_allocator), _padding);
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
        uint8_t _padding = 0;
        __fixed_stack_allocator* _memory = (__fixed_stack_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__fixed_stack_allocator), _padding);
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
        uint8_t _padding = 0;
        __stack_allocator* _memory = (__stack_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__stack_allocator), _padding);
        size_t _size = sizeof(__stack_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __fixed_queue_allocator* allocate_fixed_queue_allocator(__allocator* _pAllocator, size_t _ullSize, size_t _ullTypeSize)
    {
        uint8_t _padding = 0;
        __fixed_queue_allocator* _memory = (__fixed_queue_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__fixed_queue_allocator), _padding);
        size_t _size = sizeof(__fixed_queue_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;
        _memory->m_ullBlockSize = _ullTypeSize;
        _memory->m_ullTail = 0;
        _memory->m_ullHead = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __queue_allocator* allocate_queue_allocator(__allocator* _pAllocator, size_t _ullSize)
    {
        uint8_t _padding = 0;
        __queue_allocator* _memory = (__queue_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__queue_allocator), _padding);
        size_t _size = sizeof(__queue_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;
        _memory->m_ullTail = 0;
        _memory->m_ullHead = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

}}