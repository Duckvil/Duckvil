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

#ifdef TRACY_ENABLE
    	char* m_sName;
#endif
    };

    struct linear_allocator : public allocator
    {

    };

    struct stack_header
    {
        std::size_t m_ullElementSize;
        uint8_t m_ucPadding;
    };

    struct stack_allocator : public allocator
    {

    };

    struct fixed_stack_allocator : public allocator
    {
        std::size_t m_ullBlockSize;
    };

    struct queue_header
    {
        std::size_t m_ullElementSize;
        uint8_t m_ucPadding;
    };

    struct queue_allocator : public allocator
    {
        std::size_t m_ullTail;
        std::size_t m_ullHead;
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

    uintptr_t calculate_aligned_pointer(const uintptr_t& _ullAddress, uint8_t _ucAlignment);
    uintptr_t calculate_aligned_pointer(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment);
    uint8_t calculate_padding(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

    void* calculate_aligned_pointer(const void* _p, uint8_t _ucAlignment);
    void* calculate_aligned_pointer(const void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment);
    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

    template <typename Type>
    inline static uint8_t calculate_padding(const void* _ullAddress, uint8_t _ucAlignment)
    {
        if(alignof(Type) > _ucAlignment)
        {
            _ucAlignment = alignof(Type);
        }

        uint8_t _padding = calculate_padding(reinterpret_cast<uintptr_t>(_ullAddress) + sizeof(Type), _ucAlignment);

        return sizeof(Type) + _padding;
    }

    inline static bool is_aligned(const void* _cpAddress, const uint8_t& _ucAlignment)
    {
        return calculate_padding(_cpAddress, _ucAlignment) == 0;
    }

    template<class Type>
    inline static bool is_aligned(const Type* _cpAddress)
    {
        return is_aligned(static_cast<const void*>(_cpAddress), alignof(Type));
    }

    struct fixed_stack_container
    {
        typedef void* (*allocate)(fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*allocate_size)(fixed_stack_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*top)(fixed_stack_allocator* _pAllocator);
        typedef void (*pop)(fixed_stack_allocator* _pAllocator);
        typedef bool (*empty)(fixed_stack_allocator* _pAllocator);
        typedef bool (*full)(fixed_stack_allocator* _pAllocator);
        typedef void (*clear)(fixed_stack_allocator* _pAllocator);
        typedef std::size_t(*size)(fixed_stack_allocator* _pAllocator);
        typedef std::size_t(*capacity)(fixed_stack_allocator* _pAllocator);

        allocate        m_fnAllocate;
        allocate_size   m_fnAllocateSize;
        top             m_fnTop;
        pop             m_fnPop;
        empty           m_fnEmpty;
        full            m_fnFull;
        clear           m_fnClear;
        size            m_fnSize;
        capacity        m_fnCapacity;
    };

    struct fixed_queue_container
    {
        typedef void* (*allocate)(fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*allocate_size)(fixed_queue_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*begin)(fixed_queue_allocator* _pAllocator);
        typedef void (*pop)(fixed_queue_allocator* _pAllocator);
        typedef bool (*empty)(fixed_queue_allocator* _pAllocator);
        typedef bool (*full)(fixed_queue_allocator* _pAllocator);
        typedef void (*clear)(fixed_queue_allocator* _pAllocator);
        typedef void (*resize)(const void* _pInterface, free_list_allocator* _pParentAllocator, fixed_queue_allocator** _pAllocator, std::size_t _ullNewSize);
        typedef std::size_t(*size)(fixed_queue_allocator* _pAllocator);
        typedef std::size_t(*capacity)(fixed_queue_allocator* _pAllocator);

        allocate        m_fnAllocate;
        allocate_size   m_fnAllocateSize;
        begin           m_fnBegin;
        pop             m_fnPop;
        empty           m_fnEmpty;
        full            m_fnFull;
        clear           m_fnClear;
        resize          m_fnResize;
        size            m_fnSize;
        capacity        m_fnCapacity;
    };

    struct fixed_vector_container
    {
        typedef void* (*allocate)(fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*allocate_size)(fixed_vector_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*at)(fixed_vector_allocator* _pAllocator, std::size_t _ullIndex);
        typedef void* (*begin)(fixed_vector_allocator* _pAllocator);
        typedef void* (*back)(fixed_vector_allocator* _pAllocator);
        typedef void* (*end)(fixed_vector_allocator* _pAllocator);
        typedef bool (*empty)(fixed_vector_allocator* _pAllocator);
        typedef bool (*full)(fixed_vector_allocator* _pAllocator);
        typedef void (*clear)(fixed_vector_allocator* _pAllocator);
        typedef void (*resize)(const void* _pInterface, free_list_allocator* _pParentAllocator, fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize);
        typedef std::size_t(*size)(fixed_vector_allocator* _pAllocator);
        typedef std::size_t(*capacity)(fixed_vector_allocator* _pAllocator);
        typedef void (*erase)(fixed_vector_allocator* _pAllocator, uint32_t _uiIndex);
        typedef void (*copy)(fixed_vector_allocator* _pSourceAllocator, fixed_vector_allocator* _pTargetAllocator, void (*)(fixed_vector_allocator* _pTargetAllocator, void* _pValue));

        allocate        m_fnAllocate;
        allocate_size   m_fnAllocateSize;
        begin           m_fnBegin;
        back            m_fnBack;
        end             m_fnEnd;
        at              m_fnAt;
        empty           m_fnEmpty;
        full            m_fnFull;
        clear           m_fnClear;
        resize          m_fnResize;
        size            m_fnSize;
        capacity        m_fnCapacity;
        erase           m_fnErase;
        copy            m_fnCopy;
    };

    struct fixed_array_container
    {
        typedef void* (*allocate)(fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*allocate_size)(fixed_array_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*at)(fixed_array_allocator* _pAllocator, std::size_t _ullIndex);
        typedef void* (*begin)(fixed_array_allocator* _pAllocator);
        typedef void* (*back)(fixed_array_allocator* _pAllocator);
        typedef bool (*empty)(fixed_array_allocator* _pAllocator);
        typedef bool (*full)(fixed_array_allocator* _pAllocator);
        typedef std::size_t(*size)(fixed_array_allocator* _pAllocator);
        typedef void (*clear)(fixed_array_allocator* _pAllocator);

        allocate        m_fnAllocate;
        allocate_size   m_fnAllocateSize;
        begin           m_fnBegin;
        back            m_fnBack;
        at              m_fnAt;
        empty           m_fnEmpty;
        full            m_fnFull;
        size            m_fnSize;
        clear           m_fnClear;
    };

    struct stack_container
    {
        typedef void* (*allocate)(stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*allocate_size)(stack_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*top)(stack_allocator* _pAllocator);
        typedef void (*pop)(stack_allocator* _pAllocator);
        typedef bool (*empty)(stack_allocator* _pAllocator);
        typedef bool (*full)(stack_allocator* _pAllocator);
        typedef void (*clear)(stack_allocator* _pAllocator);
        typedef std::size_t(*size)(stack_allocator* _pAllocator);
        typedef std::size_t(*capacity)(stack_allocator* _pAllocator);

        allocate        m_fnAllocate;
        allocate_size   m_fnAllocateSize;
        top             m_fnTop;
        pop             m_fnPop;
        empty           m_fnEmpty;
        full            m_fnFull;
        clear           m_fnClear;
        size            m_fnSize;
        capacity        m_fnCapacity;
    };

    struct queue_container
    {
        typedef void* (*allocate)(queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*allocate_size)(queue_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*begin)(queue_allocator* _pAllocator);
        typedef void (*pop)(queue_allocator* _pAllocator);
        typedef bool (*empty)(queue_allocator* _pAllocator);
        typedef bool (*full)(queue_allocator* _pAllocator);
        typedef void (*clear)(queue_allocator* _pAllocator);
        typedef void (*resize)(const void* _pInterface, free_list_allocator* _pParentAllocator, queue_allocator** _pAllocator, std::size_t _ullNewSize);
        typedef std::size_t(*size)(queue_allocator* _pAllocator);
        typedef std::size_t(*capacity)(queue_allocator* _pAllocator);

        allocate        m_fnAllocate;
        allocate_size   m_fnAllocateSize;
        begin           m_fnBegin;
        pop             m_fnPop;
        empty           m_fnEmpty;
        full            m_fnFull;
        clear           m_fnClear;
        resize          m_fnResize;
        size            m_fnSize;
        capacity        m_fnCapacity;
    };

    struct byte_buffer_container
    {
        typedef bool (*will_fit)(byte_buffer_allocator* _pAllocator, std::size_t _ullSize);
        typedef void (*clear)(byte_buffer_allocator* _pAllocator);
        typedef void (*resize)(const void* _pInterface, free_list_allocator* _pParentAllocator, byte_buffer_allocator** _pAllocator, std::size_t _ullNewSize);
        typedef void (*seek_to_begin)(byte_buffer_allocator* _pAllocator);
        typedef void (*seek_to_end)(byte_buffer_allocator* _pAllocator);
        typedef void (*advance)(byte_buffer_allocator* _pAllocator, std::size_t _ullSize);
        typedef void (*write)(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize, const void* _pValue);
        typedef void* (*read)(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize);

        will_fit        m_fnWillFit;
        clear           m_fnClear;
        resize          m_fnResize;
        seek_to_begin   m_fnSeekToBegin;
        seek_to_end     m_fnSeekToEnd;
        advance         m_fnAdvance;
        write           m_fnWrite;
        read            m_fnRead;
    };

    struct ftable
    {
        typedef bool (*_basic_allocate)(linear_allocator** _pAllocator, std::size_t _ullSize);
        typedef bool (*_basic_free)(linear_allocator** _pAllocator);

        typedef uintptr_t (*_calculate_aligned_pointer)(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
        typedef uint8_t (*_calculate_padding)(const uintptr_t& _ullAddress, uint8_t _ucAlignment);
        typedef uint8_t (*_calculate_padding_h)(const uintptr_t& _ullAddress, uint8_t _ucAlignment, uint8_t _ucHeaderSize);
        typedef void* (*_calculate_aligned_pointer_)(const void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
        typedef uint8_t (*_calculate_padding_)(const void* _p, uint8_t _ucAlignment);
        typedef uint8_t (*_calculate_padding_h_)(const void* _p, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

        typedef void* (*_linear_allocate)(linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void (*_linear_clear)(linear_allocator* _pAllocator);

        typedef void* (*_free_list_allocate_d_)(free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_free_list_allocate_)(free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void* (*_free_list_reallocate_)(free_list_allocator* _pAllocator, void* _pData, std::size_t _ullDataSize, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef void (*_free_list_free_)(free_list_allocator* _pAllocator, void* _pointer);
        typedef void (*_free_list_clear_)(free_list_allocator* _pAllocator);

        typedef linear_allocator* (*_linear_allocate_linear_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_stack_allocator* (*_linear_allocate_fixed_stack_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef fixed_array_allocator* (*_linear_allocate_fixed_array_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef free_list_allocator* (*_linear_allocate_free_list_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_vector_allocator* (*_linear_allocate_fixed_vector_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef byte_buffer_allocator* (*_linear_allocate_byte_buffer_allocator)(linear_allocator* _pAllocator, std::size_t _ullSize);

        typedef queue_allocator* (*_free_list_allocate_queue_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_queue_allocator* (*_free_list_allocate_fixed_queue_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef free_list_allocator* (*_free_list_allocate_free_list_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_vector_allocator* (*_free_list_allocate_fixed_vector_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef stack_allocator* (*_free_list_allocate_stack_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);
        typedef fixed_stack_allocator* (*_free_list_allocate_fixed_stack_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef fixed_array_allocator* (*_free_list_allocate_fixed_array_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef byte_buffer_allocator* (*_free_list_allocate_byte_buffer_allocator)(const ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize);

        _basic_allocate             m_fnBasicAllocate;
        _basic_free                 m_fnBasicFree;

        _calculate_aligned_pointer  m_fnCalculateAlignedPointer;
        _calculate_padding          m_fnCalculatePadding;
        _calculate_padding_h        m_fnCalculatePaddingH;
        _calculate_aligned_pointer_ m_fnCalculateAlignedPointer_;
        _calculate_padding_         m_fnCalculatePadding_;
        _calculate_padding_h_       m_fnCalculatePaddingH_;

        _linear_allocate            m_fnLinearAllocate;
        _linear_clear               m_fnLinearClear;

        fixed_stack_container   m_fixedStackContainer;
        fixed_queue_container   m_fixedQueueContainer;
        fixed_vector_container  m_fixedVectorContainer;
        fixed_array_container   m_fixedArrayContainer;
        stack_container         m_stackContainer;
        queue_container         m_queueContainer;
        byte_buffer_container   m_byteBufferContainer;

        _free_list_allocate_        m_fnFreeListAllocate_;
        _free_list_allocate_d_      m_fnFreeListAllocate_D_;
        _free_list_reallocate_      m_fnFreeListReallocate_;
        _free_list_free_            m_fnFreeListFree_;
        _free_list_clear_           m_fnFreeListClear_;

        _linear_allocate_linear_allocator               m_fnLinearAllocateLinearAllocator;
        _linear_allocate_fixed_stack_allocator          m_fnLinearAllocateFixedStackAllocator;
        _linear_allocate_fixed_array_allocator          m_fnLinearAllocateFixedArrayAllocator;
        _linear_allocate_free_list_allocator            m_fnLinearAllocateFreeListAllocator;
        _linear_allocate_fixed_vector_allocator         m_fnLinearAllocateFixedVectorAllocator;
        _linear_allocate_byte_buffer_allocator          m_fnLinearAllocateByteBufferAllocator;

        _free_list_allocate_queue_allocator             m_fnFreeListAllocateQueueAllocator;
        _free_list_allocate_fixed_queue_allocator       m_fnFreeListAllocateFixedQueueAllocator;
        _free_list_allocate_free_list_allocator         m_fnFreeListAllocateFreeListAllocator;
        _free_list_allocate_fixed_vector_allocator      m_fnFreeListAllocateFixedVectorAllocator;
        _free_list_allocate_stack_allocator             m_fnFreeListAllocateStackAllocator;
        _free_list_allocate_fixed_stack_allocator       m_fnFreeListAllocateFixedStackAllocator;
        _free_list_allocate_fixed_array_allocator       m_fnFreeListAllocateFixedArrayAllocator;
        _free_list_allocate_byte_buffer_allocator       m_fnFreeListAllocateByteBufferAllocator;
    };

}}

typedef const Duckvil::Memory::ftable* (*duckvil_memory_init_callback)();

DUCKVIL_EXPORT const Duckvil::Memory::ftable* duckvil_memory_init();