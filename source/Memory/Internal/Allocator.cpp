#include "Memory/Internal/Allocator.h"

#include <cstring>
#include <cstdlib>

#include "Memory/Internal/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    bool impl_allocate(__linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator == nullptr)
        {
            return false;
        }

        if(_pAllocator->m_pMemory == nullptr)
        {
            _pAllocator->m_pMemory = (uint8_t*)std::malloc(_ullSize);

            if(_pAllocator->m_pMemory == nullptr)
            {
                return false;
            }

            memset(_pAllocator->m_pMemory, 0, _ullSize);

            _pAllocator->m_ullCapacity = _ullSize;
        }

        return true;
    }

    __linear_allocator* impl_linear_allocate_linear_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __linear_allocator* _memory = (__linear_allocator*)calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, alignof(__linear_allocator), _padding);
        std::size_t _size = sizeof(__linear_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_pMemory = (uint8_t*)_memory + _size;
        _memory->m_ullUsed = 0;

        memset(_memory->m_pMemory, 0, _ullSize);

        _pAllocator->m_ullUsed += _size + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_linear);
#endif

        return _memory;
    }

    __fixed_stack_allocator* impl_linear_allocate_fixed_stack_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __fixed_stack_allocator* _memory = (__fixed_stack_allocator*)calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, alignof(__fixed_stack_allocator), _padding);
        std::size_t _size = sizeof(__fixed_stack_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_pMemory = (uint8_t*)_memory + _size;
        _memory->m_ullUsed = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->m_pMemory, 0, _ullSize);

        _pAllocator->m_ullUsed += _size + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_stack);
#endif

        return _memory;
    }

    __fixed_array_allocator* impl_linear_allocate_fixed_array_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __fixed_array_allocator* _memory = (__fixed_array_allocator*)calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, alignof(__fixed_array_allocator), _padding);
        std::size_t _size = sizeof(__fixed_array_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_pMemory = (uint8_t*)_memory + _size;
        _memory->m_ullUsed = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->m_pMemory, 0, _ullSize);

        _pAllocator->m_ullUsed += _size + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_array);
#endif

        return _memory;
    }

    __free_list_allocator* impl_linear_allocate_free_list_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __free_list_allocator* _allocator = (__free_list_allocator*)calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, alignof(__free_list_allocator), _padding);

        _allocator->m_pMemory = (uint8_t*)_allocator + sizeof(__free_list_allocator);
        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;

        _allocator->m_pHead = _allocator->m_pMemory;

        memset(_allocator->m_pMemory, 0, _ullSize);

        __free_list_node* _node = (__free_list_node*)_allocator->m_pHead;

        _node->m_ullSize = _ullSize;
        _node->m_pNext = nullptr;

        _pAllocator->m_ullUsed += _ullSize + sizeof(__free_list_allocator);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_free_list);
#endif

        return _allocator;
    }

    __fixed_vector_allocator* impl_linear_allocate_fixed_vector_allocator(__linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        __fixed_vector_allocator* _memory = (__fixed_vector_allocator*)calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, alignof(__fixed_vector_allocator), _padding);
        std::size_t _size = sizeof(__fixed_vector_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_pMemory = (uint8_t*)_memory + _size;
        _memory->m_ullUsed = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(_memory->m_pMemory, 0, _ullSize);

        _pAllocator->m_ullUsed += _size + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_vector);
#endif

        return _memory;
    }

    __fixed_queue_allocator* impl_free_list_allocate_fixed_queue_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        __fixed_queue_allocator* _allocator = (__fixed_queue_allocator*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__fixed_queue_allocator) + _ullSize, alignof(__fixed_queue_allocator));

        std::size_t _size = sizeof(__fixed_queue_allocator);

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_pMemory = (uint8_t*)_allocator + _size;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;
        _allocator->m_ullTail = 0;
        _allocator->m_ullHead = 0;

        memset(_allocator->m_pMemory, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_queue);
#endif

        return _allocator;
    }

    __free_list_allocator* impl_free_list_allocate_free_list_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize)
    {
        __free_list_allocator* _allocator = (__free_list_allocator*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__free_list_allocator) + _ullSize, alignof(__free_list_allocator));

        std::size_t _size = sizeof(__free_list_allocator);

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_pMemory = (uint8_t*)_allocator + _size;
        _allocator->m_ullUsed = 0;

        _allocator->m_pHead = _allocator->m_pMemory;

        memset(_allocator->m_pMemory, 0, _ullSize);

        __free_list_node* _node = (__free_list_node*)_allocator->m_pHead;

        _node->m_ullSize = _ullSize;
        _node->m_pNext = nullptr;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_free_list);
#endif

        return _allocator;
    }

    __fixed_vector_allocator* impl_free_list_allocate_fixed_vector_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        __fixed_vector_allocator* _allocator = (__fixed_vector_allocator*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__fixed_vector_allocator) + _ullSize, alignof(__fixed_vector_allocator));
        std::size_t _size = sizeof(__fixed_vector_allocator);

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_pMemory = (uint8_t*)_allocator + _size;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;

        memset(_allocator->m_pMemory, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_vector);
#endif

        return _allocator;
    }

    __fixed_stack_allocator* impl_free_list_allocate_fixed_stack_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        __fixed_stack_allocator* _allocator = (__fixed_stack_allocator*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__fixed_stack_allocator) + _ullSize, alignof(__fixed_stack_allocator));
        std::size_t _size = sizeof(__fixed_stack_allocator);

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_pMemory = (uint8_t*)_allocator + _size;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;

        memset(_allocator->m_pMemory, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_stack);
#endif

        return _allocator;
    }

    __fixed_array_allocator* impl_free_list_allocate_fixed_array_allocator(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        __fixed_array_allocator* _allocator = (__fixed_array_allocator*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__fixed_array_allocator) + _ullSize, alignof(__fixed_array_allocator));
        std::size_t _size = sizeof(__fixed_array_allocator);

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_pMemory = (uint8_t*)_allocator + _size;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;

        memset(_allocator->m_pMemory, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_stack);
#endif

        return _allocator;
    }

}}