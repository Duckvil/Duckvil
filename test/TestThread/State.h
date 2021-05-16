#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Memory.h"

struct duckvil_global
{
    static Duckvil::PlugNPlay::__module m_module;
    static Duckvil::PlugNPlay::__module_information m_memoryModule;
    static Duckvil::Memory::ftable* m_pMemoryInterface;
    static Duckvil::Memory::linear_allocator* m_pMemoryChunk;
    static Duckvil::Memory::free_list_allocator* m_pHeap;
};