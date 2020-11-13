#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    inline Type* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    inline void* fixed_stack_top(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackTop_(_pAllocator);
    }

    inline void fixed_stack_pop(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedStackPop_(_pAllocator);
    }

    inline bool fixed_stack_empty(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    inline bool fixed_stack_full(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    inline std::size_t fixed_stack_size(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    inline std::size_t fixed_stack_capacity(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

}}