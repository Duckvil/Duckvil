#pragma once

#include "Memory/Memory.h"
#include "Memory/FreeListAllocator.h"

#include "Memory/Vector.h"

namespace Duckvil { namespace Memory {

    class FreeList
    {
    private:
        __allocator* m_pAllocator;
        IMemory* m_pMemory;
        __free_list_allocator* m_pContainer;

    public:
        FreeList(IMemory* _pMemory, __linear_allocator* _pAllocator, std::size_t _ullSize) :
            m_pAllocator(_pAllocator),
            m_pMemory(_pMemory)
        {
            m_pContainer = _pMemory->m_fnAllocateFreeListAllocator(_pAllocator, _ullSize);
        }

        FreeList(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize) :
            m_pAllocator(_pAllocator),
            m_pMemory(_pMemory)
        {
            // m_pContainer = _pMemory->m_fnAllocateFreeListAllocator(_pAllocator, _ullSize);
        }

        ~FreeList()
        {

        }

        template <typename Type>
        void Allocate(Vector<Type>& _container, std::size_t _ullCount)
        {
            _container = Vector<Type>(m_pMemory, m_pContainer, _ullCount);
        }

    };

}}