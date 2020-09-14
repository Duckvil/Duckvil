#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    struct __free_list_header
    {
        uint8_t m_ucPadding = 0;
        std::size_t m_ullSize = 0;
    };

    struct __free_list_node
    {
        __free_list_node* m_pNext = nullptr;
        std::size_t m_ullSize = 0;
    };

    void* free_list_allocate(__free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    const char* free_list_allocate(__free_list_allocator* _pAllocator, const char* _pData);
    void free_list_clear(__free_list_allocator* _pAllocator);

}}