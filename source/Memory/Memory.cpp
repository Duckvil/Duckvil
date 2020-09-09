#include "Memory/Memory.h"

#include "Memory/Internal/Allocator.h"
#include "Memory/Internal/LinearAllocator.h"
#include "Memory/Internal/StackAllocator.h"
#include "Memory/Internal/FixedStackAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    void* calculate_aligned_pointer(void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset)
    {
        uintptr_t _memory_address = reinterpret_cast<uintptr_t>(_p);
        uint8_t _padding = _ucAlignment - 1;
        uintptr_t _new_address = (_memory_address + _padding) & ~_padding;

        _ucPaddedOffset = _new_address - _memory_address;

        return reinterpret_cast<void*>(_new_address);
    }

}}

Duckvil::Memory::IMemory* duckvil_memory_init()
{
    using namespace Duckvil::Memory;

    IMemory* memory = (IMemory*)malloc(sizeof(IMemory));

    memory->m_fnBasicAllocate = &allocate;

    memory->m_fnLinearAllocate = &linear_allocate;
    memory->m_fnLinearAllocateCStr = &linear_allocate;
    memory->m_fnLinearClear = &linear_clear;
    memory->m_fnLinearAllocate_ = &linear_allocate;
    memory->m_fnLinearAllocateCStr_ = &linear_allocate;
    memory->m_fnLinearClear_ = &linear_clear;

    memory->m_fnFixedStackAllocate_ = &fixed_stack_allocate;
    memory->m_fnFixedStackAllocateCStr_ = &fixed_stack_allocate;
    memory->m_fnFixedStackTop_ = &fixed_stack_allocator_top;
    memory->m_fnFixedStackPop_ = &fixed_stack_allocator_pop;
    memory->m_fnFixedStackClear_ = &fixed_stack_clear;

    memory->m_fnStackAllocate_ = &stack_allocate;
    memory->m_fnStackAllocateCStr_ = &stack_allocate;
    memory->m_fnStackTop_ = &stack_allocator_top;
    memory->m_fnStackPop_ = &stack_allocator_pop;
    memory->m_fnStackClear_ = &stack_clear;

    memory->m_fnAllocateLinearAllocator = &allocate_linear_allocator;
    memory->m_fnAllocateFixedStackAllocator = &allocate_fixed_stack_allocator;
    memory->m_fnAllocateStackAllocator = &allocate_stack_allocator;

    return memory;
}