#pragma once

#include "Memory/FixedQueueAllocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Queue
    {
    private:
        __fixed_queue_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Queue()
        {
            m_pMemoryInterface = 0;
            m_pAllocator = 0;
        }

    // TODO: Fix allocating other allocators in Allocator.cpp
        Queue(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnFreeListAllocateFixedQueueAllocator(_pMemoryInterface, _pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Queue(IMemory* _pMemoryInterface, __fixed_queue_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {

        }

        ~Queue()
        {

        }

        inline Type* Allocate(const Type& _data)
        {
            return fixed_queue_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        inline const Type& Begin() const
        {
            return *(Type*)fixed_queue_begin(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Empty() const
        {
            return fixed_queue_empty(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Full() const
        {
            return fixed_queue_full(m_pMemoryInterface, m_pAllocator);
        }

        inline void Pop()
        {
            fixed_queue_pop(m_pMemoryInterface, m_pAllocator);
        }

        inline std::size_t GetSize() const
        {
            return fixed_queue_size(m_pMemoryInterface, m_pAllocator);
        }

        inline std::size_t GetCapacity() const
        {
            return fixed_queue_capacity(m_pMemoryInterface, m_pAllocator);
        }

        inline const __fixed_queue_allocator* GetAllocator() const { return m_pAllocator; }
    };

}}