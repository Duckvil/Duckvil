#include "Memory/Internal/FixedStackAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* impl_fixed_array_allocate(__fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullUsed > _pAllocator->m_ullCapacity || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer((uint8_t*)_pAllocator + sizeof(__fixed_array_allocator) + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_array_begin(__fixed_array_allocator* _pAllocator)
    {
        return (uint8_t*)_pAllocator + sizeof(__fixed_array_allocator);
    }

    void* impl_fixed_array_back(__fixed_array_allocator* _pAllocator)
    {
        return (uint8_t*)_pAllocator + sizeof(__fixed_array_allocator) + _pAllocator->m_ullUsed - _pAllocator->m_ullBlockSize;
    }

    void* impl_fixed_array_at(__fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return (uint8_t*)_pAllocator + sizeof(__fixed_array_allocator) + (_ullIndex * _pAllocator->m_ullBlockSize);
    }

    std::size_t impl_fixed_array_size(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    bool impl_fixed_array_empty(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_array_full(__fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_array_clear(__fixed_array_allocator* _pAllocator)
    {
        memset((uint8_t*)_pAllocator + sizeof(__fixed_array_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}