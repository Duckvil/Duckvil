#include "Memory/Internal/FixedVectorAllocator.h"

#include <cstring>

#include "Memory/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_vector_allocate(__fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(_pAllocator->m_ullUsed >= _pAllocator->m_ullCapacity || _pAllocator->m_ullBlockSize < _ullSize)
        {
            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(_pAllocator->m_pMemory + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        return _memory;
    }

    void* impl_fixed_vector_begin(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_pMemory;
    }

    void* impl_fixed_vector_back(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_pMemory + _pAllocator->m_ullUsed - _pAllocator->m_ullBlockSize;
    }

    void* impl_fixed_vector_end(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_pMemory + _pAllocator->m_ullUsed;
    }

    void* impl_fixed_vector_at(__fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pAllocator->m_pMemory + (_ullIndex * _pAllocator->m_ullBlockSize);
    }

    std::size_t impl_fixed_vector_size(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_fixed_vector_capacity(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

    bool impl_fixed_vector_empty(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_vector_full(__fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_vector_resize(IMemory* _pInterface, __free_list_allocator* _pParentAllocator, __fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        if(_pParentAllocator->m_ullCapacity < _ullNewSize + _pParentAllocator->m_ullUsed)
        {
            return;
        }

        // __fixed_vector_allocator* _allocator = (__fixed_vector_allocator*)free_list_allocate(_pInterface, _pParentAllocator, sizeof(__fixed_vector_allocator) + (_ullNewSize * (*_pAllocator)->m_ullBlockSize), alignof(__fixed_vector_allocator));

        // _allocator->m_pMemory = (uint8_t*)_allocator + sizeof(__fixed_vector_allocator);
        // _allocator->m_ullCapacity = _ullNewSize * (*_pAllocator)->m_ullBlockSize;
        // _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;
        // _allocator->m_ullBlockSize = (*_pAllocator)->m_ullBlockSize;

        // memcpy(_allocator->m_pMemory, (*_pAllocator)->m_pMemory, (*_pAllocator)->m_ullUsed);

        __fixed_vector_allocator* _allocator = _pInterface->m_fnFreeListAllocateFixedVectorAllocator(_pInterface, _pParentAllocator, _ullNewSize * (*_pAllocator)->m_ullBlockSize, (*_pAllocator)->m_ullBlockSize);

        _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;

        memcpy(_allocator->m_pMemory, (*_pAllocator)->m_pMemory, (*_pAllocator)->m_ullUsed);

/*#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pParentAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pParentAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pParentAllocator, _allocator, duckvil_memory_allocator_type_vector);
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif*/

#ifdef DUCKVIL_MEMORY_DEBUGGER
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif

        __fixed_vector_allocator* _ptr = *_pAllocator;

        *_pAllocator = _allocator;

        free_list_free(_pInterface, _pParentAllocator, _ptr);
    }

    void impl_fixed_vector_erase(IMemory* _pInterface, __free_list_allocator* _pParentAllocator, __fixed_vector_allocator** _pAllocator, uint32_t _uiIndex)
    {
        uint32_t _newSize = (*_pAllocator)->m_ullCapacity - (*_pAllocator)->m_ullBlockSize;
        __fixed_vector_allocator* _allocator = (__fixed_vector_allocator*)free_list_allocate(_pInterface, _pParentAllocator, sizeof(__fixed_vector_allocator) + _newSize, alignof(__fixed_vector_allocator));

        _allocator->m_pMemory = (uint8_t*)_allocator + sizeof(__fixed_vector_allocator);
        _allocator->m_ullCapacity = _newSize;
        _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed - (*_pAllocator)->m_ullBlockSize;
        _allocator->m_ullBlockSize = (*_pAllocator)->m_ullBlockSize;

    // Edge case: if it is the first index we have nothing to copy before it
        if((_uiIndex + 1) * (*_pAllocator)->m_ullBlockSize > (*_pAllocator)->m_ullBlockSize)
        {
            memcpy(_allocator->m_pMemory, (*_pAllocator)->m_pMemory, _uiIndex * (*_pAllocator)->m_ullBlockSize);
        }

    // Egde case: if it is the last index we have nothing to copy behind it
        if((_uiIndex + 1) * (*_pAllocator)->m_ullBlockSize < (*_pAllocator)->m_ullCapacity)
        {
            memcpy(
                _allocator->m_pMemory + (_uiIndex * (*_pAllocator)->m_ullBlockSize),
                (*_pAllocator)->m_pMemory + ((_uiIndex + 1) * (*_pAllocator)->m_ullBlockSize),
                (*_pAllocator)->m_ullCapacity - ((_uiIndex + 1) * (*_pAllocator)->m_ullBlockSize)
            );
        }

        __fixed_vector_allocator* _ptr = *_pAllocator;

        *_pAllocator = _allocator;

        _pInterface->m_fnFreeListFree_(_pParentAllocator, _ptr);
    }

    void impl_fixed_vector_clear(__fixed_vector_allocator* _pAllocator)
    {
        memset(_pAllocator->m_pMemory, 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}