#pragma once

#include <cstdint>
#include <limits>
#include <vector>

#include "Utils/Macro.h"

#include "Memory/MemoryDebugger.h"

namespace Duckvil { namespace Memory {

    struct allocator
    {
#ifdef DUCKVIL_MEMORY_DEBUGGER
        void (*m_fnOnAllocate)(allocator* _pParentAllocator, allocator* _pAllocator, duckvil_memory_allocator_type _type) = 0;
        void (*m_fnOnDeallocate)(allocator* _pParentAllocator, allocator* _pAllocator) = 0;
        duckvil_memory_debug_info* m_pDebugData;
#endif

        std::size_t m_ullCapacity = 0;
        std::size_t m_ullUsed = 0;
    };

    struct linear_allocator : public allocator
    {

    };

    struct fixed_stack_allocator : public allocator
    {
        std::size_t m_ullBlockSize;
    };

    struct fixed_queue_allocator : public allocator
    {
        std::size_t m_ullBlockSize;
        std::size_t m_ullTail;
        std::size_t m_ullHead;
    };

    struct fixed_array_allocator : public allocator
    {
        std::size_t m_ullBlockSize;
    };

    struct __free_list_node;

    struct free_list_allocator : public allocator
    {
        __free_list_node* m_pHead;
    };

    struct fixed_vector_allocator : public allocator
    {
        std::size_t m_ullBlockSize;
    };

    struct byte_buffer_allocator : public allocator
    {
        std::size_t m_ullPosition;
    };

    uintptr_t calculate_aligned_pointer(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment);
    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

    void* calculate_aligned_pointer(const void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment);
    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

    struct ftable
    {
        typedef bool (*_basic_allocate)(linear_allocator** _pAllocator, std::size_t _ullSize);

