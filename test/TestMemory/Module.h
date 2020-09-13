#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Memory.h"

struct __duckvil_global
{
    static Duckvil::PlugNPlay::module m_module;
    static Duckvil::PlugNPlay::__module_information m_memoryModule;
    static Duckvil::Memory::IMemory* m_pMemoryInterface;
    // static Duckvil::Memory::__linear_allocator m_memoryChunk;
    static void* m_memoryChunk;
};