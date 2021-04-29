#include "Memory/Internal/ByteBuffer.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    bool impl_byte_buffer_will_fit(byte_buffer_allocator* _pAllocator, std::size_t _ullSize)
    {
        return _pAllocator->m_ullUsed + _ullSize <= _pAllocator->m_ullCapacity;
    }

    void impl_byte_buffer_clear(byte_buffer_allocator* _pAllocator)
    {
        _pAllocator->m_ullPosition = 0;
        _pAllocator->m_ullUsed = 0;
    }

    void impl_byte_buffer_resize(ftable* _pInterface, free_list_allocator* _pParentAllocator, byte_buffer_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        if(_pParentAllocator->m_ullCapacity < _ullNewSize + _pParentAllocator->m_ullUsed)
        {
            return;
        }

        // fixed_vector_allocator* _allocator = (fixed_vector_allocator*)free_list_allocate(_pInterface, _pParentAllocator, sizeof(fixed_vector_allocator) + (_ullNewSize * (*_pAllocator)->m_ullBlockSize), alignof(fixed_vector_allocator));

        // _allocator->m_pMemory = (uint8_t*)_allocator + sizeof(fixed_vector_allocator);
        // _allocator->m_ullCapacity = _ullNewSize * (*_pAllocator)->m_ullBlockSize;
        // _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;
        // _allocator->m_ullBlockSize = (*_pAllocator)->m_ullBlockSize;

        // memcpy(_allocator->m_pMemory, (*_pAllocator)->m_pMemory, (*_pAllocator)->m_ullUsed);

        byte_buffer_allocator* _allocator = _pInterface->m_fnFreeListAllocateByteBufferAllocator(_pInterface, _pParentAllocator, _ullNewSize);

        _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;

        memcpy((uint8_t*)_allocator + sizeof(byte_buffer_allocator), (uint8_t*)(*_pAllocator) + sizeof(byte_buffer_allocator), (*_pAllocator)->m_ullUsed);

/*#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pParentAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pParentAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pParentAllocator, _allocator, duckvil_memory_allocator_type_vector);
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif*/

#ifdef DUCKVIL_MEMORY_DEBUGGER
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif

        byte_buffer_allocator* _ptr = *_pAllocator;

        *_pAllocator = _allocator;

// TODO:
        // free_list_free(_pInterface, _pParentAllocator, _ptr);

        _pInterface->m_fnFreeListFree_(_pParentAllocator, _ptr);
    }

    void impl_byte_buffer_seek_to_begin(byte_buffer_allocator* _pAllocator)
    {
        _pAllocator->m_ullPosition = 0;
    }

    void impl_byte_buffer_seek_to_end(byte_buffer_allocator* _pAllocator)
    {
        _pAllocator->m_ullPosition = _pAllocator->m_ullUsed;
    }

    void impl_byte_buffer_advance(byte_buffer_allocator* _pAllocator, std::size_t _ullSize)
    {
        _pAllocator->m_ullPosition += _ullSize;
    }

    void impl_byte_buffer_write(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize, const void* _pValue)
    {
        memcpy((uint8_t*)_pAllocator + sizeof(byte_buffer_allocator) + _pAllocator->m_ullPosition, _pValue, _ullTypeSize);

        _pAllocator->m_ullUsed += _ullTypeSize;
        _pAllocator->m_ullPosition += _ullTypeSize;
    }

    void* impl_byte_buffer_read(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize)
    {
        void* _res = (void*)((uint8_t*)_pAllocator + sizeof(byte_buffer_allocator) + _pAllocator->m_ullPosition);

        _pAllocator->m_ullPosition += _ullTypeSize;

        return _res;
    }

}}