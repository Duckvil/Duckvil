#include "Memory/Internal/Allocator.h"

#include <cstring>
#include <cstdlib>
#include <string>

#include "Memory/Internal/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    bool impl_allocate(linear_allocator** _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator == nullptr)
        {
            return false;
        }

        *_pAllocator = static_cast<linear_allocator*>(std::malloc(_ullSize + sizeof(linear_allocator)));

        if(*_pAllocator == nullptr)
        {
            return false;
        }

        memset(*_pAllocator, 0, _ullSize + sizeof(linear_allocator));

        (*_pAllocator)->m_ullCapacity = _ullSize;

        return true;
    }

    bool impl_free(linear_allocator** _pAllocator)
    {
        if(*_pAllocator == nullptr)
        {
            return false;
        }

        std::free(*_pAllocator);

        *_pAllocator = nullptr;

        return true;
    }

    linear_allocator* impl_linear_allocate_linear_allocator(linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(linear_allocator);
        linear_allocator* _memory = static_cast<linear_allocator*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + ALLOCATOR_SIZE + _pAllocator->m_ullUsed, alignof(linear_allocator), _padding));

        _memory->m_ullCapacity = _ullSize;
        _memory->m_ullUsed = 0;

        memset(reinterpret_cast<uint8_t*>(_memory) + ALLOCATOR_SIZE, 0, _ullSize);

        _pAllocator->m_ullUsed += ALLOCATOR_SIZE + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_linear);
#endif

        return _memory;
    }

    fixed_stack_allocator* impl_linear_allocate_fixed_stack_allocator(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        fixed_stack_allocator* _memory = static_cast<fixed_stack_allocator*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(linear_allocator) + _pAllocator->m_ullUsed, alignof(fixed_stack_allocator), _padding));
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_stack_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_ullUsed = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(reinterpret_cast<uint8_t*>(_memory) + ALLOCATOR_SIZE, 0, _ullSize);

        _pAllocator->m_ullUsed += ALLOCATOR_SIZE + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_stack);
#endif

        return _memory;
    }

    fixed_array_allocator* impl_linear_allocate_fixed_array_allocator(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        fixed_array_allocator* _memory = static_cast<fixed_array_allocator*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(linear_allocator) + _pAllocator->m_ullUsed, alignof(fixed_array_allocator), _padding));
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_array_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_ullUsed = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(reinterpret_cast<uint8_t*>(_memory) + ALLOCATOR_SIZE, 0, _ullSize);

        _pAllocator->m_ullUsed += ALLOCATOR_SIZE + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_array);
#endif

        return _memory;
    }

    free_list_allocator* impl_linear_allocate_free_list_allocator(linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        free_list_allocator* _allocator = static_cast<free_list_allocator*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(linear_allocator) + _pAllocator->m_ullUsed, alignof(free_list_allocator), _padding));
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_array_allocator);

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;

        _allocator->m_pHead = reinterpret_cast<__free_list_node*>((reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE));

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

        __free_list_node* _node = static_cast<__free_list_node*>(_allocator->m_pHead);

        _node->m_ullSize = _ullSize;
        _node->m_pNext = nullptr;

        _pAllocator->m_ullUsed += _ullSize + ALLOCATOR_SIZE;

#ifdef TRACY_ENABLE
        /*auto _name = "aaa";
        _allocator->m_sName = new char[strlen(_name) + 1];
        memcpy(_allocator->m_sName, _name, strlen(_name) + 1);*/
        _allocator->m_sName = nullptr;
#endif

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_free_list);
#endif

        return _allocator;
    }

    fixed_vector_allocator* impl_linear_allocate_fixed_vector_allocator(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        fixed_vector_allocator* _memory = static_cast<fixed_vector_allocator*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(linear_allocator) + _pAllocator->m_ullUsed, alignof(fixed_vector_allocator), _padding));
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_vector_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_ullUsed = 0;
        _memory->m_ullBlockSize = _ullTypeSize;

        memset(reinterpret_cast<uint8_t*>(_memory) + ALLOCATOR_SIZE, 0, _ullSize);

        _pAllocator->m_ullUsed += ALLOCATOR_SIZE + _ullSize;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _memory->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_vector);
