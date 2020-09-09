#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnFixedStackAllocateCStr_(_pAllocator, _pData);
    }

}}