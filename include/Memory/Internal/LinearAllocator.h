#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    void* impl_linear_allocate(linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment = 8);
    void impl_linear_clear(linear_allocator* _pAllocator);

}}