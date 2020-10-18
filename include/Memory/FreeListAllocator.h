#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    static Type* free_list_allocate(IMemory* _pMemory, __free_list_allocator* _pAllocator, const Type& _data)
    {
        return (Type*)_pMemory->m_fnFreeListAllocate_D_(_pAllocator, &_data, sizeof(Type), alignof(Type));
    }

    static void* free_list_allocate(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        return _pMemory->m_fnFreeListAllocate_(_pAllocator, _ullSize, _ucAlignment);
    }

    static void free_list_free(IMemory* _pMemory, __free_list_allocator* _pAllocator, void* _pointer)
    {
        _pMemory->m_fnFreeListFree_(_pAllocator, _pointer);
    }

}}