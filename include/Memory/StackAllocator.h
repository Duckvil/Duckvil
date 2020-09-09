#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* stack_allocate(IMemory* _pMemory, __stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* stack_allocate(IMemory* _pMemory, __stack_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnStackAllocateCStr_(_pAllocator, _pData);
    }

}}