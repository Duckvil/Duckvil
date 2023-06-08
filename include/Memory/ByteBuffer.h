#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    inline bool byte_buffer_will_fit(const ftable* _pMemory, byte_buffer_allocator* _pAllocator, std::size_t _ullSize)
    {
        return _pMemory->m_fnByteBufferWillFit_(_pAllocator, _ullSize);
    }

    template <typename Type>
    inline bool byte_buffer_will_fit(const ftable* _pMemory, byte_buffer_allocator* _pAllocator)
    {
        return _pMemory->m_fnByteBufferWillFit_(_pAllocator, sizeof(Type));
    }

    inline void byte_buffer_clear(const ftable* _pMemory, byte_buffer_allocator* _pAllocator)
    {
        _pMemory->m_fnByteBufferClear_(_pAllocator);
    }

    inline void byte_buffer_resize(const ftable* _pInterface, free_list_allocator* _pParentAllocator, byte_buffer_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pInterface->m_fnByteBufferResize_(_pInterface, _pParentAllocator, _pAllocator, _ullNewSize);
    }

    inline void byte_buffer_seek_to_begin(const ftable* _pMemory, byte_buffer_allocator* _pAllocator)
    {
        _pMemory->m_fnByteBufferSeekToBegin_(_pAllocator);
    }

    inline void byte_buffer_seek_to_end(const ftable* _pMemory, byte_buffer_allocator* _pAllocator)
    {
        _pMemory->m_fnByteBufferSeekToEnd_(_pAllocator);
    }

    inline void byte_buffer_advance(const ftable* _pMemory, byte_buffer_allocator* _pAllocator, std::size_t _ullSize)
    {
        _pMemory->m_fnByteBufferAdvance_(_pAllocator, _ullSize);
    }

    inline void byte_buffer_write(const ftable* _pMemory, byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize, const void* _pValue)
    {
        _pMemory->m_fnByteBufferWrite_(_pAllocator, _ullTypeSize, _pValue);
    }

    inline void* byte_buffer_read(const ftable* _pMemory, byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize)
    {
        return _pMemory->m_fnByteBufferRead_(_pAllocator, _ullTypeSize);
    }

    template <typename Type>
    inline void byte_buffer_write(const ftable* _pMemory, byte_buffer_allocator* _pAllocator, const Type& _pValue)
    {
        _pMemory->m_fnByteBufferWrite_(_pAllocator, sizeof(Type), &_pValue);
    }

    template <typename Type>
    inline Type* byte_buffer_read(const ftable* _pMemory, byte_buffer_allocator* _pAllocator)
    {
        return (Type*)_pMemory->m_fnByteBufferRead_(_pAllocator, sizeof(Type));
    }

    template <typename Type>
    inline void byte_buffer_read(const ftable* _pMemory, byte_buffer_allocator* _pAllocator, Type* _pValue)
    {
        *_pValue = *(Type*)_pMemory->m_fnByteBufferRead_(_pAllocator, sizeof(Type));
    }

}}