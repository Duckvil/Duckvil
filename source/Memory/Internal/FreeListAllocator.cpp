#include "Memory/Internal/FreeListAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* free_list_allocate(__free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        __free_list_header* _header = (__free_list_header*)_pAllocator->memory;
        __free_list_node* _free_block = (__free_list_node*)_pAllocator->m_pFreeBlocks;
        __free_list_node* _previous_free_block = nullptr;

        while(_free_block != nullptr)
        {
            uint8_t _padding = calculate_padding((uint8_t*)_free_block + _pAllocator->used, _ucAlignment, sizeof(__free_list_header));
            std::size_t _total_size = _ullSize + _padding;

            if(_free_block->m_ullSize < _total_size)
            {
                _previous_free_block = _free_block;
                _free_block = _free_block->m_pNext;

                continue;
            }

            static_assert(sizeof(__free_list_header) >= sizeof(__free_list_node), "IDK");

            if(_free_block->m_ullSize - _total_size <= sizeof(__free_list_header))
            {
                _total_size = -_free_block->m_ullSize;

                if(_previous_free_block != nullptr)
                {
                    _previous_free_block->m_pNext = _free_block->m_pNext;
                }
                else
                {
                    _pAllocator->m_pFreeBlocks = _free_block->m_pNext;
                }
            }
            else
            {
                __free_list_node* _next_block = (__free_list_node*)(uint8_t*)_free_block + _total_size;

                _next_block->m_ullSize = _free_block->m_ullSize - _total_size;
                _next_block->m_pNext = _free_block->m_pNext;

                if(_previous_free_block != nullptr)
                {
                    _previous_free_block->m_pNext = _next_block;
                }
                else
                {
                    _pAllocator->m_pFreeBlocks = _next_block;
                }
            }

            uintptr_t _aligned_address = (uintptr_t)_free_block + _padding;
            __free_list_header* _new_header = (__free_list_header*)(_aligned_address - sizeof(__free_list_header));

            _new_header->m_ullSize = _total_size;
            _new_header->m_ucPadding = _padding;

            _pAllocator->used += _total_size;

            return (void*)_aligned_address;
        }

        // _pAllocator->used += sizeof(__free_list_node) + _padding;

        // void* _memory = calculate_aligned_pointer((uint8_t*)_node + sizeof(__free_list_node), _ucAlignment, _padding);

        // memcpy(_memory, _pData, _ullSize);

        // _pAllocator->used += _ullSize + _padding;

        // return _memory;

        return 0;
    }

    const char* free_list_allocate(__free_list_allocator* _pAllocator, const char* _pData, std::size_t _ullLength)
    {
        if(_pAllocator->capacity < _pAllocator->used + _ullLength)
        {
            return 0;
        }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _ullLength);

        _pAllocator->used += _ullLength;

        return (const char*)_memory;
    }

    void free_list_clear(__free_list_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}