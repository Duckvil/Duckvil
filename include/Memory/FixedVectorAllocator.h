#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

#include "Utils/Utils.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    inline Type* fixed_vector_allocate(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator, const Type& _data)
    {
        return (Type*)_pMemory->m_fnFixedVectorAllocate_(_pAllocator, &_data, sizeof(Type), alignof(Type));
    }

    inline void* fixed_vector_begin(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorBegin_(_pAllocator);
    }

    inline void* fixed_vector_back(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorBack_(_pAllocator);
    }

    inline void* fixed_vector_at(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedVectorAt_(_pAllocator, _ullIndex);
    }

    inline bool fixed_vector_empty(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorEmpty_(_pAllocator);
    }

    inline bool fixed_vector_full(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorFull_(_pAllocator);
    }

    inline void fixed_vector_resize(IMemory* _pMemory, __free_list_allocator* m_pParentAllocator, __fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_fnFixedVectorResize_(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

    inline std::size_t fixed_vector_size(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorSize_(_pAllocator);
    }

    inline std::size_t fixed_vector_capacity(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorCapacity_(_pAllocator);
    }

    inline void fixed_vector_erase(IMemory* _pMemory, __free_list_allocator* m_pParentAllocator, __fixed_vector_allocator** _pAllocator, uint32_t _uiIndex)
    {
        _pMemory->m_fnFixedVectorErase_(_pMemory, m_pParentAllocator, _pAllocator, _uiIndex);
    }

}}