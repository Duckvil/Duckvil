#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* free_list_allocate(IMemory* _pMemory, __free_list_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFreeListAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    void* free_list_allocate(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        return _pMemory->m_fnFreeListAllocate_(_pAllocator, 0, _ullSize, _ucAlignment);
    }

    void free_list_free(IMemory* _pMemory, __free_list_allocator* _pAllocator, void** _pointer)
    {
        _pMemory->m_fnFreeListFree_(_pAllocator, _pointer);
    }

}}