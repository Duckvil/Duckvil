#include "Memory/Memory.h"

#include "Memory/Internal/Allocator.h"
#include "Memory/Internal/LinearAllocator.h"
#include "Memory/Internal/StackAllocator.h"
#include "Memory/Internal/FixedStackAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    void* calculate_aligned_pointer(void* _p, uint8_t _ucAlignment, uint8_t& _padded_offset)
    {
        uintptr_t _memory_address = reinterpret_cast<uintptr_t>(_p);
        uint8_t _padding = _ucAlignment - 1;
        uintptr_t _new_address = (_memory_address + _padding) & ~_padding;

        _padded_offset = _new_address - _memory_address;

        return reinterpret_cast<void*>(_new_address);
    }

}}

Duckvil::Memory::IMemory* duckvil_memory_init()
{
    Duckvil::Memory::IMemory* memory = (Duckvil::Memory::IMemory*)malloc(sizeof(Duckvil::Memory::IMemory));

    memory->m_fnBasicAllocate = &Duckvil::Memory::allocate;

    memory->m_fnLinearAllocate = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearAllocateCStr = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearClear = &Duckvil::Memory::linear_clear;
    memory->m_fnLinearAllocate_ = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearAllocateCStr_ = &Duckvil::Memory::linear_allocate;
    memory->m_fnLinearClear_ = &Duckvil::Memory::linear_clear;

    memory->m_fnFixedStackAllocate_ = &Duckvil::Memory::fixed_stack_allocate;
    memory->m_fnFixedStackAllocateCStr_ = &Duckvil::Memory::fixed_stack_allocate;
    memory->m_fnFixedStackTop_ = &Duckvil::Memory::fixed_stack_allocator_top;
    memory->m_fnFixedStackPop_ = &Duckvil::Memory::fixed_stack_allocator_pop;
    memory->m_fnFixedStackClear_ = &Duckvil::Memory::fixed_stack_clear;

    memory->m_fnStackAllocate_ = &Duckvil::Memory::stack_allocate;
    memory->m_fnStackAllocateCStr_ = &Duckvil::Memory::stack_allocate;
    memory->m_fnStackTop_ = &Duckvil::Memory::stack_allocator_top;
    memory->m_fnStackPop_ = &Duckvil::Memory::stack_allocator_pop;
    memory->m_fnStackClear_ = &Duckvil::Memory::stack_clear;

    memory->m_fnAllocateLinearAllocator = &Duckvil::Memory::allocate_linear_allocator;
    memory->m_fnAllocateFixedStackAllocator = &Duckvil::Memory::allocate_fixed_stack_allocator;
    memory->m_fnAllocateStackAllocator = &Duckvil::Memory::allocate_stack_allocator;

    return memory;
}