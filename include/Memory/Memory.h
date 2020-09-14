#pragma once

#include <cstdint>
#include <limits>

#include "Utils/Macro.h"

namespace Duckvil { namespace Memory {

    static const std::size_t g_ullMax = std::numeric_limits<std::size_t>::max();

    struct __allocator
    {
        std::size_t capacity = 0;
        std::size_t used = 0;
        uint8_t* memory = 0;
    };

    struct __linear_allocator : public __allocator
    {

    };

    struct __fixed_stack_allocator : public __allocator
    {
        std::size_t m_ullBlockSize;
    };

    struct __stack_allocator : public __allocator
    {

    };

    struct __fixed_queue_allocator : public __allocator
    {
        std::size_t m_ullBlockSize;
        std::size_t m_ullTail;
        std::size_t m_ullHead;
    };

    struct __queue_allocator : public __allocator
    {
        std::size_t m_ullTail;
        std::size_t m_ullHead;
    };

    struct __fixed_array_allocator : public __allocator
    {
        std::size_t m_ullBlockSize;
    };

    struct __free_list_allocator : public __allocator
    {
        void* m_pFreeBlocks;
    };

    void* calculate_aligned_pointer(const void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);
    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment);
    uint8_t calculate_padding(const void* _p, uint8_t _ucAlignment, uint8_t _ucHeaderSize);

    struct IMemory
    {
        typedef bool (*_basic_allocate)(__allocator* _pAllocator, std::size_t _ullSize);

        typedef void* (*_linear_allocate)(__linear_allocator& _allocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_linear_allocate_cstr)(__linear_allocator& _allocator, const char* _pData);
        typedef void (*_linear_clear)(__linear_allocator& _allocator);
        typedef void* (*_linear_allocate_)(__linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_linear_allocate_cstr_)(__linear_allocator* _pAllocator, const char* _pData);
        typedef void (*_linear_clear_)(__linear_allocator* _pAllocator);

        typedef void* (*_fixed_stack_allocate_)(__fixed_stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_fixed_stack_allocate_cstr_)(__fixed_stack_allocator* _pAllocator, const char* _pData);
        typedef void* (*_fixed_stack_top_)(__fixed_stack_allocator* _pAllocator);
        typedef void (*_fixed_stack_pop_)(__fixed_stack_allocator* _pAllocator);
        typedef bool (*_fixed_stack_empty_)(__fixed_stack_allocator* _pAllocator);
        typedef bool (*_fixed_stack_full_)(__fixed_stack_allocator* _pAllocator);
        typedef void (*_fixed_stack_clear_)(__fixed_stack_allocator* _pAllocator);

        typedef void* (*_stack_allocate_)(__stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_stack_allocate_cstr_)(__stack_allocator* _pAllocator, const char* _pData);
        typedef void* (*_stack_top_)(__stack_allocator* _pAllocator);
        typedef void (*_stack_pop_)(__stack_allocator* _pAllocator);
        typedef bool (*_stack_empty_)(__stack_allocator* _pAllocator);
        typedef bool (*_stack_full_)(__stack_allocator* _pAllocator);
        typedef void (*_stack_clear_)(__stack_allocator* _pAllocator);

        typedef void* (*_fixed_queue_allocate_)(__fixed_queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_fixed_queue_allocate_cstr_)(__fixed_queue_allocator* _pAllocator, const char* _pData);
        typedef void* (*_fixed_queue_begin_)(__fixed_queue_allocator* _pAllocator);
        typedef void (*_fixed_queue_pop_)(__fixed_queue_allocator* _pAllocator);
        typedef bool (*_fixed_queue_empty_)(__fixed_queue_allocator* _pAllocator);
        typedef bool (*_fixed_queue_full_)(__fixed_queue_allocator* _pAllocator);
        typedef void (*_fixed_queue_clear_)(__fixed_queue_allocator* _pAllocator);

        typedef void* (*_queue_allocate_)(__queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_queue_allocate_cstr_)(__queue_allocator* _pAllocator, const char* _pData);
        typedef void* (*_queue_begin_)(__queue_allocator* _pAllocator);
        typedef void (*_queue_pop_)(__queue_allocator* _pAllocator);
        typedef bool (*_queue_empty_)(__queue_allocator* _pAllocator);
        typedef bool (*_queue_full_)(__queue_allocator* _pAllocator);
        typedef void (*_queue_clear_)(__queue_allocator* _pAllocator);

        typedef void* (*_fixed_array_allocate_)(__fixed_array_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_fixed_array_allocate_cstr_)(__fixed_array_allocator* _pAllocator, const char* _pData);
        typedef void* (*_fixed_array_at_)(__fixed_array_allocator* _pAllocator, std::size_t _ullIndex);
        typedef void* (*_fixed_array_begin_)(__fixed_array_allocator* _pAllocator);
        typedef void* (*_fixed_array_back_)(__fixed_array_allocator* _pAllocator);
        typedef bool (*_fixed_array_empty_)(__fixed_array_allocator* _pAllocator);
        typedef bool (*_fixed_array_full_)(__fixed_array_allocator* _pAllocator);
        typedef void (*_fixed_array_clear_)(__fixed_array_allocator* _pAllocator);

