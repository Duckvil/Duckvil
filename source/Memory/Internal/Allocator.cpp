#include "Memory/Internal/Allocator.h"

#include <cstring>
#include <cstdlib>

#include "Memory/Internal/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    bool allocate(__linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator == nullptr)
        {
            return false;
        }

        if(_pAllocator->memory == nullptr)
        {
            _pAllocator->memory = (uint8_t*)std::malloc(_ullSize);

            if(_pAllocator->memory == nullptr)
            {
                return false;
            }

            memset(_pAllocator->memory, 0, _ullSize);

            _pAllocator->capacity = _ullSize;
        }

        return true;
    }

    __linear_allocator* allocate_linear_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->capacity < _ullSize + _pAllocator->used)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __linear_allocator* _memory = (__linear_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__linear_allocator), _padding);
        std::size_t _size = sizeof(__linear_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __fixed_stack_allocator* allocate_fixed_stack_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->capacity < _ullSize + _pAllocator->used)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __fixed_stack_allocator* _memory = (__fixed_stack_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__fixed_stack_allocator), _padding);
        std::size_t _size = sizeof(__fixed_stack_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __fixed_array_allocator* allocate_fixed_array_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->capacity < _ullSize + _pAllocator->used)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __fixed_array_allocator* _memory = (__fixed_array_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__fixed_array_allocator), _padding);
        std::size_t _size = sizeof(__fixed_array_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __free_list_allocator* allocate_free_list_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->capacity < _ullSize + _pAllocator->used)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __free_list_allocator* _allocator = (__free_list_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__free_list_allocator), _padding);

        _allocator->memory = (uint8_t*)_allocator + sizeof(__free_list_allocator);
        _allocator->capacity = _ullSize;
        _allocator->used = 0;

        _allocator->m_pHead = _allocator->memory;

        memset(_allocator->memory, 0, _ullSize);

        __free_list_node* _node = (__free_list_node*)_allocator->m_pHead;

        _node->m_ullSize = _ullSize;
        _node->m_pNext = nullptr;

        _pAllocator->used += _ullSize + sizeof(__free_list_allocator);

        return _allocator;
    }

    __fixed_vector_allocator* allocate_fixed_vector_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->capacity < _ullSize + _pAllocator->used)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __fixed_vector_allocator* _memory = (__fixed_vector_allocator*)calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, alignof(__fixed_vector_allocator), _padding);
        std::size_t _size = sizeof(__fixed_vector_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    __fixed_queue_allocator* allocate_fixed_queue_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        __fixed_queue_allocator* _allocator = (__fixed_queue_allocator*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__fixed_queue_allocator) + _ullSize, alignof(__fixed_queue_allocator));

        std::size_t _size = sizeof(__fixed_queue_allocator);

        _allocator->capacity = _ullSize;
        _allocator->memory = (uint8_t*)_allocator + _size;
        _allocator->used = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;
        _allocator->m_ullTail = 0;
        _allocator->m_ullHead = 0;

        memset(_allocator->memory, 0, _ullSize);

        return _allocator;
    }

}}