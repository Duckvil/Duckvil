#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    // template <typename Type>
    // static inline Type* fixed_queue_allocate(ftable* _pMemory, fixed_queue_allocator* _pAllocator, const Type& _pData)
    // {
    //     return static_cast<Type*>(_pMemory->m_fnFixedQueueAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type)));
    // }

    // template <typename Type>
    // static inline Type* fixed_queue_allocate(ftable* _pMemory, fixed_queue_allocator* _pAllocator, Type&& _data)
    // {
    //     return new(_pMemory->m_fnFixedQueueAllocateSize_(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
    // }

    template <typename Type, typename... Args>
    static inline Type* fixed_queue_emplace_back(const ftable* _pMemory, fixed_queue_allocator* _pAllocator, Args&&... _vData)
    {
        return new(_pMemory->m_fixedQueueContainer.m_fnAllocateSize(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* fixed_queue_allocate(const ftable* _pMemory, fixed_queue_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_fixedQueueContainer.m_fnAllocate(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return fixed_queue_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* fixed_queue_allocate(const ftable* _pMemory, fixed_queue_allocator* _pAllocator, Type&& _data)
    {
        return fixed_queue_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }

    static inline void* fixed_queue_begin(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fixedQueueContainer.m_fnBegin(_pAllocator);
    }

    template <typename Type>
    static inline Type* fixed_queue_begin(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return static_cast<Type*>(fixed_queue_begin(_pMemory, _pAllocator));
    }

    static inline void fixed_queue_pop(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        _pMemory->m_fixedQueueContainer.m_fnPop(_pAllocator);
    }

    template <typename Type>
    static inline void fixed_queue_pop(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        Type* _object = fixed_queue_begin<Type>(_pMemory, _pAllocator);

        _object->~Type();

        fixed_queue_pop(_pMemory, _pAllocator);
    }

    static inline bool fixed_queue_empty(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fixedQueueContainer.m_fnEmpty(_pAllocator);
    }

    static inline bool fixed_queue_full(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fixedQueueContainer.m_fnFull(_pAllocator);
    }

    static inline void fixed_queue_resize(const ftable* _pMemory, free_list_allocator* m_pParentAllocator, fixed_queue_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_fixedQueueContainer.m_fnResize(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

    static inline std::size_t fixed_queue_size(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fixedQueueContainer.m_fnSize(_pAllocator);
    }

    static inline std::size_t fixed_queue_capacity(const ftable* _pMemory, fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fixedQueueContainer.m_fnCapacity(_pAllocator);
    }

}}