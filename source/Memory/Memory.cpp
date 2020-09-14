#include "Memory/Memory.h"

#include "Memory/Internal/Allocator.h"
#include "Memory/Internal/LinearAllocator.h"
#include "Memory/Internal/StackAllocator.h"
#include "Memory/Internal/FixedStackAllocator.h"
#include "Memory/Internal/FixedQueueAllocator.h"
#include "Memory/Internal/QueueAllocator.h"
#include "Memory/Internal/FixedArrayAllocator.h"

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
    memory->m_fnFixedStackEmpty_ = &fixed_stack_allocator_empty;
    memory->m_fnFixedStackFull_ = &fixed_stack_allocator_full;
    memory->m_fnFixedStackClear_ = &fixed_stack_clear;

    memory->m_fnStackAllocate_ = &stack_allocate;
    memory->m_fnStackAllocateCStr_ = &stack_allocate;
    memory->m_fnStackTop_ = &stack_allocator_top;
    memory->m_fnStackPop_ = &stack_allocator_pop;
    memory->m_fnStackEmpty_ = &stack_allocator_empty;
    memory->m_fnStackFull_ = &stack_allocator_full;
    memory->m_fnStackClear_ = &stack_clear;

    memory->m_fnFixedQueueAllocate_ = &fixed_queue_allocate;
    memory->m_fnFixedQueueAllocateCStr_ = &fixed_queue_allocate;
    memory->m_fnFixedQueueBegin_ = &fixed_queue_begin;
    memory->m_fnFixedQueuePop_ = &fixed_queue_pop;
    memory->m_fnFixedQueueEmpty_ = &fixed_queue_empty;
    memory->m_fnFixedQueueFull_ = &fixed_queue_full;
    memory->m_fnFixedQueueClear_ = &fixed_queue_clear;

    memory->m_fnQueueAllocate_ = &queue_allocate;
    memory->m_fnQueueAllocateCStr_ = &queue_allocate;
    memory->m_fnQueueBegin_ = &queue_begin;
    memory->m_fnQueuePop_ = &queue_pop;
    memory->m_fnQueueEmpty_ = &queue_empty;
    memory->m_fnQueueFull_ = &queue_full;
    memory->m_fnQueueClear_ = &queue_clear;

    memory->m_fnFixedArrayAllocate_ = &fixed_array_allocate;
    memory->m_fnFixedArrayAllocateCStr_ = &fixed_array_allocate;
    memory->m_fnFixedArrayBegin_ = &fixed_array_begin;
    memory->m_fnFixedArrayBack_ = &fixed_array_back;
    memory->m_fnFixedArrayAt_ = &fixed_array_at;
    memory->m_fnFixedArrayEmpty_ = &fixed_array_empty;
    memory->m_fnFixedArrayFull_ = &fixed_array_full;
    memory->m_fnFixedArrayClear_ = &fixed_array_clear;

    memory->m_fnAllocateLinearAllocator = &allocate_linear_allocator;
    memory->m_fnAllocateFixedStackAllocator = &allocate_fixed_stack_allocator;
    memory->m_fnAllocateStackAllocator = &allocate_stack_allocator;
    memory->m_fnAllocateFixedQueueAllocator = &allocate_fixed_queue_allocator;
    memory->m_fnAllocateQueueAllocator = &allocate_queue_allocator;
    memory->m_fnAllocateFixedArrayAllocator = &allocate_fixed_array_allocator;

    return memory;
}