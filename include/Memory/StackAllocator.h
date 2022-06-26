#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type, typename... Args>
    static inline Type* stack_emplace_back(ftable* _pMemory, stack_allocator* _pAllocator, Args&&... _vData)
    {
        return new(_pMemory->m_fnStackAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* stack_allocate(ftable* _pMemory, stack_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_fnStackAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return stack_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* stack_allocate(ftable* _pMemory, stack_allocator* _pAllocator, Type&& _data)
    {
        return stack_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }

    static inline void* stack_top(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackTop_(_pAllocator);
    }

    template <typename Type>
    static inline Type* stack_top(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        return static_cast<Type*>(stack_top(_pMemory, _pAllocator));
    }

    static inline void stack_pop(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        _pMemory->m_fnStackPop_(_pAllocator);
    }

    template <typename Type>
    static inline void stack_pop(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        Type* _object = stack_top<Type>(_pMemory, _pAllocator);

        _object->~Type();

        stack_pop(_pMemory, _pAllocator);
    }

    static inline bool stack_empty(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackEmpty_(_pAllocator);
    }

    static inline bool stack_full(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackFull_(_pAllocator);
    }

    static inline std::size_t stack_size(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackSize_(_pAllocator);
    }

    static inline std::size_t stack_capacity(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackCapacity_(_pAllocator);
    }

    static inline void stack_clear(ftable* _pMemory, stack_allocator* _pAllocator)
    {
        _pMemory->m_fnStackClear_(_pAllocator);
    }

}}