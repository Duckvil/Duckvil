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

    struct IMemory
    {
        typedef bool (*_basic_allocate)(__allocator* _pAllocator, size_t _ullSize);

        typedef void* (*_linear_allocate)(__linear_allocator& _allocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_linear_allocate_cstr)(__linear_allocator& _allocator, const char* _pData);
        typedef void (*_linear_clear)(__linear_allocator& _allocator);

        typedef __linear_allocator* (*_allocate_linear_allocator)(__allocator* _pAllocator, size_t _ullSize);

        typedef void* (*_linear_allocate_)(__linear_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment);
        typedef const char* (*_linear_allocate_cstr_)(__linear_allocator* _pAllocator, const char* _pData);
        typedef void (*_linear_clear_)(__linear_allocator* _pAllocator);

        _basic_allocate             m_fnBasicAllocate;

        _linear_allocate            m_fnLinearAllocate;
        _linear_allocate_cstr       m_fnLinearAllocateCStr;
        _linear_clear               m_fnLinearClear;

        _allocate_linear_allocator  m_fnAllocateLinearAllocator;

        _linear_allocate_           m_fnLinearAllocate_;
        _linear_allocate_cstr_      m_fnLinearAllocateCStr_;
        _linear_clear_              m_fnLinearClear_;
    };

}}

DUCKVIL_EXPORT Duckvil::Memory::IMemory* duckvil_memory_init();