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

    void* stack_top(IMemory* _pMemory, __stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackTop_(_pAllocator);
    }

    void stack_pop(IMemory* _pMemory, __stack_allocator* _pAllocator)
    {
        _pMemory->m_fnStackPop_(_pAllocator);
    }

    bool stack_empty(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool stack_full(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    std::size_t stack_size(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    std::size_t stack_capacity(__stack_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

}}