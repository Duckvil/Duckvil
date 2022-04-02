#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

namespace Duckvil { namespace Memory {

    // template <typename Type>
    // static inline Type* fixed_array_allocate(ftable* _pMemory, fixed_array_allocator* _pAllocator, const Type& _pData)
    // {
    //     return static_cast<Type*>(_pMemory->m_fnFixedArrayAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type)));
    // }

    // template <typename Type>
    // static inline Type* fixed_array_allocate(ftable* _pMemory, fixed_array_allocator* _pAllocator, Type&& _data)
    // {
    //     return new(_pMemory->m_fnFixedArrayAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
    // }

    template <typename Type, typename... Args>
    static inline Type* fixed_array_emplace_back(ftable* _pMemory, fixed_array_allocator* _pAllocator, Args&&... _vData)
    {
        return new(_pMemory->m_fnFixedArrayAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* fixed_array_allocate(ftable* _pMemory, fixed_array_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_fnFixedArrayAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return fixed_array_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* fixed_array_allocate(ftable* _pMemory, fixed_array_allocator* _pAllocator, Type&& _data)
    {
        return fixed_array_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }

    static inline void* fixed_array_begin(ftable* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayBegin_(_pAllocator);
    }

    static inline void* fixed_array_back(ftable* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayBack_(_pAllocator);
    }

    static inline void* fixed_array_at(ftable* _pMemory, fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedArrayAt_(_pAllocator, _ullIndex);
    }

    template <typename Type>
    static inline Type* fixed_array_at(ftable* _pMemory, fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return static_cast<Type*>(fixed_array_at(_pMemory, _pAllocator, _ullIndex));
    }

    static inline bool fixed_array_empty(ftable* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayEmpty_(_pAllocator);
    }

    static inline bool fixed_array_full(ftable* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayFull_(_pAllocator);
    }

    static inline std::size_t fixed_array_size(ftable* _pMemory, fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArraySize_(_pAllocator);
    }

    static inline void fixed_array_clear(ftable* _pMemory, fixed_array_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedArrayClear_(_pAllocator);
    }

}}