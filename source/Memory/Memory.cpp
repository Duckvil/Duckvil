#include "Memory/Memory.h"

#include "Memory/Internal/Allocator.h"
#include "Memory/Internal/LinearAllocator.h"
#include "Memory/Internal/FixedStackAllocator.h"
#include "Memory/Internal/FixedQueueAllocator.h"
#include "Memory/Internal/FixedArrayAllocator.h"
#include "Memory/Internal/FreeListAllocator.h"
#include "Memory/Internal/FixedVectorAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    uintptr_t calculate_aligned_pointer(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset)
    {
        uint8_t _padding = _ucAlignment - 1;
        uintptr_t _new_address = (_ullAddress + _padding) & ~_padding;

        _ucPaddedOffset = _new_address - _ullAddress;

        return _new_address;
    }

    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment)
    {
        uint8_t _padding = _ucAlignment - 1;
        uintptr_t _new_address = (_ullAddress + _padding) & ~_padding;

        return _new_address - _ullAddress;
    }

    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t _ucHeaderSize)
    {
        uint8_t _padding = calculate_padding(_ullAddress, _ucAlignment);
        uint8_t _needed_space = _ucHeaderSize;

        if(_padding < _needed_space)
        {
            _needed_space -= _padding;
            _padding += _ucAlignment * (_needed_space / _ucAlignment);

            if(_needed_space % _ucAlignment > 0)
            {
                _padding += _ucAlignment;
            }
        }

        return _padding;
    }

    void* calculate_aligned_pointer(const void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset)
    {
        return reinterpret_cast<void*>(calculate_aligned_pointer(reinterpret_cast<uintptr_t>(_p), _ucAlignment, _ucPaddedOffset));
    }

    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment)
    {
        return calculate_padding(reinterpret_cast<uintptr_t>(_p), _ucAlignment);
    }

    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment, uint8_t _ucHeaderSize)
    {
        return calculate_padding(reinterpret_cast<uintptr_t>(_p), _ucAlignment, _ucHeaderSize);
    }

}}

