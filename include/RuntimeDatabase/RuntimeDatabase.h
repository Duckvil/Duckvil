#pragma once

#include "Memory/Memory.h"

#include "Utils/Macro.h"

namespace Duckvil { namespace RuntimeDatabase {

    struct __data
    {
        Duckvil::Memory::__free_list_allocator* m_pAllocator;
    };

}}

DUCKVIL_EXPORT Duckvil::RuntimeDatabase::__data* duckcvil_runtimedatabase_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);