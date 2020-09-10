#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnFixedStackAllocateCStr_(_pAllocator, _pData);
    }

    template <typename Type>
    class FixedStack
    {
    private:
        __fixed_stack_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        FixedStack(IMemory* _pMemoryInterface, __allocator* _pAllocator, size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        ~FixedStack()
        {

        }

        Type* Allocate(const Type& _data)
        {
            return fixed_stack_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        const Type& Top() const
        {
            return *(Type*)m_pMemoryInterface->m_fnFixedStackTop_(m_pAllocator);
        }

        void Pop()
        {
            m_pMemoryInterface->m_fnFixedStackPop_(m_pAllocator);
        }

        bool Empty() const
        {
            return m_pMemoryInterface->m_fnFixedStackEmpty_(m_pAllocator);
        }

        bool Full() const
        {
            return m_pMemoryInterface->m_fnFixedStackFull_(m_pAllocator);
        }
    };

}}