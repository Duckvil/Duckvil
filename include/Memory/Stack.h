#pragma once

#include "Memory/FixedStackAllocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Stack
    {
    private:
        __fixed_stack_allocator* m_pContainer;
        IMemory* m_pMemoryInterface;
        __allocator* m_pAllocator;

    public:
        Stack()
        {
            m_pContainer = nullptr;
            m_pAllocator = nullptr;
            m_pMemoryInterface = nullptr;
        }

        Stack(IMemory* _pMemoryInterface, __linear_allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pContainer = m_pMemoryInterface->m_fnLinearAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Stack(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            // m_pContainer = m_pMemoryInterface->m_fnLinearAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Stack(IMemory* _pMemoryInterface, __fixed_stack_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pContainer(_pAllocator)
        {

        }

        ~Stack()
        {

        }

        inline Type* Allocate(const Type& _data)
        {
            return fixed_stack_allocate(m_pMemoryInterface, m_pContainer, _data);
        }

        inline const Type& Top() const
        {
            return *(Type*)fixed_stack_top(m_pMemoryInterface, m_pContainer);
        }

        inline void Pop()
        {
            fixed_stack_pop(m_pMemoryInterface, m_pContainer);
        }

        inline bool Empty() const
        {
            return fixed_stack_empty(m_pMemoryInterface, m_pContainer);
        }

        inline bool Full() const
        {
            return fixed_stack_full(m_pMemoryInterface, m_pContainer);
        }

        inline std::size_t GetSize() const
        {
            return fixed_stack_size(m_pMemoryInterface, m_pContainer);
        }

        inline std::size_t GetCapacity() const
        {
            return fixed_stack_capacity(m_pMemoryInterface, m_pContainer);
        }
    };

}}