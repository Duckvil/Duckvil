#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    void* fixed_vector_allocate(__fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);

    void* fixed_vector_at(__fixed_vector_allocator* _pAllocator, std::size_t _ullIndex);
    void* fixed_vector_begin(__fixed_vector_allocator* _pAllocator);
    void* fixed_vector_back(__fixed_vector_allocator* _pAllocator);
    std::size_t fixed_vector_size(__fixed_vector_allocator* _pAllocator);
    std::size_t fixed_vector_capacity(__fixed_vector_allocator* _pAllocator);
    bool fixed_vector_empty(__fixed_vector_allocator* _pAllocator);
    bool fixed_vector_full(__fixed_vector_allocator* _pAllocator);
    void fixed_vector_resize(IMemory* _pInterface, __free_list_allocator* _pParentAllocator, __fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize);
    void fixed_vector_erase(IMemory* _pInterface, __free_list_allocator* _pParentAllocator, __fixed_vector_allocator** _pAllocator, uint32_t _uiIndex);

    void fixed_vector_clear(__fixed_vector_allocator* _pAllocator);

}}