#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_queue_allocate(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedQueueAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* fixed_queue_allocate(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnFixedQueueAllocateCStr_(_pAllocator, _pData);
    }

    template <typename Type>
    class FixedQueue
    {
    private:
        __fixed_queue_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        FixedQueue(IMemory* _pMemoryInterface, __allocator* _pAllocator, size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedQueueAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        ~FixedQueue()
        {

        }

        Type* Allocate(const Type& _data)
        {
            return fixed_queue_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        const Type& Begin() const
        {
            Type* _ptr = (Type*)m_pMemoryInterface->m_fnFixedQueueBegin_(m_pAllocator);

            return *_ptr;
        }

        bool Empty() const
        {
            return m_pMemoryInterface->m_fnFixedQueueEmpty_(m_pAllocator);
        }

        bool Full() const
        {
            return m_pMemoryInterface->m_fnFixedQueueFull_(m_pAllocator);
        }

        void Pop()
        {
            m_pMemoryInterface->m_fnFixedQueuePop_(m_pAllocator);
        }
    };

}}