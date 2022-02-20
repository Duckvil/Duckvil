#include "Memory/Internal/FixedArrayAllocator.h"

#include <cstring>
#include <stdexcept>

namespace Duckvil { namespace Memory {

    void* impl_fixed_array_allocate(fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_fixed_array_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_fixed_array_allocate(fixed_array_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(impl_fixed_array_full(_pAllocator) || _ullSize != _pAllocator->m_ullBlockSize)
        {
            throw std::overflow_error("Array is full!");

            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_array_allocator) + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_array_begin(fixed_array_allocator* _pAllocator)
    {
        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_array_allocator);
    }

    void* impl_fixed_array_back(fixed_array_allocator* _pAllocator)
    {
        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_array_allocator) + _pAllocator->m_ullUsed - _pAllocator->m_ullBlockSize;
    }

    void* impl_fixed_array_at(fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        if(_ullIndex * _pAllocator->m_ullBlockSize > _pAllocator->m_ullCapacity)
        {
            throw std::out_of_range("Index is out of range!");

            return nullptr;
        }

        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_array_allocator) + (_ullIndex * _pAllocator->m_ullBlockSize);
    }

    std::size_t impl_fixed_array_size(fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    bool impl_fixed_array_empty(fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_array_full(fixed_array_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_array_clear(fixed_array_allocator* _pAllocator)
    {
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_array_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}