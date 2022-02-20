#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, const Type& _data)
    {
        return static_cast<Type*>(_pMemory->m_fnFreeListAllocate_D_(_pAllocator, &_data, sizeof(Type), alignof(Type)));
    }

    template <typename Type>
    static inline Type* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, Type&& _data)
    {
        return new(free_list_allocate(_pMemory, _pAllocator, sizeof(Type), alignof(Type))) Type(std::forward<Type>(_data));
    }

    static inline void* free_list_allocate(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        return _pMemory->m_fnFreeListAllocate_(_pAllocator, _ullSize, _ucAlignment);
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

        // TODO: Call destructor?

        _pMemory->m_fnFreeListFree_(_pAllocator, _pPointer);
    }

}}