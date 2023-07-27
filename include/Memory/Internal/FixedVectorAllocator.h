#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_vector_allocate(fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    void* impl_fixed_vector_allocate(fixed_vector_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* impl_fixed_vector_at(fixed_vector_allocator* _pAllocator, std::size_t _ullIndex);
    void* impl_fixed_vector_begin(fixed_vector_allocator* _pAllocator);
    void* impl_fixed_vector_back(fixed_vector_allocator* _pAllocator);
    void* impl_fixed_vector_end(fixed_vector_allocator* _pAllocator);
    std::size_t impl_fixed_vector_size(fixed_vector_allocator* _pAllocator);
    std::size_t impl_fixed_vector_capacity(fixed_vector_allocator* _pAllocator);
    bool impl_fixed_vector_empty(fixed_vector_allocator* _pAllocator);
    bool impl_fixed_vector_full(fixed_vector_allocator* _pAllocator);
    void impl_fixed_vector_resize(const void* _pInterface, free_list_allocator* _pParentAllocator, fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize);
    void impl_fixed_vector_erase(fixed_vector_allocator* _pAllocator, uint32_t _uiIndex);
    void impl_fixed_vector_copy(fixed_vector_allocator* _pSourceAllocator, fixed_vector_allocator* _pTargetAllocator, void (*_fnCopy)(fixed_vector_allocator* _pTargetAllocator, void* _pValue));

    void impl_fixed_vector_clear(fixed_vector_allocator* _pAllocator);

}}