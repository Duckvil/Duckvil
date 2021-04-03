#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    struct Container
    {
        Container()
        {
            m_pMemoryInterface = nullptr;
            m_pAllocator = nullptr;
        }

        Container(ftable* _pMemory, allocator* _pAllocator) :
            m_pMemoryInterface(_pMemory),
            m_pAllocator(_pAllocator)
        {

        }

        ftable* m_pMemoryInterface;
        allocator* m_pAllocator;
    };

}}