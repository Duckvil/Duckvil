#include "Memory/Internal/FixedVectorAllocator.h"

#include <cstring>
#include <stdexcept>

#include "tracy/Tracy.hpp"

#include "Memory/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

    void* impl_fixed_vector_allocate(fixed_vector_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = impl_fixed_vector_allocate(_pAllocator, _ullSize, _ucAlignment);

        memcpy(_memory, _pData, _ullSize);

        return _memory;
    }

    void* impl_fixed_vector_allocate(fixed_vector_allocator* _pAllocator, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        void* _memory = nullptr;

        if(impl_fixed_vector_full(_pAllocator) || _pAllocator->m_ullBlockSize != _ullSize)
        {
            throw std::overflow_error("Vector is full!");

            return _memory;
        }

        uint8_t _padding = 0;
        _memory = calculate_aligned_pointer(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + _pAllocator->m_ullUsed, _ucAlignment, _padding);

        _pAllocator->m_ullUsed += _pAllocator->m_ullBlockSize + _padding;

        TracyAllocN(_memory, _ullSize, "Vector");

        return _memory;
    }

    void* impl_fixed_vector_begin(fixed_vector_allocator* _pAllocator)
    {
        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator);
    }

    void* impl_fixed_vector_back(fixed_vector_allocator* _pAllocator)
    {
        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + _pAllocator->m_ullUsed - _pAllocator->m_ullBlockSize;
    }

    void* impl_fixed_vector_end(fixed_vector_allocator* _pAllocator)
    {
        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + _pAllocator->m_ullUsed;
    }

    void* impl_fixed_vector_at(fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + (_ullIndex * _pAllocator->m_ullBlockSize);
    }

    std::size_t impl_fixed_vector_size(fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed;
    }

    std::size_t impl_fixed_vector_capacity(fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullCapacity;
    }

    bool impl_fixed_vector_empty(fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == 0;
    }

    bool impl_fixed_vector_full(fixed_vector_allocator* _pAllocator)
    {
        return _pAllocator->m_ullUsed == _pAllocator->m_ullCapacity;
    }

    void impl_fixed_vector_resize(ftable* _pInterface, free_list_allocator* _pParentAllocator, fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        if(_pParentAllocator->m_ullCapacity < _ullNewSize + _pParentAllocator->m_ullUsed)
        {
            return;
        }

        TracyMessageL("Vector resize");

        // fixed_vector_allocator* _allocator = (fixed_vector_allocator*)free_list_allocate(_pInterface, _pParentAllocator, sizeof(fixed_vector_allocator) + (_ullNewSize * (*_pAllocator)->m_ullBlockSize), alignof(fixed_vector_allocator));

        // _allocator->m_pMemory = (uint8_t*)_allocator + sizeof(fixed_vector_allocator);
        // _allocator->m_ullCapacity = _ullNewSize * (*_pAllocator)->m_ullBlockSize;
        // _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;
        // _allocator->m_ullBlockSize = (*_pAllocator)->m_ullBlockSize;

        // memcpy(_allocator->m_pMemory, (*_pAllocator)->m_pMemory, (*_pAllocator)->m_ullUsed);

        fixed_vector_allocator* _allocator = _pInterface->m_fnFreeListAllocateFixedVectorAllocator(_pInterface, _pParentAllocator, _ullNewSize * (*_pAllocator)->m_ullBlockSize, (*_pAllocator)->m_ullBlockSize);

        _allocator->m_ullUsed = (*_pAllocator)->m_ullUsed;

        memcpy(reinterpret_cast<uint8_t*>(_allocator) + sizeof(fixed_vector_allocator), reinterpret_cast<uint8_t*>(*_pAllocator) + sizeof(fixed_vector_allocator), (*_pAllocator)->m_ullUsed);

/*#ifdef DUCKVIL_MEMORY_DEBUGGER
        _allocator->m_fnOnAllocate = _pParentAllocator->m_fnOnAllocate;
        _allocator->m_fnOnDeallocate = _pParentAllocator->m_fnOnDeallocate;
        _allocator->m_fnOnAllocate(_pParentAllocator, _allocator, duckvil_memory_allocator_type_vector);
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif*/

#ifdef DUCKVIL_MEMORY_DEBUGGER
        memcpy(_allocator->m_pDebugData->m_aLabel, (*_pAllocator)->m_pDebugData->m_aLabel, 128);
#endif

        fixed_vector_allocator* _ptr = *_pAllocator;

        for(uint32_t i = 0; i < (*_pAllocator)->m_ullUsed / (*_pAllocator)->m_ullBlockSize; ++i)
        {
            TracyFreeN(reinterpret_cast<uint8_t*>((*_pAllocator)) + sizeof(fixed_vector_allocator) + (i * (*_pAllocator)->m_ullBlockSize), "Vector");
        }

        *_pAllocator = _allocator;

        for(uint32_t i = 0; i < (*_pAllocator)->m_ullUsed / (*_pAllocator)->m_ullBlockSize; ++i)
        {
            TracyAllocN(reinterpret_cast<uint8_t*>((*_pAllocator)) + sizeof(fixed_vector_allocator) + (i * (*_pAllocator)->m_ullBlockSize), (*_pAllocator)->m_ullBlockSize, "Vector");
        }

        free_list_free(_pInterface, _pParentAllocator, _ptr);
    }

    void impl_fixed_vector_erase(fixed_vector_allocator* _pAllocator, uint32_t _uiIndex)
    {
        TracyFreeN(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + (_uiIndex * _pAllocator->m_ullBlockSize), "Vector");

        if(_uiIndex == 0 && _pAllocator->m_ullUsed == _pAllocator->m_ullBlockSize)
        {
            memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator), 0, _pAllocator->m_ullUsed);
        }
        else if((_uiIndex + 1) * _pAllocator->m_ullBlockSize >= _pAllocator->m_ullUsed)
        {
            memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + (_uiIndex * _pAllocator->m_ullBlockSize), 0, _pAllocator->m_ullBlockSize);
        }
        else
        {
            memcpy(
                reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + (_uiIndex * _pAllocator->m_ullBlockSize),
                reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + ((_uiIndex + 1) * _pAllocator->m_ullBlockSize),
                _pAllocator->m_ullUsed - ((_uiIndex + 1) * _pAllocator->m_ullBlockSize)
            );
        }

        _pAllocator->m_ullUsed -= _pAllocator->m_ullBlockSize;
    }

    void impl_fixed_vector_clear(fixed_vector_allocator* _pAllocator)
    {
        TracyMessageL("Vector clear");

        for(uint32_t i = 0; i < _pAllocator->m_ullUsed / _pAllocator->m_ullBlockSize; ++i)
        {
            TracyFreeN(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator) + (i * _pAllocator->m_ullBlockSize), "Vector");
        }

        memset(reinterpret_cast<uint8_t*>(_pAllocator) + sizeof(fixed_vector_allocator), 0, _pAllocator->m_ullCapacity);
        _pAllocator->m_ullUsed = 0;
    }

}}