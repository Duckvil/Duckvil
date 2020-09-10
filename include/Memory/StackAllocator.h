#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* stack_allocate(IMemory* _pMemory, __stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* stack_allocate(IMemory* _pMemory, __stack_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnStackAllocateCStr_(_pAllocator, _pData);
    }

    void* stack_top(IMemory* _pMemory, __stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnStackTop_(_pAllocator);
    }

    void stack_pop(IMemory* _pMemory, __stack_allocator* _pAllocator)
    {
        _pMemory->m_fnStackPop_(_pAllocator);
    }

    bool stack_empty(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool stack_full(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    size_t stack_size(__stack_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    size_t stack_capacity(__stack_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

    class Stack
    {
    private:
        __stack_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Stack(IMemory* _pMemoryInterface, __allocator* _pAllocator, size_t _ullSize) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateStackAllocator(_pAllocator, _ullSize);
        }

        ~Stack()
        {

        }

        template <typename Type>
        Type* Allocate(const Type& _data)
        {
            return stack_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        void* Top() const
        {
            return m_pMemoryInterface->m_fnStackTop_(m_pAllocator);
        }

        void Pop()
        {
            m_pMemoryInterface->m_fnStackPop_(m_pAllocator);
        }

        bool Empty() const
        {
            return m_pMemoryInterface->m_fnStackEmpty_(m_pAllocator);
        }

        bool Full() const
        {
            return m_pMemoryInterface->m_fnStackFull_(m_pAllocator);
        }
    };

}}