#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    inline Type* fixed_array_allocate(IMemory* _pMemory, fixed_array_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedArrayAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    inline void* fixed_array_begin(IMemory* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayBegin_(_pAllocator);
    }

    inline void* fixed_array_back(IMemory* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayBack_(_pAllocator);
    }

    inline void* fixed_array_at(IMemory* _pMemory, fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedArrayAt_(_pAllocator, _ullIndex);
    }

    inline bool fixed_array_empty(IMemory* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayEmpty_(_pAllocator);
    }

    inline bool fixed_array_full(IMemory* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayFull_(_pAllocator);
    }

    inline std::size_t fixed_array_size(IMemory* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArraySize_(_pAllocator);
    }

    inline void fixed_array_clear(IMemory* _pMemory, fixed_array_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedArrayClear_(_pAllocator);
    }

}}