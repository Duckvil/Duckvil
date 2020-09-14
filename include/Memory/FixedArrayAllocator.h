#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_array_allocate(IMemory* _pMemory, __fixed_array_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedArrayAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* fixed_array_allocate(IMemory* _pMemory, __fixed_array_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnFixedArrayAllocateCStr_(_pAllocator, _pData);
    }

    void* fixed_array_at(IMemory* _pMemory, __fixed_array_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedArrayAt_(_pAllocator, _ullIndex);
    }

    bool fixed_array_empty(IMemory* _pMemory, __fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayEmpty_(_pAllocator);
    }

    bool fixed_array_full(IMemory* _pMemory, __fixed_array_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedArrayFull_(_pAllocator);
    }

    template <typename Type>
    class Array
    {
    private:
        __fixed_array_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Array(IMemory* _pMemoryInterface, __allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedArrayAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Array(IMemory* _pMemoryInterface, __fixed_stack_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {

        }

        ~Array()
        {

        }

        void* Allocate(const Type& _value)
        {
            return fixed_array_allocate(m_pMemoryInterface, m_pAllocator, _value);
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_array_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_array_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        const Type& operator[](std::size_t _ullIndex) const
        {
            return At(_ullIndex);
        }

        Type& operator[](std::size_t _ullIndex)
        {
            return At(_ullIndex);
        }

        bool Empty() const
        {
            return fixed_array_empty(m_pMemoryInterface, m_pAllocator);
        }

        bool Full() const
        {
            return fixed_array_full(m_pMemoryInterface, m_pAllocator);
        }
    };

}}