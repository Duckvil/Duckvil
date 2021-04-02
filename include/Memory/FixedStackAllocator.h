#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    inline Type* fixed_stack_allocate(IMemory* _pMemory, fixed_stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    inline void* fixed_stack_top(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackTop_(_pAllocator);
    }

    inline void fixed_stack_pop(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedStackPop_(_pAllocator);
    }

    inline bool fixed_stack_empty(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackEmpty_(_pAllocator);
    }

    inline bool fixed_stack_full(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackFull_(_pAllocator);
    }

    inline std::size_t fixed_stack_size(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackSize_(_pAllocator);
    }

    inline std::size_t fixed_stack_capacity(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackCapacity_(_pAllocator);
    }

    inline void fixed_stack_clear(IMemory* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedStackClear_(_pAllocator);
    }

}}