#include "Memory/Internal/FixedStackAllocator.h"

#include <cstring>
#include <stdexcept>

namespace Duckvil { namespace Memory {

    void* impl_fixed_stack_allocate(fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_fixed_stack_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_fixed_stack_allocate(fixed_stack_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(impl_fixed_stack_allocator_full(_pAllocator) || _ullSize != _pAllocator->m_ullBlockSize)
        {
            throw std::overflow_error("Stack is full!");

            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_stack_allocator) + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_stack_allocator_top(fixed_stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_stack_allocator) + _pAllocator->m_ullUsed;

        return _current_memory - _pAllocator->m_ullBlockSize;
    }

    void impl_fixed_stack_allocator_pop(fixed_stack_allocator* _pAllocator)
    {
        if(impl_fixed_stack_allocator_empty(_pAllocator))
        {
            throw std::underflow_error("Stack is empty!");

            return;
        }

        uint8_t* _current_memory = reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_stack_allocator) + _pAllocator->m_ullUsed;

        memset(_current_memory - _pAllocator->m_ullBlockSize, 0, _pAllocator->m_ullBlockSize);

        _pAllocator->m_ullUsed -= _pAllocator->m_ullBlockSize;
    }

    bool impl_fixed_stack_allocator_empty(fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_stack_allocator_full(fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_stack_clear(fixed_stack_allocator* _pAllocator)
    {
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_stack_allocator), 0, _pAllocator->m_ullCapacity);

        _pAllocator->m_ullUsed = 0;
    }

    std::size_t impl_fixed_stack_allocator_size(fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_fixed_stack_allocator_capacity(fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

}}