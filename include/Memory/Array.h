#pragma once

#include "Memory/FixedArrayAllocator.h"
#include "Memory/Iterator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Array
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

    private:
        __fixed_array_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Array(IMemory* _pMemoryInterface, __allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedArrayAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Array(IMemory* _pMemoryInterface, __fixed_array_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {

        }

        ~Array()
        {

        }

        inline void* Allocate(const Type& _value)
        {
            return fixed_array_allocate(m_pMemoryInterface, m_pAllocator, _value);
        }

        inline const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_array_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        inline Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_array_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        inline const Type& operator[](std::size_t _ullIndex) const
        {
            return At(_ullIndex);
        }

        inline Type& operator[](std::size_t _ullIndex)
        {
            return At(_ullIndex);
        }

        inline const Type& Begin() const
        {
            return *(Type*)fixed_array_begin(m_pMemoryInterface, m_pAllocator);
        }

        inline Type& Begin()
        {
            return *(Type*)fixed_array_begin(m_pMemoryInterface, m_pAllocator);
        }

        inline const Type& Back() const
        {
            return *(Type*)fixed_array_back(m_pMemoryInterface, m_pAllocator);
        }

        inline Type& Back()
        {
            return *(Type*)fixed_array_back(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Empty() const
        {
            return fixed_array_empty(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Full() const
        {
            return fixed_array_full(m_pMemoryInterface, m_pAllocator);
        }

        inline Iterator begin()
        {
            return Iterator((Type*)m_pAllocator->memory);
        }

        inline Iterator end()
        {
            return Iterator((Type*)(m_pAllocator->memory + m_pAllocator->used));
        }

        inline ConstIterator cbegin()
        {
            return ConstIterator((Type*)m_pAllocator->memory);
        }

        inline ConstIterator cend()
        {
            return ConstIterator((Type*)(m_pAllocator->memory + m_pAllocator->used));
        }
    };

}}