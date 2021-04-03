#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* linear_allocate(ftable* _pMemory, linear_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnLinearAllocate(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    bool linear_empty(linear_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool linear_full(linear_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    std::size_t linear_size(linear_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t linear_capacity(linear_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

}}