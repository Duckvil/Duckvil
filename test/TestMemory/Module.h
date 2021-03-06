#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Memory.h"
#include "Memory/FreeList.h"

struct __duckvil_global
{
    static Duckvil::PlugNPlay::__module m_module;
    static Duckvil::PlugNPlay::__module_information m_memoryModule;
    static Duckvil::Memory::ftable* m_pMemoryInterface;
    static void* m_pMemoryChunk;
    static void* m_pHeap;
    static Duckvil::Memory::FreeList m_freeList;
};