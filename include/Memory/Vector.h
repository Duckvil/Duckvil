#pragma once

#include "Memory/FixedVectorAllocator.h"
#include "Memory/Iterator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Vector
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

    private:
        __fixed_vector_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        Vector(IMemory* _pMemoryInterface, __allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedArrayAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Vector(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
        }

        Vector(IMemory* _pMemoryInterface, __fixed_vector_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {

        }

        ~Vector()
        {

        }

        void* Allocate(const Type& _value)
        {
            return fixed_vector_allocate(m_pMemoryInterface, m_pAllocator, _value);
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_vector_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_vector_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        const Type& operator[](std::size_t _ullIndex) const
        {
            return At(_ullIndex);
        }

        Type& operator[](std::size_t _ullIndex)
        {
            return At(_ullIndex);
        }

        const Type& Begin() const
        {
            return *(Type*)fixed_vector_begin(m_pMemoryInterface, m_pAllocator);
        }

        Type& Begin()
        {
            return *(Type*)fixed_vector_begin(m_pMemoryInterface, m_pAllocator);
        }

        const Type& Back() const
        {
            return *(Type*)fixed_vector_back(m_pMemoryInterface, m_pAllocator);
        }

        Type& Back()
        {
            return *(Type*)fixed_vector_back(m_pMemoryInterface, m_pAllocator);
        }

        bool Empty() const
        {
            return fixed_vector_empty(m_pMemoryInterface, m_pAllocator);
        }

        bool Full() const
        {
            return fixed_vector_full(m_pMemoryInterface, m_pAllocator);
        }

        void Resize(__free_list_allocator* _pParentAllocator, std::size_t _ullNewSize)
        {
            fixed_vector_resize(m_pMemoryInterface, _pParentAllocator, &m_pAllocator, _ullNewSize);
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