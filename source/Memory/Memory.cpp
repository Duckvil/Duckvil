#include "Memory/Memory.h"

#include "Memory/Internal/Allocator.h"
#include "Memory/Internal/LinearAllocator.h"
#include "Memory/Internal/StackAllocator.h"
#include "Memory/Internal/FixedStackAllocator.h"
#include "Memory/Internal/QueueAllocator.h"
#include "Memory/Internal/FixedQueueAllocator.h"
#include "Memory/Internal/FixedArrayAllocator.h"
#include "Memory/Internal/FreeListAllocator.h"
#include "Memory/Internal/FixedVectorAllocator.h"
#include "Memory/Internal/ByteBuffer.h"

#include <memory>

namespace Duckvil { namespace Memory {

    uintptr_t calculate_aligned_pointer(const uintptr_t& _ullAddress, uint8_t _ucAlignment)
    {
        if(_ucAlignment == 0 || (_ucAlignment & (_ucAlignment - 1)) != 0)
        {
            throw "Alignment must be power of 2!";
        }

        uint8_t _padding = _ucAlignment - 1;
        uintptr_t _new_address = (_ullAddress + _padding) & ~_padding;

        return _new_address;
    }

    uintptr_t calculate_aligned_pointer(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset)
    {
        uintptr_t _new_address = calculate_aligned_pointer(_ullAddress, _ucAlignment);

        _ucPaddedOffset = _new_address - _ullAddress;

        return _new_address;
    }

    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment)
    {
        return calculate_aligned_pointer(_ullAddress, _ucAlignment) - _ullAddress;
    }

    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t _ucHeaderSize)
    {
        uint8_t _padding = calculate_padding(_ullAddress, _ucAlignment);

        if(_padding < _ucHeaderSize)
        {
            _ucHeaderSize -= _padding;
            _padding += _ucAlignment * (_ucHeaderSize / _ucAlignment);

            if(_ucHeaderSize % _ucAlignment > 0)
            {
                _padding += _ucAlignment;
            }
        }

        return _padding;
    }

    void* calculate_aligned_pointer(const void* _p, uint8_t _ucAlignment)
    {
        return reinterpret_cast<void*>(calculate_aligned_pointer(reinterpret_cast<uintptr_t>(_p), _ucAlignment));
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

const Duckvil::Memory::ftable* duckvil_memory_init()
{
    using namespace Duckvil::Memory;

    // ftable* memory = (ftable*)malloc(sizeof(ftable));

    static ftable memory = { 0 };

    memory.m_fnBasicAllocate =  &impl_allocate;
    memory.m_fnBasicFree =      &impl_free;

    memory.m_fnCalculateAlignedPointer =    &calculate_aligned_pointer;
    memory.m_fnCalculatePadding =           &calculate_padding;
    memory.m_fnCalculatePaddingH =          &calculate_padding;
    memory.m_fnCalculateAlignedPointer_ =   &calculate_aligned_pointer;
    memory.m_fnCalculatePadding_ =          &calculate_padding;
    memory.m_fnCalculatePaddingH_ =         &calculate_padding;

    memory.m_fnLinearAllocate = &impl_linear_allocate;
    memory.m_fnLinearClear =    &impl_linear_clear;

    memory.m_fnStackAllocate_ =        &impl_stack_allocate;
    memory.m_fnStackAllocateSize_ =    &impl_stack_allocate;
    memory.m_fnStackTop_ =             &impl_stack_allocator_top;
    memory.m_fnStackPop_ =             &impl_stack_allocator_pop;
    memory.m_fnStackEmpty_ =           &impl_stack_allocator_empty;
    memory.m_fnStackFull_ =            &impl_stack_allocator_full;
    memory.m_fnStackClear_ =           &impl_stack_clear;
    memory.m_fnStackSize_ =            &impl_stack_allocator_size;
    memory.m_fnStackCapacity_ =        &impl_stack_allocator_capacity;

    memory.m_fnFixedStackAllocate_ =        &impl_fixed_stack_allocate;
    memory.m_fnFixedStackAllocateSize_ =    &impl_fixed_stack_allocate;
    memory.m_fnFixedStackTop_ =             &impl_fixed_stack_allocator_top;
    memory.m_fnFixedStackPop_ =             &impl_fixed_stack_allocator_pop;
    memory.m_fnFixedStackEmpty_ =           &impl_fixed_stack_allocator_empty;
    memory.m_fnFixedStackFull_ =            &impl_fixed_stack_allocator_full;
    memory.m_fnFixedStackClear_ =           &impl_fixed_stack_clear;
    memory.m_fnFixedStackSize_ =            &impl_fixed_stack_allocator_size;
    memory.m_fnFixedStackCapacity_ =        &impl_fixed_stack_allocator_capacity;

    memory.m_fnQueueAllocate_ =        &impl_queue_allocate;
    memory.m_fnQueueAllocateSize_ =    &impl_queue_allocate;
    memory.m_fnQueueBegin_ =           &impl_queue_begin;
    memory.m_fnQueuePop_ =             &impl_queue_pop;
    memory.m_fnQueueEmpty_ =           &impl_queue_empty;
    memory.m_fnQueueFull_ =            &impl_queue_full;
    memory.m_fnQueueClear_ =           &impl_queue_clear;
    memory.m_fnQueueResize_ =          &impl_queue_resize;
    memory.m_fnQueueSize_ =            &impl_queue_size;
    memory.m_fnQueueCapacity_ =        &impl_queue_capacity;

    memory.m_fnFixedQueueAllocate_ =        &impl_fixed_queue_allocate;
    memory.m_fnFixedQueueAllocateSize_ =    &impl_fixed_queue_allocate;
    memory.m_fnFixedQueueBegin_ =           &impl_fixed_queue_begin;
    memory.m_fnFixedQueuePop_ =             &impl_fixed_queue_pop;
    memory.m_fnFixedQueueEmpty_ =           &impl_fixed_queue_empty;
    memory.m_fnFixedQueueFull_ =            &impl_fixed_queue_full;
    memory.m_fnFixedQueueClear_ =           &impl_fixed_queue_clear;
    memory.m_fnFixedQueueResize_ =          &impl_fixed_queue_resize;
    memory.m_fnFixedQueueSize_ =            &impl_fixed_queue_size;
    memory.m_fnFixedQueueCapacity_ =        &impl_fixed_queue_capacity;

    memory.m_fnFixedArrayAllocate_ =        &impl_fixed_array_allocate;
    memory.m_fnFixedArrayAllocateSize_ =    &impl_fixed_array_allocate;
    memory.m_fnFixedArrayBegin_ =           &impl_fixed_array_begin;
    memory.m_fnFixedArrayBack_ =            &impl_fixed_array_back;
    memory.m_fnFixedArrayAt_ =              &impl_fixed_array_at;
    memory.m_fnFixedArrayEmpty_ =           &impl_fixed_array_empty;
    memory.m_fnFixedArrayFull_ =            &impl_fixed_array_full;
    memory.m_fnFixedArraySize_ =            &impl_fixed_array_size;
    memory.m_fnFixedArrayClear_ =           &impl_fixed_array_clear;

    memory.m_fnFreeListAllocate_ =      &impl_free_list_allocate;
    memory.m_fnFreeListAllocate_D_ =    &impl_free_list_allocate;
    memory.m_fnFreeListReallocate_ =    &impl_free_list_reallocate;
    memory.m_fnFreeListFree_ =          &impl_free_list_free;
    memory.m_fnFreeListClear_ =         &impl_free_list_clear;

    memory.m_fnFixedVectorAllocate_ =       &impl_fixed_vector_allocate;
    memory.m_fnFixedVectorAllocateSize_ =   &impl_fixed_vector_allocate;
    memory.m_fnFixedVectorBegin_ =          &impl_fixed_vector_begin;
    memory.m_fnFixedVectorBack_ =           &impl_fixed_vector_back;
    memory.m_fnFixedVectorEnd_ =            &impl_fixed_vector_end;
    memory.m_fnFixedVectorAt_ =             &impl_fixed_vector_at;
    memory.m_fnFixedVectorEmpty_ =          &impl_fixed_vector_empty;
    memory.m_fnFixedVectorFull_ =           &impl_fixed_vector_full;
    memory.m_fnFixedVectorClear_ =          &impl_fixed_vector_clear;
    memory.m_fnFixedVectorResize_ =         &impl_fixed_vector_resize;
    memory.m_fnFixedVectorSize_ =           &impl_fixed_vector_size;
    memory.m_fnFixedVectorCapacity_ =       &impl_fixed_vector_capacity;
    memory.m_fnFixedVectorErase_ =          &impl_fixed_vector_erase;
    memory.m_fnFixedVectorCopy_ =           &impl_fixed_vector_copy;

    memory.m_fnByteBufferWillFit_ =     &impl_byte_buffer_will_fit;
    memory.m_fnByteBufferClear_ =       &impl_byte_buffer_clear;
    memory.m_fnByteBufferResize_ =      &impl_byte_buffer_resize;
    memory.m_fnByteBufferSeekToBegin_ = &impl_byte_buffer_seek_to_begin;
    memory.m_fnByteBufferSeekToEnd_ =   &impl_byte_buffer_seek_to_end;
    memory.m_fnByteBufferAdvance_ =     &impl_byte_buffer_advance;
    memory.m_fnByteBufferWrite_ =       &impl_byte_buffer_write;
    memory.m_fnByteBufferRead_ =        &impl_byte_buffer_read;

    memory.m_fnLinearAllocateLinearAllocator =          &impl_linear_allocate_linear_allocator;
    memory.m_fnLinearAllocateFixedStackAllocator =      &impl_linear_allocate_fixed_stack_allocator;
    memory.m_fnLinearAllocateFixedArrayAllocator =      &impl_linear_allocate_fixed_array_allocator;
    memory.m_fnLinearAllocateFreeListAllocator =        &impl_linear_allocate_free_list_allocator;
    memory.m_fnLinearAllocateFixedVectorAllocator =     &impl_linear_allocate_fixed_vector_allocator;
    memory.m_fnLinearAllocateByteBufferAllocator =      &impl_linear_allocate_byte_buffer_allocator;

    memory.m_fnFreeListAllocateQueueAllocator =         &impl_free_list_allocate_queue_allocator;
    memory.m_fnFreeListAllocateFixedQueueAllocator =    &impl_free_list_allocate_fixed_queue_allocator;
    memory.m_fnFreeListAllocateFreeListAllocator =      &impl_free_list_allocate_free_list_allocator;
    memory.m_fnFreeListAllocateFixedVectorAllocator =   &impl_free_list_allocate_fixed_vector_allocator;
    memory.m_fnFreeListAllocateFixedStackAllocator =    &impl_free_list_allocate_fixed_stack_allocator;
    memory.m_fnFreeListAllocateFixedArrayAllocator =    &impl_free_list_allocate_fixed_array_allocator;
    memory.m_fnFreeListAllocateByteBufferAllocator =    &impl_free_list_allocate_byte_buffer_allocator;

    return &memory;
}