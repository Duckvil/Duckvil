#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    static Type* free_list_allocate(IMemory* _pMemory, __free_list_allocator* _pAllocator, const Type& _data)
    {
        return (Type*)_pMemory->m_fnFreeListAllocate_D_(_pAllocator, &_data, sizeof(Type), alignof(Type));
    }

    static void* free_list_allocate(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        return _pMemory->m_fnFreeListAllocate_(_pAllocator, _ullSize, _ucAlignment);
    }

    static void free_list_free(IMemory* _pMemory, __free_list_allocator* _pAllocator, void* _pointer)
    {
        _pMemory->m_fnFreeListFree_(_pAllocator, _pointer);
    }

    // template <typename Allocator>
    // static __fixed_vector_allocator* free_list_allocate_allocator(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize, uint8_t _ucAlignment)
    // {
    //     return nullptr;
    // }

    // template <>
    // static __fixed_vector_allocator* free_list_allocate_allocator<__fixed_vector_allocator>(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize, uint8_t _ucAlignment)
    // {
    //     __fixed_vector_allocator* _vector_allocator = (__fixed_vector_allocator*)free_list_allocate(_pMemoryInterface, _pAllocator, sizeof(__fixed_vector_allocator) + _ullSize, _ucAlignment);

    //     _vector_allocator->m_ullCapacity = _ullSize;
    //     _vector_allocator->m_ullUsed = 0;
    //     _vector_allocator->m_ullBlockSize = _ullTypeSize;
    //     _vector_allocator->m_pMemory = (uint8_t*)_vector_allocator + sizeof(__fixed_vector_allocator);

    //     return _vector_allocator;
    // }

}}