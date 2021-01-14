#include "Memory/Internal/LinearAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* impl_linear_allocate(__linear_allocator& _allocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_allocator.m_ullCapacity < _allocator.m_ullUsed + _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_allocator.m_pMemory + _allocator.m_ullUsed, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _allocator.m_ullUsed += _ullSize + _padding;

        return _memory;
    }

    void impl_linear_clear(__linear_allocator& _allocator)
    {
        memset(_allocator.m_pMemory, 0, _allocator.m_ullCapacity);
        _allocator.m_ullUsed = 0;
    }

    void* impl_linear_allocate(__linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullCapacity < _pAllocator->m_ullUsed + _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullUsed += _ullSize + _padding;

        return _memory;
    }

    void impl_linear_clear(__linear_allocator* _pAllocator)
    {
        memset(_pAllocator->m_pMemory, 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}