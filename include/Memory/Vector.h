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
        Vector()
        {
            m_pMemoryInterface = 0;
            m_pAllocator = 0;
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

        inline void* Allocate(const Type& _value)
        {
            return fixed_vector_allocate(m_pMemoryInterface, m_pAllocator, _value);
        }

        inline const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_vector_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        inline Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_vector_at(m_pMemoryInterface, m_pAllocator, _ullIndex);
        }

        inline std::size_t Size() const
        {
            return fixed_vector_size(m_pMemoryInterface, m_pAllocator) / sizeof(Type);
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
            return *(Type*)fixed_vector_begin(m_pMemoryInterface, m_pAllocator);
        }

        inline Type& Begin()
        {
            return *(Type*)fixed_vector_begin(m_pMemoryInterface, m_pAllocator);
        }

        inline const Type& Back() const
        {
            return *(Type*)fixed_vector_back(m_pMemoryInterface, m_pAllocator);
        }

        inline Type& Back()
        {
            return *(Type*)fixed_vector_back(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Empty() const
        {
            return fixed_vector_empty(m_pMemoryInterface, m_pAllocator);
        }

        inline bool Full() const
        {
            return fixed_vector_full(m_pMemoryInterface, m_pAllocator);
        }

        inline void Resize(__free_list_allocator* _pParentAllocator, std::size_t _ullNewSize)
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

    // template <typename Type, typename Allocator>
    // class Vector : public BaseVector<Type, Allocator>
    // {
    // public:
    //     Vector()
    //     {
    //         m_pAllocator = 0;
    //         m_pMemoryInterface = 0;
    //     }

    //     Vector(IMemory* _pMemoryInterface, Allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         m_pMemoryInterface = _pMemoryInterface;
    //         m_pAllocator = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
    //     }

    //     Vector(const Vector<Type, Allocator>& _vector)
    //     {

    //     }
    // };

    // template <typename Type>
    // class Vector<Type, __free_list_allocator> : public BaseVector<Type, __free_list_allocator>
    // {
    //     __free_list_allocator* m_pAlloc;
    // public:
    //     Vector()
    //     {
    //         m_pAllocator = 0;
    //         m_pMemoryInterface = 0;
    //     }

    //     Vector(__free_list_allocator* _pAllocator)
    //     {
    //         m_pAlloc = _pAllocator;
    //     }

    //     Vector(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         m_pMemoryInterface = _pMemoryInterface;
    //         m_pAllocator = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
    //     }

    //     Vector(const Vector& _vector)
    //     {

    //     }
    // };

    // template <typename Type>
    // class Vector<Type, __linear_allocator> : public BaseVector<Type, __linear_allocator>
    // {
    // public:
    //     Vector()
    //     {
    //         m_pAllocator = 0;
    //         m_pMemoryInterface = 0;
    //     }

    //     Vector(IMemory* _pMemoryInterface, __linear_allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         m_pMemoryInterface = _pMemoryInterface;
    //         // m_pAllocator = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
    //     }

    //     Vector(const Vector& _vector)
    //     {

    //     }
    // };

    // template <typename Type>
    // struct VectorFactory
    // {
    //     template <typename Allocator>
    //     static auto Create(IMemory* _pMemoryInterface, Allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         return Vector<Type, Allocator>(_pMemoryInterface, _pAllocator, _ullCount);
    //     }
    // };

}}