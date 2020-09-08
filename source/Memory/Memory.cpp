#include "Memory/Memory.h"

#include "Memory/Allocator.h"

namespace Duckvil { namespace Memory {



}}

Duckvil::Memory::IMemory* duckvil_memory_init()
{
    Duckvil::Memory::IMemory* memory = (Duckvil::Memory::IMemory*)malloc(sizeof(Duckvil::Memory::IMemory));

    memory->m_fnBasicAllocate = &Duckvil::Memory::allocate;
    memory->m_fnLinearAllocate = &Duckvil::Memory::allocate_linear;

    return memory;
}