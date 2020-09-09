#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_queue_allocate(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedQueueAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* fixed_queue_allocate(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnFixedQueueAllocateCStr_(_pAllocator, _pData);
    }

}}