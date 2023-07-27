#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

#include "Utils/Utils.h"
#include "Utils/Function.h"

namespace Duckvil { namespace Memory {

//     template <typename Type>
//     static inline typename std::enable_if<std::is_trivially_copy_constructible<Type>::value, Type*>::type fixed_vector_allocate(ftable* _pMemory, fixed_vector_allocator* _pAllocator, const Type& _data)
//     {
// #pragma message("Warning: Bitwise copy")
//         return static_cast<Type*>(_pMemory->m_fnFixedVectorAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
//     }

//     // template <typename Type, typename... Args>
//     // static inline typename std::enable_if<std::is_copy_constructible<Type>::value, Type*>::type fixed_vector_allocate(ftable* _pMemory, fixed_vector_allocator* _pAllocator, Args&&... _data)
//     // {
//     //     return new(_pMemory->m_fnFixedVectorAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_data)...);
//     // }

//     template <typename Type>
//     static inline typename std::enable_if<!std::is_trivially_copy_constructible<Type>::value, Type*>::type fixed_vector_allocate(ftable* _pMemory, fixed_vector_allocator* _pAllocator, const Type& _data)
//     {
//         return new(_pMemory->m_fnFixedVectorAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(_data);
//     }

    template <typename Type, typename... Args>
    static inline Type* fixed_vector_emplace_back(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, Args&&... _vData)
    {
        return new(_pMemory->m_fixedVectorContainer.m_fnAllocateSize(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* fixed_vector_allocate(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_fixedVectorContainer.m_fnAllocate(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return fixed_vector_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* fixed_vector_allocate(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, Type&& _data)
    {
        return fixed_vector_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }

    static inline void* fixed_vector_begin(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnBegin(_pAllocator);
    }

    static inline void* fixed_vector_back(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnBack(_pAllocator);
    }

    static inline void* fixed_vector_end(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnEnd(_pAllocator);
    }

    static inline void* fixed_vector_at(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fixedVectorContainer.m_fnAt(_pAllocator, _ullIndex);
    }

    template <typename Type>
    static inline Type* fixed_vector_at(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return static_cast<Type*>(fixed_vector_at(_pMemory, _pAllocator, _ullIndex));
    }

    static inline bool fixed_vector_empty(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnEmpty(_pAllocator);
    }

    static inline bool fixed_vector_full(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnFull(_pAllocator);
    }

    static inline void fixed_vector_resize(const ftable* _pMemory, free_list_allocator* m_pParentAllocator, fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_fixedVectorContainer.m_fnResize(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

    static inline std::size_t fixed_vector_size(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnSize(_pAllocator);
    }

    static inline std::size_t fixed_vector_capacity(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fixedVectorContainer.m_fnCapacity(_pAllocator);
    }

    static inline void fixed_vector_erase(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, uint32_t _uiIndex)
    {
        _pMemory->m_fixedVectorContainer.m_fnErase(_pAllocator, _uiIndex);
    }

    template <typename Type>
    static inline void fixed_vector_erase(const ftable* _pMemory, fixed_vector_allocator* _pAllocator, uint32_t _uiIndex)
    {
        Type* _object = fixed_vector_at<Type>(_pMemory, _pAllocator, _uiIndex);

        _object->~Type();

        fixed_vector_erase(_pMemory, _pAllocator, _uiIndex);
    }

    static inline void fixed_vector_pop_back(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        uint32_t _index = fixed_vector_size(_pMemory, _pAllocator) / _pAllocator->m_ullBlockSize;

        fixed_vector_erase(_pMemory, _pAllocator, _index - 1);
    }

    template <typename Type>
    static inline void fixed_vector_pop_back(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        uint32_t _index = fixed_vector_size(_pMemory, _pAllocator) / _pAllocator->m_ullBlockSize;

        fixed_vector_erase<Type>(_pMemory, _pAllocator, _index - 1);
    }

    static inline void fixed_vector_clear(const ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        _pMemory->m_fixedVectorContainer.m_fnClear(_pAllocator);
    }

    template <typename Type>
    static inline void fixed_vector_copy(const ftable* _pMemory, fixed_vector_allocator* _pSourceAllocator, fixed_vector_allocator* _pTargetAllocator)
    {
        _pMemory->m_fixedVectorContainer.m_fnCopy(_pSourceAllocator, _pTargetAllocator, Utils::lambda([_pMemory](fixed_vector_allocator* _pTargetAllocator, void* _pValue)
        {
        	fixed_vector_allocate<Type>(_pMemory, _pTargetAllocator, *static_cast<Type*>(_pValue));
        }));
    }

}}