#include "Memory/Internal/FixedVectorAllocator.h"

#include <cstring>

#include "Memory/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    void* fixed_vector_allocate(__fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->used >= _pAllocator->capacity || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->used += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    const char* fixed_vector_allocate(__fixed_vector_allocator* _pAllocator, const char* _pData, std::size_t _ullLength)
    {
        void* _memory = nullptr;

        if(_pAllocator->capacity < _pAllocator->used + _ullLength)
        {
            return 0;
        }

        _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _ullLength);

        _pAllocator->used += _ullLength;

        return (const char*)_memory;
    }

    void* fixed_vector_begin(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->memory;
    }

    void* fixed_vector_back(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->memory + _pAllocator->used - _pAllocator->m_ullBlockSize;
    }

    void* fixed_vector_at(__fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pAllocator->memory + (_ullIndex * _pAllocator->m_ullBlockSize);
    }

    std::size_t fixed_vector_size(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    bool fixed_vector_empty(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_vector_full(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    void fixed_vector_resize(IMemory* _pInterface, __free_list_allocator* _pParentAllocator, __fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        if(_pParentAllocator->capacity < _ullNewSize + _pParentAllocator->used)
        {
            return;
        }

        __fixed_vector_allocator* _allocator = (__fixed_vector_allocator*)free_list_allocate(_pInterface, _pParentAllocator, sizeof(__fixed_vector_allocator) + _ullNewSize, alignof(__fixed_vector_allocator));

        _allocator->memory = (uint8_t*)_allocator + sizeof(__fixed_vector_allocator);
        _allocator->capacity = _ullNewSize;
        _allocator->used = (*_pAllocator)->used;
        _allocator->m_ullBlockSize = (*_pAllocator)->m_ullBlockSize;

        memcpy(_allocator->memory, (*_pAllocator)->memory, (*_pAllocator)->used);

        _pInterface->m_fnFreeListFree_(_pParentAllocator, *_pAllocator);

        *_pAllocator = _allocator;
    }

    void fixed_vector_clear(__fixed_vector_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}