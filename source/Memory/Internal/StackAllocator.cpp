#include "Memory/Internal/StackAllocator.h"

#include <cstring>
#include <stdexcept>

namespace Duckvil { namespace Memory {

    void* impl_stack_allocate(stack_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_stack_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_stack_allocate(stack_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(impl_stack_allocator_full(_pAllocator))
        {
            throw std::overflow_error("Stack is full!");

            return _memory;
        }

        std::size_t _currentUsed = _pAllocator->m_ullUsed;

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(stack_allocator) + _currentUsed, _ucAlignment, _padding);

        _currentUsed += _ullSize + _padding;

        uint8_t _hPadding = 0;
        stack_header* _header = static_cast<stack_header*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(stack_allocator) + _currentUsed, alignof(stack_header), _hPadding));

        _currentUsed += sizeof(stack_header) + _hPadding;

        if(_currentUsed > impl_stack_allocator_capacity(_pAllocator))
        {
            throw std::overflow_error("Stack is full!");

            return nullptr;
        }

        _header->m_ullElementSize = _ullSize;
        _header->m_ucPadding = _hPadding;

        _pAllocator->m_ullUsed = _currentUsed;

        return _memory;
    }

    void* impl_stack_allocator_top(stack_allocator* _pAllocator)
    {
        uint8_t _p = 0;
        stack_header* _header = static_cast<stack_header*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(stack_allocator) + _pAllocator->m_ullUsed - sizeof(stack_header), alignof(stack_header), _p));

        return reinterpret_cast<uint8_t*>(_header) - _header->m_ullElementSize - _header->m_ucPadding;
    }

    void impl_stack_allocator_pop(stack_allocator* _pAllocator)
    {
        if(impl_stack_allocator_empty(_pAllocator))
        {
            throw std::underflow_error("Stack is empty!");

            return;
        }

        uint8_t _p = 0;
        stack_header* _header = static_cast<stack_header*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(stack_allocator) + _pAllocator->m_ullUsed - sizeof(stack_header), alignof(stack_header), _p));
        uint8_t* _memory = reinterpret_cast<uint8_t*>(_header) - _header->m_ullElementSize - _header->m_ucPadding;

        _pAllocator->m_ullUsed -= (_header->m_ullElementSize + _header->m_ucPadding + sizeof(stack_header));

        memset(_memory, 0, _header->m_ullElementSize + _header->m_ucPadding + sizeof(stack_header));
    }

    bool impl_stack_allocator_empty(stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_stack_allocator_full(stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_stack_clear(stack_allocator* _pAllocator)
    {
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(stack_allocator), 0, _pAllocator->m_ullCapacity);

        _pAllocator->m_ullUsed = 0;
    }

    std::size_t impl_stack_allocator_size(stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_stack_allocator_capacity(stack_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

}}