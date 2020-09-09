#include "Memory/Memory.h"

#include "Memory/Internal/Allocator.h"
#include "Memory/Internal/LinearAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {



}}

Duckvil::Memory::IMemory* duckvil_memory_init()
{
    Duckvil::Memory::IMemory* memory = (Duckvil::Memory::IMemory*)malloc(sizeof(Duckvil::Memory::IMemory));

    memory->m_fnBasicAllocate = &Duckvil::Memory::allocate;

    memory->m_fnLinearAllocate = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearAllocateCStr = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearClear = &Duckvil::Memory::linear_clear;

    memory->m_fnAllocateLinearAllocator = &Duckvil::Memory::allocate_linear_allocator;

    memory->m_fnLinearAllocate_ = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearAllocateCStr_ = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearClear_ = &Duckvil::Memory::linear_clear;

    return memory;
}