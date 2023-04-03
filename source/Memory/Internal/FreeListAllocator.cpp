#include "Memory/Internal/FreeListAllocator.h"

#include <cstring>
#include <assert.h>

#include "tracy/Tracy.hpp"

#if defined DUCKVIL_PLATFORM_LINUX
#define _ASSERT(x) assert(x)
#endif

namespace Duckvil { namespace Memory {

    void* impl_free_list_allocate(free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        __free_list_node* _free_block = static_cast<__free_list_node*>(_pAllocator->m_pHead);
        __free_list_node* _previous_free_block = nullptr;

        while(_free_block != nullptr)
        {
            uint8_t _padding = calculate_padding<__free_list_header>(_free_block, _ucAlignment);
            std::size_t _total_size = _ullSize + _padding;

            if(_free_block->m_ullSize < _total_size)
            {
                _previous_free_block = _free_block;
                _free_block = _free_block->m_pNext;

                continue;
            }

            static_assert(sizeof(__free_list_header) >= sizeof(__free_list_node), "sizeof(AllocationHeader) < sizeof(FreeBlock)"); 

            if(_free_block->m_ullSize - _total_size <= sizeof(__free_list_header))
            {
                _total_size = _free_block->m_ullSize;

                if(_previous_free_block != nullptr)
                {
                    _previous_free_block->m_pNext = _free_block->m_pNext;
                }
                else
                {
                    _pAllocator->m_pHead = _free_block->m_pNext;
                }
            }
            else
            {
                __free_list_node* _next_node = reinterpret_cast<__free_list_node*>((reinterpret_cast<uint8_t*>(_free_block) + _total_size));

                _next_node->m_ullSize = _free_block->m_ullSize - _total_size;
                _next_node->m_pNext = _free_block->m_pNext;

                if(_previous_free_block != nullptr)
                {
                    _previous_free_block->m_pNext = _next_node;
                }
                else
                {
                    _pAllocator->m_pHead = _next_node;
                }
            }

            uintptr_t _aligned_address = reinterpret_cast<uintptr_t>(_free_block) + _padding;
            __free_list_header* _header = reinterpret_cast<__free_list_header*>((_aligned_address - sizeof(__free_list_header)));

            _header->m_ullSize = _total_size;
            _header->m_ucPadding = _padding;

            _ASSERT(is_aligned(_header));

            _pAllocator->m_ullUsed += _total_size;

            _ASSERT(calculate_padding(_aligned_address, _ucAlignment) == 0);

#ifdef TRACY_ENABLE
            if(_pAllocator->m_sName != nullptr)
				TracyAllocN(reinterpret_cast<void*>(_aligned_address), _ullSize, _pAllocator->m_sName);
#endif

            return reinterpret_cast<void*>(_aligned_address);
        }

        throw std::bad_alloc();

        return nullptr;
    }

    void* impl_free_list_allocate(free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_free_list_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_free_list_reallocate(free_list_allocator* _pAllocator, void* _pData, std::size_t _ullDataSize, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _newAddress = impl_free_list_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_newAddress, _pData, _ullDataSize);

        impl_free_list_free(_pAllocator, _pData);

        return _newAddress;
    }

    void impl_free_list_free(free_list_allocator* _pAllocator, void* _pointer)
    {
        __free_list_header* _header = reinterpret_cast<__free_list_header*>((reinterpret_cast<uintptr_t>(_pointer) - sizeof(__free_list_header)));
        uintptr_t _node = reinterpret_cast<uintptr_t>(_pointer) - _header->m_ucPadding;
        std::size_t _block_size = _header->m_ullSize;
        uintptr_t _block_end = _node + _block_size;
        __free_list_node* _previous_node = nullptr;
        __free_list_node* _iterator = static_cast<__free_list_node*>(_pAllocator->m_pHead);

        _ASSERT(is_aligned(_header));

        while(_iterator != nullptr)
        {
            if(reinterpret_cast<uintptr_t>(_iterator) >= _block_end)
            {
                break;
            }

            _previous_node = _iterator;
            _iterator = _iterator->m_pNext;
        }

        if(_previous_node == nullptr)
        {
            _previous_node = reinterpret_cast<__free_list_node*>(_node);

            _previous_node->m_ullSize = _block_size;
            _previous_node->m_pNext = static_cast<__free_list_node*>(_pAllocator->m_pHead);

            _pAllocator->m_pHead = static_cast<__free_list_node*>(_previous_node);
        }
        else if(reinterpret_cast<uintptr_t>(_previous_node) + _previous_node->m_ullSize == _node)
        {
            _previous_node->m_ullSize += _block_size;
        }
        else
        {
            __free_list_node* _tmp = reinterpret_cast<__free_list_node*>(_node);

            _tmp->m_ullSize = _block_size;
            _tmp->m_pNext = _previous_node->m_pNext;
            _previous_node->m_pNext = _tmp;
            _previous_node = _tmp;
        }

        if(_iterator != nullptr && reinterpret_cast<uintptr_t>(_iterator) == _block_end)
        {
            _previous_node->m_ullSize += _iterator->m_ullSize;
            _previous_node->m_pNext = _iterator->m_pNext;
        }

        _pAllocator->m_ullUsed -= _block_size;

#ifdef TRACY_ENABLE
        if(_pAllocator->m_sName != nullptr)
			TracyFreeN(_pointer, _pAllocator->m_sName);
#endif
    }

    void impl_free_list_clear(free_list_allocator* _pAllocator)
    {
        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(free_list_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}