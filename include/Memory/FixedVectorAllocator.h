#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

#include "Utils/Utils.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    static Type* fixed_vector_allocate(ftable* _pMemory, fixed_vector_allocator* _pAllocator, const Type& _data)
    {
        return (Type*)_pMemory->m_fnFixedVectorAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type));
    }

    template <typename Type>
    static Type* fixed_vector_allocate(ftable* _pMemory, fixed_vector_allocator* _pAllocator, Type&& _data)
    {
        return (Type*)new(_pMemory->m_fnFixedVectorAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
    }

    static void* fixed_vector_begin(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorBegin_(_pAllocator);
    }

    static void* fixed_vector_back(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorBack_(_pAllocator);
    }

    static void* fixed_vector_end(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorEnd_(_pAllocator);
    }

    static void* fixed_vector_at(ftable* _pMemory, fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedVectorAt_(_pAllocator, _ullIndex);
    }

    template <typename Type>
    static Type* fixed_vector_at(ftable* _pMemory, fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return (Type*)fixed_vector_at(_pMemory, _pAllocator, _ullIndex);
    }

    static bool fixed_vector_empty(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorEmpty_(_pAllocator);
    }

    static bool fixed_vector_full(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorFull_(_pAllocator);
    }

    static void fixed_vector_resize(ftable* _pMemory, free_list_allocator* m_pParentAllocator, fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_fnFixedVectorResize_(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

    static std::size_t fixed_vector_size(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorSize_(_pAllocator);
    }

    static std::size_t fixed_vector_capacity(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorCapacity_(_pAllocator);
    }

    static void fixed_vector_erase(ftable* _pMemory, fixed_vector_allocator* _pAllocator, uint32_t _uiIndex)
    {
        _pMemory->m_fnFixedVectorErase_(_pAllocator, _uiIndex);
    }

    template <typename Type>
    static void fixed_vector_erase(ftable* _pMemory, fixed_vector_allocator* _pAllocator, uint32_t _uiIndex)
    {
        Type* _object = fixed_vector_at<Type>(_pMemory, _pAllocator, _uiIndex);

        _object->~Type();

        _pMemory->m_fnFixedVectorErase_(_pAllocator, _uiIndex);
    }

    static void fixed_vector_clear(ftable* _pMemory, fixed_vector_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedVectorClear_(_pAllocator);
    }

}}