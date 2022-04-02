#pragma once

#include "Memory/Internal/Allocator.h"

#include <type_traits>

namespace Duckvil { namespace Memory {

    static inline void* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        return _pMemory->m_fnFreeListAllocate_(_pAllocator, _ullSize, _ucAlignment);
    }

    // // Create new object based on specific type without calling constructor
    // template <typename Type>
    // static inline typename std::enable_if<!std::is_default_constructible<Type>::value, Type*>::type free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator)
    // {
    //     return static_cast<Type*>(free_list_allocate(_pMemory, _pAllocator, sizeof(Type), alignof(Type)));
    // }

    // // Create new object based on specific type by calling constructor
    // template <typename Type>
    // static inline typename std::enable_if<std::is_default_constructible<Type>::value, Type*>::type free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator)
    // {
    //     return new(free_list_allocate(_pMemory, _pAllocator, sizeof(Type), alignof(Type))) Type();
    // }

    // // template <typename Type>
    // // static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, const Type& _data)
    // // {
    // //     return static_cast<Type*>(_pMemory->m_fnFreeListAllocate_D_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
    // // }

    // // template <typename Type>
    // // static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, Type&& _data)
    // // {
    // //     return new(free_list_allocate<Type>(_pMemory, _pAllocator)) Type(std::forward<Type>(_data));
    // // }

    // template <typename Type, typename... Args>
    // static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, Args&&... _vArgs)
    // {
    //     return new(free_list_allocate<Type>(_pMemory, _pAllocator)) Type(std::forward<Args>(_vArgs)...);
    // }



    template <typename Type, typename... Args>
    static inline Type* free_list_emplace_back(ftable* _pMemory, free_list_allocator* _pAllocator, Args&&... _vData)
    {
        return new(free_list_allocate(_pMemory, _pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_fnFreeListAllocate_D_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return free_list_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, Type&& _data)
    {
        return fixed_array_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }



    static inline void free_list_free(ftable* _pMemory, free_list_allocator* _pAllocator, void* _pPointer)
    {
        _pMemory->m_fnFreeListFree_(_pAllocator, _pPointer);
    }

    template <typename Type>
    static inline void free_list_free(ftable* _pMemory, free_list_allocator* _pAllocator, Type* _pPointer)
    {
        if(std::is_base_of<allocator, Type>::value)
        {
#ifdef DUCKVIL_MEMORY_DEBUGGER
            _pAllocator->m_fnOnDeallocate(_pAllocator, (allocator*)_pPointer);
#endif
        }

        _pPointer->~Type();

        free_list_free(_pMemory, _pAllocator, static_cast<void*>(_pPointer));
    }

}}