Duckvil::Memory::IMemory* duckvil_memory_init()
{
    using namespace Duckvil::Memory;

    IMemory* memory = (IMemory*)malloc(sizeof(IMemory));

    memory->m_fnBasicAllocate = &impl_allocate;

    memory->m_fnCalculateAlignedPointer =   &calculate_aligned_pointer;
    memory->m_fnCalculatePadding =          &calculate_padding;
    memory->m_fnCalculatePaddingH =         &calculate_padding;
    memory->m_fnCalculateAlignedPointer_ =  &calculate_aligned_pointer;
    memory->m_fnCalculatePadding_ =         &calculate_padding;
    memory->m_fnCalculatePaddingH_ =        &calculate_padding;

    memory->m_fnLinearAllocate =    &impl_linear_allocate;
    memory->m_fnLinearClear =       &impl_linear_clear;
    memory->m_fnLinearAllocate_ =   &impl_linear_allocate;
    memory->m_fnLinearClear_ =      &impl_linear_clear;

    memory->m_fnFixedStackAllocate_ =   &impl_fixed_stack_allocate;
    memory->m_fnFixedStackTop_ =        &impl_fixed_stack_allocator_top;
    memory->m_fnFixedStackPop_ =        &impl_fixed_stack_allocator_pop;
    memory->m_fnFixedStackEmpty_ =      &impl_fixed_stack_allocator_empty;
    memory->m_fnFixedStackFull_ =       &impl_fixed_stack_allocator_full;
    memory->m_fnFixedStackClear_ =      &impl_fixed_stack_clear;
    memory->m_fnFixedStackSize_ =       &impl_fixed_stack_allocator_size;
    memory->m_fnFixedStackCapacity_ =   &impl_fixed_stack_allocator_capacity;

    memory->m_fnFixedQueueAllocate_ =   &impl_fixed_queue_allocate;
    memory->m_fnFixedQueueBegin_ =      &impl_fixed_queue_begin;
    memory->m_fnFixedQueuePop_ =        &impl_fixed_queue_pop;
    memory->m_fnFixedQueueEmpty_ =      &impl_fixed_queue_empty;
    memory->m_fnFixedQueueFull_ =       &impl_fixed_queue_full;
    memory->m_fnFixedQueueClear_ =      &impl_fixed_queue_clear;
    memory->m_fnFixedQueueSize_ =       &impl_fixed_queue_size;
    memory->m_fnFixedQueueCapacity_ =   &impl_fixed_queue_capacity;

    memory->m_fnFixedArrayAllocate_ =   &impl_fixed_array_allocate;
    memory->m_fnFixedArrayBegin_ =      &impl_fixed_array_begin;
    memory->m_fnFixedArrayBack_ =       &impl_fixed_array_back;
    memory->m_fnFixedArrayAt_ =         &impl_fixed_array_at;
    memory->m_fnFixedArrayEmpty_ =      &impl_fixed_array_empty;
    memory->m_fnFixedArrayFull_ =       &impl_fixed_array_full;
    memory->m_fnFixedArraySize_ =       &impl_fixed_array_size;
    memory->m_fnFixedArrayClear_ =      &impl_fixed_array_clear;

    memory->m_fnFreeListAllocate_ =     &impl_free_list_allocate;
    memory->m_fnFreeListAllocate_D_ =   &impl_free_list_allocate;
    memory->m_fnFreeListReallocate_ =   &impl_free_list_reallocate;
    memory->m_fnFreeListFree_ =         &impl_free_list_free;
    memory->m_fnFreeListClear_ =        &impl_free_list_clear;

    memory->m_fnFixedVectorAllocate_ =  &impl_fixed_vector_allocate;
    memory->m_fnFixedVectorBegin_ =     &impl_fixed_vector_begin;
    memory->m_fnFixedVectorBack_ =      &impl_fixed_vector_back;
    memory->m_fnFixedVectorEnd_ =       &impl_fixed_vector_end;
    memory->m_fnFixedVectorAt_ =        &impl_fixed_vector_at;
    memory->m_fnFixedVectorEmpty_ =     &impl_fixed_vector_empty;
    memory->m_fnFixedVectorFull_ =      &impl_fixed_vector_full;
    memory->m_fnFixedVectorClear_ =     &impl_fixed_vector_clear;
    memory->m_fnFixedVectorResize_ =    &impl_fixed_vector_resize;
    memory->m_fnFixedVectorSize_ =      &impl_fixed_vector_size;
    memory->m_fnFixedVectorCapacity_ =  &impl_fixed_vector_capacity;
    memory->m_fnFixedVectorErase_ =     &impl_fixed_vector_erase;

    memory->m_fnLinearAllocateLinearAllocator =         &impl_linear_allocate_linear_allocator;
    memory->m_fnLinearAllocateFixedStackAllocator =     &impl_linear_allocate_fixed_stack_allocator;
    memory->m_fnLinearAllocateFixedArrayAllocator =     &impl_linear_allocate_fixed_array_allocator;
    memory->m_fnLinearAllocateFreeListAllocator =       &impl_linear_allocate_free_list_allocator;
    memory->m_fnLinearAllocateFixedVectorAllocator =    &impl_linear_allocate_fixed_vector_allocator;

    memory->m_fnFreeListAllocateFixedQueueAllocator =   &impl_free_list_allocate_fixed_queue_allocator;
    memory->m_fnFreeListAllocateFreeListAllocator =     &impl_free_list_allocate_free_list_allocator;
    memory->m_fnFreeListAllocateFixedVectorAllocator =  &impl_free_list_allocate_fixed_vector_allocator;
    memory->m_fnFreeListAllocateFixedStackAllocator =   &impl_free_list_allocate_fixed_stack_allocator;
    memory->m_fnFreeListAllocateFixedArrayAllocator =   &impl_free_list_allocate_fixed_array_allocator;

    return memory;
}