#endif

        return _memory;
    }

    byte_buffer_allocator* impl_linear_allocate_byte_buffer_allocator(linear_allocator* _pAllocator, std::size_t _ullSize)
    {
        if(_pAllocator->m_ullCapacity < _ullSize + _pAllocator->m_ullUsed)
        {
            return 0;
        }

        uint8_t _padding = 0;
        byte_buffer_allocator* _memory = static_cast<byte_buffer_allocator*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(linear_allocator) + _pAllocator->m_ullUsed, alignof(byte_buffer_allocator), _padding));
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(byte_buffer_allocator);

        _memory->m_ullCapacity = _ullSize;
        _memory->m_ullUsed = 0;

        memset(reinterpret_cast<uint8_t*>(_memory) + ALLOCATOR_SIZE, 0, _ullSize);

        _pAllocator->m_ullUsed += ALLOCATOR_SIZE + _ullSize;

// #ifdef DUCKVIL_MEMORY_DEBUGGER
//         _memory->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
//         _memory->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
//         _memory->m_fnOnAllocate(_pAllocator, _memory, duckvil_memory_allocator_type_vector);
// #endif

        return _memory;
    }

    queue_allocator* impl_free_list_allocate_queue_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(queue_allocator);

        queue_allocator* _allocator = static_cast<queue_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(queue_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullTail = 0;
        _allocator->m_ullHead = 0;

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_queue);
#endif

        return _allocator;
    }

    fixed_queue_allocator* impl_free_list_allocate_fixed_queue_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_queue_allocator);
        fixed_queue_allocator* _allocator = static_cast<fixed_queue_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(fixed_queue_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;
        _allocator->m_ullTail = 0;
        _allocator->m_ullHead = 0;

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_queue);
#endif

        return _allocator;
    }

    free_list_allocator* impl_free_list_allocate_free_list_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(free_list_allocator);
        free_list_allocator* _allocator = static_cast<free_list_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(free_list_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;

        _allocator->m_pHead = reinterpret_cast<__free_list_node*>((reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE));

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

        __free_list_node* _node = static_cast<__free_list_node*>(_allocator->m_pHead);

        _node->m_ullSize = _ullSize;
        _node->m_pNext = nullptr;

#ifdef TRACY_ENABLE
        static int _counter = 0;

        _counter++;

        auto _name = (std::string("FreeList_") + std::to_string(_counter));
        _allocator->m_sName = new char[_name.length() + 1];
        memcpy(_allocator->m_sName, &_name[0], _name.length() + 1);
#endif

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_free_list);
#endif

        return _allocator;
    }

    fixed_vector_allocator* impl_free_list_allocate_fixed_vector_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_vector_allocator);
        fixed_vector_allocator* _allocator = static_cast<fixed_vector_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(fixed_vector_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_vector);
#endif

        return _allocator;
    }

    fixed_stack_allocator* impl_free_list_allocate_fixed_stack_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_stack_allocator);
        fixed_stack_allocator* _allocator = static_cast<fixed_stack_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(fixed_stack_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_stack);
#endif

        return _allocator;
    }

    fixed_array_allocator* impl_free_list_allocate_fixed_array_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(fixed_array_allocator);
        fixed_array_allocator* _allocator = static_cast<fixed_array_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(fixed_array_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullBlockSize = _ullTypeSize;

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_stack);
#endif

        return _allocator;
    }

    byte_buffer_allocator* impl_free_list_allocate_byte_buffer_allocator(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize)
    {
        constexpr std::size_t ALLOCATOR_SIZE = sizeof(byte_buffer_allocator);
        byte_buffer_allocator* _allocator = static_cast<byte_buffer_allocator*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, ALLOCATOR_SIZE + _ullSize, alignof(byte_buffer_allocator)));

        _allocator->m_ullCapacity = _ullSize;
        _allocator->m_ullUsed = 0;
        _allocator->m_ullPosition = 0;

        memset(reinterpret_cast<uint8_t*>(_allocator) + ALLOCATOR_SIZE, 0, _ullSize);

// TODO:
// #ifdef DUCKVIL_MEMORY_DEBUGGER
//         _allocator->m_fnOnAllocate = _pAllocator->m_fnOnAllocate;
//         _allocator->m_fnOnDeallocate = _pAllocator->m_fnOnDeallocate;
//         _allocator->m_fnOnAllocate(_pAllocator, _allocator, duckvil_memory_allocator_type_stack);
// #endif

        return _allocator;
    }

}}