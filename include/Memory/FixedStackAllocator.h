#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    // template <typename Type>
    // static inline Type* fixed_stack_allocate(ftable* _pMemory, fixed_stack_allocator* _pAllocator, const Type& _pData)
    // {
    //     return static_cast<Type*>(_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type)));
    // }

    // template <typename Type>
    // static inline Type* fixed_stack_allocate(ftable* _pMemory, fixed_stack_allocator* _pAllocator, Type&& _data)
    // {
    //     return new(_pMemory->m_fnFixedStackAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
    // }

//     template <typename Type>
//     static inline typename std::enable_if<!std::is_copy_constructible<Type>::value, Type*>::type fixed_stack_allocate(ftable* _pMemory, fixed_stack_allocator* _pAllocator, const Type& _data)
//     {
// #pragma message("Warning: Bitwise copy")
//         return static_cast<Type*>(_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
//     }

//     template <typename Type, typename... Args>
//     static inline typename std::enable_if<std::is_copy_constructible<Type>::value, Type*>::type fixed_stack_allocate(ftable* _pMemory, fixed_stack_allocator* _pAllocator, Args&&... _data)
//     {
//         return new(_pMemory->m_fnFixedStackAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_data)...);
//     }

//     template <typename Type>
//     static inline Type* fixed_stack_allocate(ftable* _pMemory, fixed_stack_allocator* _pAllocator, Type&& _data)
//     {
//         return new(_pMemory->m_fnFixedStackAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
//     }

    template <typename Type, typename... Args>
    static inline Type* fixed_stack_emplace_back(const ftable* _pMemory, fixed_stack_allocator* _pAllocator, Args&&... _vData)
    {
        return new(_pMemory->m_fixedStackContainer.m_fnAllocateSize(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* fixed_stack_allocate(const ftable* _pMemory, fixed_stack_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_fixedStackContainer.m_fnAllocate(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return fixed_stack_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* fixed_stack_allocate(const ftable* _pMemory, fixed_stack_allocator* _pAllocator, Type&& _data)
    {
        return fixed_stack_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }

    static inline void* fixed_stack_top(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fixedStackContainer.m_fnTop(_pAllocator);
    }

    template <typename Type>
    static inline Type* fixed_stack_top(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return static_cast<Type*>(fixed_stack_top(_pMemory, _pAllocator));
    }

    static inline void fixed_stack_pop(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fixedStackContainer.m_fnPop(_pAllocator);
    }

    template <typename Type>
    static inline void fixed_stack_pop(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        Type* _object = fixed_stack_top<Type>(_pMemory, _pAllocator);

        _object->~Type();

        fixed_stack_pop(_pMemory, _pAllocator);
    }

    static inline bool fixed_stack_empty(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fixedStackContainer.m_fnEmpty(_pAllocator);
    }

    static inline bool fixed_stack_full(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fixedStackContainer.m_fnFull(_pAllocator);
    }

    static inline std::size_t fixed_stack_size(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fixedStackContainer.m_fnSize(_pAllocator);
    }

    static inline std::size_t fixed_stack_capacity(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fixedStackContainer.m_fnCapacity(_pAllocator);
    }

    static inline void fixed_stack_clear(const ftable* _pMemory, fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fixedStackContainer.m_fnClear(_pAllocator);
    }

}}