        typedef uintptr_t (*_calculate_aligned_pointer)(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
        typedef uint8_t (*_calculate_padding)(const uintptr_t& _ullAddress, uint8_t _ucAlignment);
        typedef uint8_t (*_calculate_padding_h)(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t _ucHeaderSize);
        typedef void* (*_calculate_aligned_pointer_)(const void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
        typedef uint8_t (*_calculate_padding_)(const void* _p, uint8_t _ucAlignment);
        typedef uint8_t (*_calculate_padding_h_)(const void* _p, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

        typedef void* (*_linear_allocate)(linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void (*_linear_clear)(linear_allocator* _pAllocator);

        typedef void* (*_fixed_stack_allocate_)(fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_fixed_stack_top_)(fixed_stack_allocator* _pAllocator);
        typedef void (*_fixed_stack_pop_)(fixed_stack_allocator* _pAllocator);
        typedef bool (*_fixed_stack_empty_)(fixed_stack_allocator* _pAllocator);
        typedef bool (*_fixed_stack_full_)(fixed_stack_allocator* _pAllocator);
        typedef void (*_fixed_stack_clear_)(fixed_stack_allocator* _pAllocator);
        typedef std::size_t (*_fixed_stack_size_)(fixed_stack_allocator* _pAllocator);
        typedef std::size_t (*_fixed_stack_capacity_)(fixed_stack_allocator* _pAllocator);

        typedef void* (*_fixed_queue_allocate_)(fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_fixed_queue_begin_)(fixed_queue_allocator* _pAllocator);
        typedef void (*_fixed_queue_pop_)(fixed_queue_allocator* _pAllocator);
        typedef bool (*_fixed_queue_empty_)(fixed_queue_allocator* _pAllocator);
        typedef bool (*_fixed_queue_full_)(fixed_queue_allocator* _pAllocator);
        typedef void (*_fixed_queue_clear_)(fixed_queue_allocator* _pAllocator);
        typedef std::size_t (*_fixed_queue_size_)(fixed_queue_allocator* _pAllocator);
        typedef std::size_t (*_fixed_queue_capacity_)(fixed_queue_allocator* _pAllocator);

        typedef void* (*_fixed_array_allocate_)(fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_fixed_array_at_)(fixed_array_allocator* _pAllocator, std::size_t _ullIndex);
        typedef void* (*_fixed_array_begin_)(fixed_array_allocator* _pAllocator);
        typedef void* (*_fixed_array_back_)(fixed_array_allocator* _pAllocator);
        typedef bool (*_fixed_array_empty_)(fixed_array_allocator* _pAllocator);
        typedef bool (*_fixed_array_full_)(fixed_array_allocator* _pAllocator);
        typedef std::size_t (*_fixed_array_size_)(fixed_array_allocator* _pAllocator);
        typedef void (*_fixed_array_clear_)(fixed_array_allocator* _pAllocator);

        typedef void* (*_free_list_allocate_d_)(free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_free_list_allocate_)(free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_free_list_reallocate_)(free_list_allocator* _pAllocator, void* _pData, std::size_t _ullDataSize, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void (*_free_list_free_)(free_list_allocator* _pAllocator, void* _pointer);
        typedef void (*_free_list_clear_)(free_list_allocator* _pAllocator);

        typedef void* (*_fixed_vector_allocate_)(fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_fixed_vector_at_)(fixed_vector_allocator* _pAllocator, std::size_t _ullIndex);
        typedef void* (*_fixed_vector_begin_)(fixed_vector_allocator* _pAllocator);
        typedef void* (*_fixed_vector_back_)(fixed_vector_allocator* _pAllocator);
        typedef void* (*_fixed_vector_end_)(fixed_vector_allocator* _pAllocator);
        typedef bool (*_fixed_vector_empty_)(fixed_vector_allocator* _pAllocator);
        typedef bool (*_fixed_vector_full_)(fixed_vector_allocator* _pAllocator);
        typedef void (*_fixed_vector_clear_)(fixed_vector_allocator* _pAllocator);
        typedef void (*_fixed_vector_resize_)(ftable* _pInterface, free_list_allocator* _pParentAllocator, fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize);
        typedef std::size_t (*_fixed_vector_size_)(fixed_vector_allocator* _pAllocator);
        typedef std::size_t (*_fixed_vector_capacity_)(fixed_vector_allocator* _pAllocator);
        typedef void (*_fixed_vector_erase_)(ftable* _pInterface, free_list_allocator* _pParentAllocator, fixed_vector_allocator** _pAllocator, uint32_t _uiIndex);

        typedef bool (*_byte_buffer_will_fit)(byte_buffer_allocator* _pAllocator, std::size_t _ullSize);
        typedef void (*_byte_buffer_clear)(byte_buffer_allocator* _pAllocator);
        typedef void (*_byte_buffer_resize)(ftable* _pInterface, free_list_allocator* _pParentAllocator, byte_buffer_allocator** _pAllocator, std::size_t _ullNewSize);
        typedef void (*_byte_buffer_seek_to_begin)(byte_buffer_allocator* _pAllocator);
        typedef void (*_byte_buffer_seek_to_end)(byte_buffer_allocator* _pAllocator);
        typedef void (*_byte_buffer_advance)(byte_buffer_allocator* _pAllocator, std::size_t _ullSize);
        typedef void (*_byte_buffer_write)(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize, const void* _pValue);
        typedef void* (*_byte_buffer_read)(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize);

        typedef linear_allocator* (*_linear_allocate_linear_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_stack_allocator* (*_linear_allocate_fixed_stack_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef fixed_array_allocator* (*_linear_allocate_fixed_array_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef free_list_allocator* (*_linear_allocate_free_list_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_vector_allocator* (*_linear_allocate_fixed_vector_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);

        typedef fixed_queue_allocator* (*_free_list_allocate_fixed_queue_allocator)(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef free_list_allocator* (*_free_list_allocate_free_list_allocator)(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_vector_allocator* (*_free_list_allocate_fixed_vector_allocator)(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef fixed_stack_allocator* (*_free_list_allocate_fixed_stack_allocator)(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef fixed_array_allocator* (*_free_list_allocate_fixed_array_allocator)(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef byte_buffer_allocator* (*_free_list_allocate_byte_buffer_allocator)(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);

        _basic_allocate             m_fnBasicAllocate;

        _calculate_aligned_pointer  m_fnCalculateAlignedPointer;
        _calculate_padding          m_fnCalculatePadding;
        _calculate_padding_h        m_fnCalculatePaddingH;
        _calculate_aligned_pointer_ m_fnCalculateAlignedPointer_;
        _calculate_padding_         m_fnCalculatePadding_;
        _calculate_padding_h_       m_fnCalculatePaddingH_;

        _linear_allocate            m_fnLinearAllocate;
        _linear_clear               m_fnLinearClear;

        _fixed_stack_allocate_      m_fnFixedStackAllocate_;
        _fixed_stack_top_           m_fnFixedStackTop_;
        _fixed_stack_pop_           m_fnFixedStackPop_;
        _fixed_stack_empty_         m_fnFixedStackEmpty_;
        _fixed_stack_full_          m_fnFixedStackFull_;
        _fixed_stack_clear_         m_fnFixedStackClear_;
        _fixed_stack_size_          m_fnFixedStackSize_;
        _fixed_stack_capacity_      m_fnFixedStackCapacity_;

        _fixed_queue_allocate_      m_fnFixedQueueAllocate_;
        _fixed_queue_begin_         m_fnFixedQueueBegin_;
        _fixed_queue_pop_           m_fnFixedQueuePop_;
        _fixed_queue_empty_         m_fnFixedQueueEmpty_;
        _fixed_queue_full_          m_fnFixedQueueFull_;
        _fixed_queue_clear_         m_fnFixedQueueClear_;
        _fixed_queue_size_          m_fnFixedQueueSize_;
        _fixed_queue_capacity_      m_fnFixedQueueCapacity_;

        _fixed_array_allocate_      m_fnFixedArrayAllocate_;
        _fixed_array_begin_         m_fnFixedArrayBegin_;
        _fixed_array_back_          m_fnFixedArrayBack_;
        _fixed_array_at_            m_fnFixedArrayAt_;
        _fixed_array_empty_         m_fnFixedArrayEmpty_;
        _fixed_array_full_          m_fnFixedArrayFull_;
        _fixed_array_size_          m_fnFixedArraySize_;
        _fixed_array_clear_         m_fnFixedArrayClear_;

        _free_list_allocate_        m_fnFreeListAllocate_;
        _free_list_allocate_d_      m_fnFreeListAllocate_D_;
        _free_list_reallocate_      m_fnFreeListReallocate_;
        _free_list_free_            m_fnFreeListFree_;
        _free_list_clear_           m_fnFreeListClear_;

        _fixed_vector_allocate_         m_fnFixedVectorAllocate_;
        _fixed_vector_begin_            m_fnFixedVectorBegin_;
        _fixed_vector_back_             m_fnFixedVectorBack_;
        _fixed_vector_end_              m_fnFixedVectorEnd_;
        _fixed_vector_at_               m_fnFixedVectorAt_;
        _fixed_vector_empty_            m_fnFixedVectorEmpty_;
        _fixed_vector_full_             m_fnFixedVectorFull_;
        _fixed_vector_clear_            m_fnFixedVectorClear_;
        _fixed_vector_resize_           m_fnFixedVectorResize_;
        _fixed_vector_size_             m_fnFixedVectorSize_;
        _fixed_vector_capacity_         m_fnFixedVectorCapacity_;
        _fixed_vector_erase_            m_fnFixedVectorErase_;

        _byte_buffer_will_fit       m_fnByteBufferWillFit_;
        _byte_buffer_clear          m_fnByteBufferClear_;
        _byte_buffer_resize         m_fnByteBufferResize_;
        _byte_buffer_seek_to_begin  m_fnByteBufferSeekToBegin_;
        _byte_buffer_seek_to_end    m_fnByteBufferSeekToEnd_;
        _byte_buffer_advance        m_fnByteBufferAdvance_;
        _byte_buffer_write          m_fnByteBufferWrite_;
        _byte_buffer_read           m_fnByteBufferRead_;

        _linear_allocate_linear_allocator               m_fnLinearAllocateLinearAllocator;
        _linear_allocate_fixed_stack_allocator          m_fnLinearAllocateFixedStackAllocator;
        _linear_allocate_fixed_array_allocator          m_fnLinearAllocateFixedArrayAllocator;
        _linear_allocate_free_list_allocator            m_fnLinearAllocateFreeListAllocator;
        _linear_allocate_fixed_vector_allocator         m_fnLinearAllocateFixedVectorAllocator;

        _free_list_allocate_fixed_queue_allocator       m_fnFreeListAllocateFixedQueueAllocator;
        _free_list_allocate_free_list_allocator         m_fnFreeListAllocateFreeListAllocator;
        _free_list_allocate_fixed_vector_allocator      m_fnFreeListAllocateFixedVectorAllocator;
        _free_list_allocate_fixed_stack_allocator       m_fnFreeListAllocateFixedStackAllocator;
        _free_list_allocate_fixed_array_allocator       m_fnFreeListAllocateFixedArrayAllocator;
        _free_list_allocate_byte_buffer_allocator       m_fnFreeListAllocateByteBufferAllocator;
    };

    typedef ftable* (*init_callback)();

}}

DUCKVIL_EXPORT Duckvil::Memory::ftable* duckvil_memory_init();