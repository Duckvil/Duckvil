#include "Memory/Internal/FreeListAllocator.h"

#include <cstring>

namespace Duckvil { namespace Memory {

    void* free_list_allocate(__free_list_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        __free_list_node* _free_block = (__free_list_node*)_pAllocator->m_pHead;
        __free_list_node* _previous_free_block = nullptr;

        while(_free_block != nullptr)
        {
            uint8_t _padding = calculate_padding(_free_block, _ucAlignment, sizeof(__free_list_header));
            std::size_t _total_size = _ullSize + _padding;

            if(_free_block->m_ullSize < _total_size)
            {
                _previous_free_block = _free_block;
                _free_block = _free_block->m_pNext;

                continue;
            }

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
                __free_list_node* _next_node = (__free_list_node*)((uint8_t*)_free_block + _total_size);

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

            uintptr_t _aligned_address = (uintptr_t)_free_block + _padding;
            __free_list_header* _header = (__free_list_header*)(_aligned_address - sizeof(__free_list_header));

            _header->m_ullSize = _total_size;
            _header->m_ucPadding = _padding;

            _pAllocator->used += _total_size;

            return (void*)_aligned_address;
        }

        return 0;
    }

    void* free_list_allocate(__free_list_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        __free_list_node* _free_block = (__free_list_node*)_pAllocator->m_pHead;
        __free_list_node* _previous_free_block = nullptr;

        while(_free_block != nullptr)
        {
            uint8_t _padding = calculate_padding(_free_block, _ucAlignment, sizeof(__free_list_header));
            std::size_t _total_size = _ullSize + _padding;

            if(_free_block->m_ullSize < _total_size)
            {
                _previous_free_block = _free_block;
                _free_block = _free_block->m_pNext;

                continue;
            }

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
                __free_list_node* _next_node = (__free_list_node*)((uint8_t*)_free_block + _total_size);

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

            uintptr_t _aligned_address = (uintptr_t)_free_block + _padding;
            __free_list_header* _header = (__free_list_header*)(_aligned_address - sizeof(__free_list_header));

            _header->m_ullSize = _total_size;
            _header->m_ucPadding = _padding;

            _pAllocator->used += _total_size;

            __free_list_node* _new_node = (__free_list_node*)_aligned_address;

            memcpy(_new_node, _pData, _ullSize);

            return (void*)_aligned_address;
        }

        return 0;
    }

    void* free_list_reallocate(__free_list_allocator* _pAllocator, void* _pData, std::size_t _ullDataSize, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        __free_list_node* _free_block = (__free_list_node*)_pAllocator->m_pHead;
        __free_list_node* _previous_free_block = nullptr;

        while(_free_block != nullptr)
        {
            uint8_t _padding = calculate_padding(_free_block, _ucAlignment, sizeof(__free_list_header));
            std::size_t _total_size = _ullSize + _padding;

            if(_free_block->m_ullSize < _total_size)
            {
                _previous_free_block = _free_block;
                _free_block = _free_block->m_pNext;

                continue;
            }

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
                __free_list_node* _next_node = (__free_list_node*)((uint8_t*)_free_block + _total_size);

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

            uintptr_t _aligned_address = (uintptr_t)_free_block + _padding;
            __free_list_header* _header = (__free_list_header*)(_aligned_address - sizeof(__free_list_header));

            _header->m_ullSize = _total_size;
            _header->m_ucPadding = _padding;

            _pAllocator->used += _total_size;

            __free_list_node* _new_node = (__free_list_node*)_aligned_address;

            memcpy(_new_node, _pData, _ullDataSize);

            free_list_free(_pAllocator, _pData);

            return (void*)_aligned_address;
        }

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

    void free_list_free(__free_list_allocator* _pAllocator, void* _pointer)
    {
        __free_list_header* _header = (__free_list_header*)((uint8_t*)_pointer - sizeof(__free_list_header));
        uintptr_t _node = (uintptr_t)_pointer - _header->m_ucPadding;
        std::size_t _block_size = _header->m_ullSize;
        uintptr_t _block_end = _node + _block_size;
        __free_list_node* _previous_node = nullptr;
        __free_list_node* _iterator = (__free_list_node*)_pAllocator->m_pHead;

        while(_iterator != nullptr)
        {
            if((uintptr_t)_iterator >= _block_end)
            {
                break;
            }

            _previous_node = _iterator;
            _iterator = _iterator->m_pNext;
        }

        if(_previous_node == nullptr)
        {
            _previous_node = (__free_list_node*)_node;
            
            _previous_node->m_ullSize = _block_size;
            _previous_node->m_pNext = (__free_list_node*)_pAllocator->m_pHead;

            _pAllocator->m_pHead = (__free_list_node*)_previous_node;
        }
        else if((uintptr_t)_previous_node + _previous_node->m_ullSize == _node)
        {
            _previous_node->m_ullSize += _block_size;
        }
        else
        {
            __free_list_node* _tmp = (__free_list_node*)_node;

            _tmp->m_ullSize = _block_size;
            _tmp->m_pNext = _previous_node->m_pNext;
            _previous_node->m_pNext = _tmp;
            _previous_node = _tmp;
        }

        if(_iterator != nullptr && (uintptr_t)_iterator == _block_end)
        {
            _previous_node->m_ullSize += _iterator->m_ullSize;
            _previous_node->m_pNext = _iterator->m_pNext;
        }

        _pAllocator->used -= _block_size;
    }

    void free_list_clear(__free_list_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

    __fixed_vector_allocator* free_list_allocate_vector_allocator(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullSize, std::size_t _ullTypeSize, uint8_t _ucAlignment)
    {
        __fixed_vector_allocator* _vector_allocator = (__fixed_vector_allocator*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _ullSize + sizeof(__fixed_vector_allocator), _ucAlignment);

        _vector_allocator->capacity = _ullSize;
        _vector_allocator->used = 0;
        _vector_allocator->m_ullBlockSize = _ullTypeSize;
        _vector_allocator->memory = (uint8_t*)_vector_allocator + sizeof(__fixed_vector_allocator);

        return _vector_allocator;
    }

}}