        typedef __linear_allocator* (*_allocate_linear_allocator)(__allocator* _pAllocator, std::size_t _ullSize);
        typedef __fixed_stack_allocator* (*_allocate_fixed_stack_allocator)(__allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef __stack_allocator* (*_allocate_stack_allocator)(__allocator* _pAllocator, std::size_t _ullSize);
        typedef __fixed_queue_allocator* (*_allocate_fixed_queue_allocator)(__allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef __queue_allocator* (*_allocate_queue_allocator)(__allocator* _pAllocator, std::size_t _ullSize);
        typedef __fixed_array_allocator* (*_allocate_fixed_array_allocator)(__allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize);
        typedef __free_list_allocator* (*_allocate_free_list_allocator)(__allocator* _pAllocator, std::size_t _ullSize);

        typedef void* (*_free_list_allocate_)(__free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_free_list_allocate_cstr_)(__free_list_allocator* _pAllocator, const char* _pData);
        typedef void (*_free_list_clear_)(__free_list_allocator* _pAllocator);

        _basic_allocate             m_fnBasicAllocate;

        _linear_allocate            m_fnLinearAllocate;
        _linear_allocate_cstr       m_fnLinearAllocateCStr;
        _linear_clear               m_fnLinearClear;
        _linear_allocate_           m_fnLinearAllocate_;
        _linear_allocate_cstr_      m_fnLinearAllocateCStr_;
        _linear_clear_              m_fnLinearClear_;

        _fixed_stack_allocate_      m_fnFixedStackAllocate_;
        _fixed_stack_allocate_cstr_ m_fnFixedStackAllocateCStr_;
        _fixed_stack_top_           m_fnFixedStackTop_;
        _fixed_stack_pop_           m_fnFixedStackPop_;
        _fixed_stack_empty_         m_fnFixedStackEmpty_;
        _fixed_stack_full_          m_fnFixedStackFull_;
        _fixed_stack_clear_         m_fnFixedStackClear_;

        _stack_allocate_        m_fnStackAllocate_;
        _stack_allocate_cstr_   m_fnStackAllocateCStr_;
        _stack_top_             m_fnStackTop_;
        _stack_pop_             m_fnStackPop_;
        _stack_empty_           m_fnStackEmpty_;
        _stack_full_            m_fnStackFull_;
        _stack_clear_           m_fnStackClear_;

        _fixed_queue_allocate_      m_fnFixedQueueAllocate_;
        _fixed_queue_allocate_cstr_ m_fnFixedQueueAllocateCStr_;
        _fixed_queue_begin_         m_fnFixedQueueBegin_;
        _fixed_queue_pop_           m_fnFixedQueuePop_;
        _fixed_queue_empty_         m_fnFixedQueueEmpty_;
        _fixed_queue_full_          m_fnFixedQueueFull_;
        _fixed_queue_clear_         m_fnFixedQueueClear_;

        _queue_allocate_      m_fnQueueAllocate_;
        _queue_allocate_cstr_ m_fnQueueAllocateCStr_;
        _queue_begin_         m_fnQueueBegin_;
        _queue_pop_           m_fnQueuePop_;
        _queue_empty_         m_fnQueueEmpty_;
        _queue_full_          m_fnQueueFull_;
        _queue_clear_         m_fnQueueClear_;

        _fixed_array_allocate_      m_fnFixedArrayAllocate_;
        _fixed_array_allocate_cstr_ m_fnFixedArrayAllocateCStr_;
        _fixed_array_begin_         m_fnFixedArrayBegin_;
        _fixed_array_back_          m_fnFixedArrayBack_;
        _fixed_array_at_            m_fnFixedArrayAt_;
        _fixed_array_empty_         m_fnFixedArrayEmpty_;
        _fixed_array_full_          m_fnFixedArrayFull_;
        _fixed_array_clear_         m_fnFixedArrayClear_;

        _free_list_allocate_           m_fnFreeListAllocate_;
        _free_list_allocate_cstr_      m_fnFreeListAllocateCStr_;
        _free_list_clear_              m_fnFreeListClear_;

        _allocate_linear_allocator      m_fnAllocateLinearAllocator;
        _allocate_fixed_stack_allocator m_fnAllocateFixedStackAllocator;
        _allocate_stack_allocator       m_fnAllocateStackAllocator;
        _allocate_fixed_queue_allocator m_fnAllocateFixedQueueAllocator;
        _allocate_queue_allocator       m_fnAllocateQueueAllocator;
        _allocate_fixed_array_allocator m_fnAllocateFixedArrayAllocator;
        _allocate_free_list_allocator   m_fnAllocateFreeListAllocator;
    };

}}

DUCKVIL_EXPORT Duckvil::Memory::IMemory* duckvil_memory_init();