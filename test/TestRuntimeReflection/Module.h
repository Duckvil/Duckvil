#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Memory.h"

struct __duckvil_global
{
    static Duckvil::PlugNPlay::__module m_module;
    static Duckvil::PlugNPlay::__module_information m_memoryModule;
    static Duckvil::Memory::IMemory* m_pMemoryInterface;
    static void* m_pMemoryChunk;
    static void* m_pHeap;
};