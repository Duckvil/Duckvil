#include "Memory/Internal/Allocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    void* calculate_aligned_pointer(void* _p, uint8_t _ucAlignment, uint8_t& _padded_offset)
    {
        uintptr_t _memory_address = reinterpret_cast<uintptr_t>(_p);
        uint8_t _padding = _ucAlignment - 1;
        uintptr_t _new_address = (_memory_address + _padding) & ~_padding;

        _padded_offset = _new_address - _memory_address;

        return reinterpret_cast<void*>(_new_address);
    }

    bool allocate(__allocator* _pAllocator, size_t _ullSize)
    {
        if(_pAllocator->memory == NULL)
        {
            _pAllocator->memory = (uint8_t*)malloc(_ullSize);

            if(_pAllocator->memory == NULL)
            {
                return false;
            }

            memset(_pAllocator->memory, 0, _ullSize);

            _pAllocator->capacity = _ullSize;
        }

        return true;
    }

    void* linear_allocate(__linear_allocator& _allocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_allocator.capacity < _allocator.used + _ullSize)
        {
            return 0;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_allocator.memory + _allocator.used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _allocator.used += _ullSize + _padding;

        return _memory;
    }

    const char* linear_allocate(__linear_allocator& _allocator, const char* _pData)
    {
        size_t _len = strlen(_pData);

        if(_allocator.capacity < _allocator.used + _len)
        {
            return 0;
        }

        void* _memory = (void*)(_allocator.memory + _allocator.used);

        memcpy(_memory, _pData, _len);

        _allocator.used += _len + 1;

        return (const char*)_memory;
    }

    void linear_clear(__linear_allocator& _allocator)
    {
        memset(_allocator.memory, 0, _allocator.capacity);
    }

    __linear_allocator* allocate_linear_allocator(__allocator* _pAllocator, size_t _ullSize)
    {
         __linear_allocator* _memory = (__linear_allocator*)(_pAllocator->memory + _pAllocator->used);
        size_t _size = sizeof(__linear_allocator);

        _memory->capacity = _ullSize;
        _memory->memory = (uint8_t*)_memory + _size;
        _memory->used = 0;

        memset(_memory->memory, 0, _ullSize);

        _pAllocator->used += _size + _ullSize;

        return _memory;
    }

    void* linear_allocate(__linear_allocator* _pAllocator, const void* _pData, size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_pAllocator->capacity < _pAllocator->used + _ullSize)
        {
            return 0;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->used += _ullSize + _padding;

        return _memory;
    }

    const char* linear_allocate(__linear_allocator* _pAllocator, const char* _pData)
    {
        size_t _len = strlen(_pData);

        if(_pAllocator->capacity < _pAllocator->used + _len)
        {
            return 0;
        }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _len);

        _pAllocator->used += _len + 1;

        return (const char*)_memory;
    }

    void linear_clear(__linear_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
    }

}}