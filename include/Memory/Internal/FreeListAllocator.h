#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    struct __free_list_header
    {
        uint8_t m_ucPadding = 0; // How much add to reach node
        std::size_t m_ullSize = 0; // Total size = sizeof(__free_list_header) + padding + __free_list_node + data_size
    };

    struct __free_list_node
    {
        __free_list_node* m_pNext = nullptr;
        std::size_t m_ullSize = 0;
    };

    void* free_list_allocate(__free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* free_list_allocate(__free_list_allocator* _pAllocator, const char* _pData, std::size_t _ullLength);
    void free_list_free(__free_list_allocator* _pAllocator, void** _pointer);
    void free_list_clear(__free_list_allocator* _pAllocator);

    __fixed_vector_allocator* free_list_allocate_vector_allocator(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize, uint8_t _ucAlignment);

}}