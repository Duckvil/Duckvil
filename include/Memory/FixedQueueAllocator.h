#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    inline Type* fixed_queue_allocate(ftable* _pMemory, fixed_queue_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedQueueAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    template <typename Type>
    static Type* fixed_queue_allocate(ftable* _pMemory, fixed_queue_allocator* _pAllocator, Type&& _data)
    {
        return (Type*)new(_pMemory->m_fnFixedQueueAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
    }

    inline void* fixed_queue_begin(ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueBegin_(_pAllocator);
    }

    inline void fixed_queue_pop(ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedQueuePop_(_pAllocator);
    }

    inline bool fixed_queue_empty(ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueEmpty_(_pAllocator);
        // return _pAllocator->m_ullUsed == 0;
    }

    inline bool fixed_queue_full(ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueFull_(_pAllocator);
        // return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    inline void fixed_queue_resize(ftable* _pMemory, free_list_allocator* m_pParentAllocator, fixed_queue_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_fnFixedQueueResize_(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

    inline std::size_t fixed_queue_size(ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueSize_(_pAllocator);
        // return _pAllocator->m_ullUsed;
    }

    inline std::size_t fixed_queue_capacity(ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueCapacity_(_pAllocator);
        // return _pAllocator->m_ullCapacity;
    }

}}