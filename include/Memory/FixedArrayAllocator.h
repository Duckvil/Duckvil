#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_array_allocate(IMemory* _pMemory, __fixed_array_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedArrayAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    template <std::size_t Length>
    const char* fixed_array_allocate(IMemory* _pMemory, __fixed_array_allocator* _pAllocator, const char (&_pData)[Length])
    {
        return _pMemory->m_fnFixedArrayAllocateCStr_(_pAllocator, _pData, Length);
    }

    void* fixed_array_begin(IMemory* _pMemory, __fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayBegin_(_pAllocator);
    }

    void* fixed_array_back(IMemory* _pMemory, __fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayBack_(_pAllocator);
    }

    void* fixed_array_at(IMemory* _pMemory, __fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedArrayAt_(_pAllocator, _ullIndex);
    }

    bool fixed_array_empty(IMemory* _pMemory, __fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayEmpty_(_pAllocator);
    }

    bool fixed_array_full(IMemory* _pMemory, __fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayFull_(_pAllocator);
    }

}}