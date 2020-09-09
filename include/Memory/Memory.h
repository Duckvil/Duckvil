#pragma once

#include <cstdint>

#include "Utils/Macro.h"

namespace Duckvil { namespace Memory {

    struct __allocator
    {
        size_t capacity = 0;
        size_t used = 0;
        uint8_t* memory = 0;
    };

    struct __linear_allocator : public __allocator
    {

    };

    struct __fixed_stack_allocator : public __allocator
    {
        size_t m_ullBlockSize;
    };

    struct __stack_allocator : public __allocator
    {

    };

    void* calculate_aligned_pointer(void* _p, uint8_t _ucAlignment, uint8_t& _ucPaddedOffset);

    struct IMemory
    {
        typedef bool (*_basic_allocate)(__allocator* _pAllocator, size_t _ullSize);

        typedef void* (*_linear_allocate)(__linear_allocator& _allocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_linear_allocate_cstr)(__linear_allocator& _allocator, const char* _pData);
        typedef void (*_linear_clear)(__linear_allocator& _allocator);
        typedef void* (*_linear_allocate_)(__linear_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_linear_allocate_cstr_)(__linear_allocator* _pAllocator, const char* _pData);
        typedef void (*_linear_clear_)(__linear_allocator* _pAllocator);

        typedef void* (*_fixed_stack_allocate_)(__fixed_stack_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_fixed_stack_allocate_cstr_)(__fixed_stack_allocator* _pAllocator, const char* _pData);
        typedef void* (*_fixed_stack_top_)(__fixed_stack_allocator* _pAllocator);
        typedef void (*_fixed_stack_pop_)(__fixed_stack_allocator* _pAllocator);
        typedef void (*_fixed_stack_clear_)(__fixed_stack_allocator* _pAllocator);

        typedef void* (*_stack_allocate_)(__stack_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_stack_allocate_cstr_)(__stack_allocator* _pAllocator, const char* _pData);
        typedef void* (*_stack_top_)(__stack_allocator* _pAllocator);
        typedef void (*_stack_pop_)(__stack_allocator* _pAllocator);
        typedef void (*_stack_clear_)(__stack_allocator* _pAllocator);

        typedef __linear_allocator* (*_allocate_linear_allocator)(__allocator* _pAllocator, size_t _ullSize);
        typedef __fixed_stack_allocator* (*_allocate_fixed_stack_allocator)(__allocator* _pAllocator, size_t _ullSize, size_t _ullTypeSize);
        typedef __stack_allocator* (*_allocate_stack_allocator)(__allocator* _pAllocator, size_t _ullSize);

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
        _fixed_stack_clear_         m_fnFixedStackClear_;

        _stack_allocate_      m_fnStackAllocate_;
        _stack_allocate_cstr_ m_fnStackAllocateCStr_;
        _stack_top_           m_fnStackTop_;
        _stack_pop_           m_fnStackPop_;
        _stack_clear_         m_fnStackClear_;

        _allocate_linear_allocator      m_fnAllocateLinearAllocator;
        _allocate_fixed_stack_allocator m_fnAllocateFixedStackAllocator;
        _allocate_stack_allocator       m_fnAllocateStackAllocator;
    };

}}

DUCKVIL_EXPORT Duckvil::Memory::IMemory* duckvil_memory_init();