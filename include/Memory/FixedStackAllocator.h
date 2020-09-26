#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    template <std::size_t Length>
    const char* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const char (&_pData)[Length])
    {
        return _pMemory->m_fnFixedStackAllocateCStr_(_pAllocator, _pData, Length);
    }

    void* fixed_stack_top(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackTop_(_pAllocator);
    }

    void fixed_stack_pop(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedStackPop_(_pAllocator);
    }

    bool fixed_stack_empty(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_stack_full(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    std::size_t fixed_stack_size(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    std::size_t fixed_stack_capacity(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

}}