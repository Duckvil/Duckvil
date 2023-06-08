#include "Memory/Internal/QueueAllocator.h"

#include <cstring>
#include <stdexcept>

#include "Memory/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    void* impl_queue_allocate(queue_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_queue_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_queue_allocate(queue_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        uint8_t _hPadding = 0;
        queue_header* _header = reinterpret_cast<queue_header*>(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullHead);
        
        if(!is_aligned(_header))
        {
            _header = static_cast<queue_header*>(calculate_aligned_pointer(_header, alignof(queue_header), _hPadding));
        }

        if(impl_queue_full(_pAllocator) || impl_queue_size(_pAllocator) + sizeof(queue_header) + _hPadding + _ullSize > impl_queue_capacity(_pAllocator))
        {
            throw std::overflow_error("Queue is full!");

            return _memory;
        }
        else if(_pAllocator->m_ullHead + sizeof(queue_header) + _hPadding + _ullSize >= _pAllocator->m_ullCapacity)
        {
            _pAllocator->m_ullHead = 0;

            _header = reinterpret_cast<queue_header*>(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullHead);
        
            // if(!is_aligned(_header))
            {
                _header = static_cast<queue_header*>(calculate_aligned_pointer(_header, alignof(queue_header), _hPadding));
            }
        }

        _header->m_ullElementSize = _ullSize;

        _pAllocator->m_ullHead += sizeof(queue_header) + _hPadding;
        _pAllocator->m_ullUsed += sizeof(queue_header) + _hPadding;

        uint8_t _padding = 0;
        _memory = reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullHead;

        if(!is_aligned(_memory, _ucAlignment))
        {
            _memory = calculate_aligned_pointer(_memory, _ucAlignment, _padding);
        }

        _header->m_ucPadding = _padding;

        _pAllocator->m_ullHead += _ullSize + _padding;
        _pAllocator->m_ullUsed += _ullSize + _padding;

        return _memory;
    }

    void* impl_queue_begin(queue_allocator* _pAllocator)
    {
        if (_pAllocator->m_ullTail + sizeof(queue_header) >= _pAllocator->m_ullCapacity)
        {
            _pAllocator->m_ullTail = 0;
        }

        uint8_t _p = 0;
        queue_header* _header = static_cast<queue_header*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullTail, alignof(queue_header), _p));
        void* _memory = static_cast<void*>(reinterpret_cast<uint8_t*>(_header) + sizeof(queue_header) + _header->m_ucPadding);

        return _memory;
    }

    void impl_queue_pop(queue_allocator* _pAllocator)
    {
        if(impl_queue_empty(_pAllocator))
        {
            throw std::underflow_error("Queue is empty!");

            return;
        }

        uint8_t _hPadding = 0;
        queue_header* _header = static_cast<queue_header*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullTail, alignof(queue_header), _hPadding));

        if(_pAllocator->m_ullTail + sizeof(queue_header) + _hPadding + _header->m_ullElementSize >= _pAllocator->m_ullCapacity)
        {
            _pAllocator->m_ullTail = 0;

            _header = reinterpret_cast<queue_header*>(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullTail);
            _header = static_cast<queue_header*>(calculate_aligned_pointer(_header, alignof(queue_header), _hPadding));

            // queue_header* _header = static_cast<queue_header*>(calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator) + _pAllocator->m_ullTail, alignof(queue_header), _hPadding));
            // std::size_t _blockSize = _header->m_ullElementSize;
            // uint8_t _padding = _header->m_ucPadding;

            // // memset(reinterpret_cast<uint8_t*>(_header), 0, _blockSize + _padding + sizeof(queue_header));

            // _pAllocator->m_ullTail += _padding;
        }

        std::size_t _blockSize = _header->m_ullElementSize;
        uint8_t _padding = _header->m_ucPadding;

        memset(reinterpret_cast<uint8_t*>(_header), 0, _blockSize + _padding + sizeof(queue_header));

        _pAllocator->m_ullTail += _blockSize + _padding + sizeof(queue_header) + _hPadding;
        _pAllocator->m_ullUsed -= (_blockSize + _padding + sizeof(queue_header) + _hPadding);
    }

    bool impl_queue_empty(queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_queue_full(queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_queue_clear(queue_allocator* _pAllocator)
    {
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(queue_allocator), 0, _pAllocator->m_ullCapacity);

        _pAllocator->m_ullHead = 0;
        _pAllocator->m_ullUsed = 0;
        _pAllocator->m_ullTail = 0;
    }

    // Should be here?
    void impl_queue_resize(const ftable* _pInterface, free_list_allocator* _pParentAllocator, queue_allocator** _pAllocator, std::size_t _ullNewSize)
    {
//         if(_pParentAllocator->m_ullCapacity < _ullNewSize + _pParentAllocator->m_ullUsed)
//         {
//             return;
//         }

//         queue_allocator* _allocator = _pInterface->m_fnFreeListAllocateQueueAllocator(_pInterface, _pParentAllocator, _ullNewSize * (*_pAllocator)->m_ullBlockSize, (*_pAllocator)->m_ullBlockSize);

//         _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;
//         _allocator->m_ullTail = (*_pAllocator)->m_ullTail;
//         _allocator->m_ullHead = (*_pAllocator)->m_ullHead;

//         memcpy(reinterpret_cast<uint8_t*>(_allocator) + sizeof(queue_allocator), reinterpret_cast<uint8_t*>(*_pAllocator) + sizeof(queue_allocator), (*_pAllocator)->m_ullUsed);

// #ifdef DUCKVIL_MEMORY_DEBUGGER
//         memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
// #endif

//         queue_allocator* _ptr = *_pAllocator;

//         *_pAllocator = _allocator;

//         free_list_free(_pInterface, _pParentAllocator, _ptr);
    }

    std::size_t impl_queue_size(queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_queue_capacity(queue_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

}}