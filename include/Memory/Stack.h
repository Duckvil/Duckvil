#pragma once

#include "Memory/FixedStackAllocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Stack
    {
    private:
        __fixed_stack_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Stack(IMemory* _pMemoryInterface, __linear_allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Stack(IMemory* _pMemoryInterface, __fixed_stack_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {

        }

        ~Stack()
        {

        }

        inline Type* Allocate(const Type& _data)
        {
            return fixed_stack_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        inline const Type& Top() const
        {
            return *(Type*)fixed_stack_top(m_pMemoryInterface, m_pAllocator);
        }

        inline void Pop()
        {
            fixed_stack_pop(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Empty() const
        {
            return fixed_stack_empty(m_pAllocator);
        }

        inline bool Full() const
        {
            return fixed_stack_full(m_pAllocator);
        }

        inline size_t GetSize() const
        {
            return fixed_stack_size(m_pAllocator);
        }

        inline size_t GetCapacity() const
        {
            return fixed_stack_capacity(m_pAllocator);
        }
    };

}}