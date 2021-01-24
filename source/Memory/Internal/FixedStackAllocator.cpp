#include "Memory/Internal/FixedStackAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* impl_fixed_stack_allocate(__fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullCapacity < _pAllocator->m_ullUsed + _ullSize || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_stack_allocator_top(__fixed_stack_allocator* _pAllocator)
    {
        uint8_t* _current_memory = _pAllocator->m_pMemory + _pAllocator->m_ullUsed;

        return _current_memory - _pAllocator->m_ullBlockSize;
    }

    void impl_fixed_stack_allocator_pop(__fixed_stack_allocator* _pAllocator)
    {
        if(_pAllocator->m_ullUsed == 0)
        {
            return;
        }

        uint8_t* _current_memory = _pAllocator->m_pMemory + _pAllocator->m_ullUsed;

        memset(_current_memory - _pAllocator->m_ullBlockSize, 0, _pAllocator->m_ullBlockSize);

        _pAllocator->m_ullUsed -= _pAllocator->m_ullBlockSize;
    }

    bool impl_fixed_stack_allocator_empty(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_stack_allocator_full(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_stack_clear(__fixed_stack_allocator* _pAllocator)
    {
        memset(_pAllocator->m_pMemory, 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

    std::size_t impl_fixed_stack_allocator_size(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_fixed_stack_allocator_capacity(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

}}