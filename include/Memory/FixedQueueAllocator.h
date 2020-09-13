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

    void* fixed_queue_begin(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedQueueBegin_(_pAllocator);
    }

    void fixed_queue_pop(IMemory* _pMemory, __fixed_queue_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedQueuePop_(_pAllocator);
    }

    bool fixed_queue_empty(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_queue_full(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    std::size_t fixed_queue_size(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    std::size_t fixed_queue_capacity(__fixed_queue_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

    template <typename Type>
    class Queue
    {
    private:
        __fixed_queue_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Queue(IMemory* _pMemoryInterface, __allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedQueueAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Queue(IMemory* _pMemoryInterface, __fixed_queue_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {

        }

        ~Queue()
        {

        }

        Type* Allocate(const Type& _data)
        {
            return fixed_queue_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        const Type& Begin() const
        {
            return *(Type*)fixed_queue_begin(m_pMemoryInterface, m_pAllocator);
        }

        bool Empty() const
        {
            return fixed_queue_empty(m_pAllocator);
        }

        bool Full() const
        {
            return fixed_queue_full(m_pAllocator);
        }

        void Pop()
        {
            fixed_queue_pop(m_pMemoryInterface, m_pAllocator);
        }
    };

}}