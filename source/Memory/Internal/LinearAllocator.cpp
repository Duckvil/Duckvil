#include "Memory/Internal/LinearAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* impl_linear_allocate(linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullCapacity < _pAllocator->m_ullUsed + _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer((uint8_t*)_pAllocator + sizeof(linear_allocator) + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullUsed += _ullSize + _padding;

        return _memory;
    }

    void impl_linear_clear(linear_allocator* _pAllocator)
    {
        memset((uint8_t*)_pAllocator + sizeof(linear_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}