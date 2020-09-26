#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_queue_allocate(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedQueueAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    template <std::size_t Length>
    const char* fixed_queue_allocate(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator, const char (&_pData)[Length])
    {
        return _pMemory->m_fnFixedQueueAllocateCStr_(_pAllocator, _pData, Length);
    }

    void* fixed_queue_begin(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueBegin_(_pAllocator);
    }

    void fixed_queue_pop(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedQueuePop_(_pAllocator);
    }

    bool fixed_queue_empty(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_queue_full(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    std::size_t fixed_queue_size(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    std::size_t fixed_queue_capacity(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

}}