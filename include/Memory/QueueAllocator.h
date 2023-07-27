#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type, typename... Args>
    static inline Type* queue_emplace_back(ftable* _pMemory, queue_allocator* _pAllocator, Args&&... _vData)
    {
        return new(_pMemory->m_queueContainer.m_fnAllocateSize(_pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Args>(_vData)...);
    }

    template <typename Type>
    static inline Type* queue_allocate(ftable* _pMemory, queue_allocator* _pAllocator, const Type& _data)
    {
        if constexpr (std::is_trivially_copy_constructible<Type>::value)
        {
            return static_cast<Type*>(_pMemory->m_queueContainer.m_fnAllocate(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        }
        else if constexpr (std::is_copy_constructible<Type>::value)
        {
            return queue_emplace_back<Type>(_pMemory, _pAllocator, _data);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename Type>
    static inline Type* queue_allocate(ftable* _pMemory, queue_allocator* _pAllocator, Type* _pData)
    {
        return static_cast<Type*>(_pMemory->m_queueContainer.m_fnAllocate(_pAllocator, _pData, sizeof(Type), alignof(Type)));

        // if constexpr (std::is_trivially_copy_constructible<Type>::value)
        // {
        //     return static_cast<Type*>(_pMemory->m_fnAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
        // }
        // else if constexpr (std::is_copy_constructible<Type>::value)
        // {
        //     return queue_emplace_back<Type>(_pMemory, _pAllocator, _data);
        // }
        // else
        // {
        //     return nullptr;
        // }
    }

    template <typename Type>
    static inline Type* queue_allocate(ftable* _pMemory, queue_allocator* _pAllocator, Type&& _data)
    {
        return queue_emplace_back<Type>(_pMemory, _pAllocator, std::move(_data));
    }

    static inline void* queue_begin(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        return _pMemory->m_queueContainer.m_fnBegin(_pAllocator);
    }

    template <typename Type>
    static inline Type* queue_begin(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        return static_cast<Type*>(queue_begin(_pMemory, _pAllocator));
    }

    static inline void queue_pop(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        _pMemory->m_queueContainer.m_fnPop(_pAllocator);
    }

    template <typename Type>
    static inline void queue_pop(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        Type* _object = queue_begin<Type>(_pMemory, _pAllocator);

        _object->~Type();

        queue_pop(_pMemory, _pAllocator);
    }

    static inline bool queue_empty(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        return _pMemory->m_queueContainer.m_fnEmpty(_pAllocator);
    }

    static inline bool queue_full(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        return _pMemory->m_queueContainer.m_fnFull(_pAllocator);
    }

    static inline void queue_resize(ftable* _pMemory, free_list_allocator* m_pParentAllocator, queue_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_queueContainer.m_fnResize(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

    static inline std::size_t queue_size(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        return _pMemory->m_queueContainer.m_fnSize(_pAllocator);
    }

    static inline std::size_t queue_capacity(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        return _pMemory->m_queueContainer.m_fnCapacity(_pAllocator);
    }

    static inline void queue_clear(ftable* _pMemory, queue_allocator* _pAllocator)
    {
        _pMemory->m_queueContainer.m_fnClear(_pAllocator);
    }

}}