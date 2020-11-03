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
        __fixed_vector_allocator* m_pContainer;
        IMemory* m_pMemoryInterface;
        void (*m_fnCopy)(IMemory* _pMemoryInterface, const Vector& _vector, Vector* _pThis);
        void (*m_fnDestruct)(IMemory* _pMemoryInterface, __allocator*, __fixed_vector_allocator* _pThis);
        void (*m_fnResize)(IMemory* _pMemoryInterface, __allocator* _pAllocator, __fixed_vector_allocator** _pContainer, std::size_t _ullNewSize);
        __allocator* m_pAllocator;

        static void free_list_copy(IMemory* _pMemoryInterface, const Vector& _vector, Vector* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_vector.m_pAllocator;

            _pThis->m_pContainer = _pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _allocator, sizeof(Type) * _vector.Size(), sizeof(Type), alignof(Type));
            _pThis->m_fnCopy = _vector.m_fnCopy;
            _pThis->m_fnDestruct = _vector.m_fnDestruct;
            _pThis->m_pAllocator = _vector.m_pAllocator;
        }

        static void free_list_destruct(IMemory* _pMemoryInterface, __allocator* _pAllocator, __fixed_vector_allocator* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pAllocator;

            _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis);
        }

        static void free_list_resize(IMemory* _pMemoryInterface, __allocator* _pAllocator, __fixed_vector_allocator** _pContainer, std::size_t _ullNewSize)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pAllocator;

            fixed_vector_resize(_pMemoryInterface, _allocator, _pContainer, _ullNewSize);
        }

    public:
        Vector()
        {
            m_pMemoryInterface = nullptr;
            m_pContainer = nullptr;
            m_pAllocator = nullptr;
            // m_fnCopy = &free_list_copy;
            // m_fnDestruct = &free_list_destruct;
        }

        Vector(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pAllocator(_pAllocator)
        {
            m_fnCopy = &free_list_copy;
            m_fnDestruct = &free_list_destruct;
            m_fnResize = &free_list_resize;
            m_pContainer = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
        }

        Vector(IMemory* _pMemoryInterface, __fixed_vector_allocator* _pAllocator) :
            m_pMemoryInterface(_pMemoryInterface),
            m_pContainer(_pAllocator)
        {
            m_pAllocator = 0;
        }

        Vector(const Vector& _vector) :
            m_pMemoryInterface(_vector.m_pMemoryInterface)
        {
            _vector.m_fnCopy(m_pMemoryInterface, _vector, this);
        }

        Vector(Vector&& _vector) noexcept :
            m_pContainer(std::move(_vector.m_pContainer)),
            m_pMemoryInterface(std::move(_vector.m_pMemoryInterface)),
            m_fnCopy(std::move(_vector.m_fnCopy)),
            m_fnDestruct(std::move(_vector.m_fnDestruct)),
            m_pAllocator(std::move(_vector.m_pAllocator))
        {
            _vector.m_pMemoryInterface = nullptr;
            _vector.m_pContainer = nullptr;
            _vector.m_pAllocator = nullptr;
            _vector.m_fnCopy = nullptr;
            _vector.m_fnDestruct = nullptr;
        }

        ~Vector()
        {
            if(m_pAllocator == nullptr)
            {
                return;
            }

            m_fnDestruct(m_pMemoryInterface, m_pAllocator, m_pContainer);
        }

        Vector& operator=(Vector&& _vector)
        {
            if(&_vector == this)
            {
                return *this;
            }

            m_pContainer = std::move(_vector.m_pContainer);
            m_pMemoryInterface = std::move(_vector.m_pMemoryInterface);
            m_fnCopy = std::move(_vector.m_fnCopy);
            m_fnDestruct = std::move(_vector.m_fnDestruct);
            m_pAllocator = std::move(_vector.m_pAllocator);

            _vector.m_pContainer = nullptr;
            _vector.m_pMemoryInterface = nullptr;
            _vector.m_fnCopy = nullptr;
            _vector.m_fnDestruct = nullptr;
            _vector.m_pAllocator = nullptr;

            return *this;
        }

        Vector& operator=(const Vector& _vector)
        {
            m_pMemoryInterface = _vector.m_pMemoryInterface;

            _vector.m_fnCopy(m_pMemoryInterface, _vector, this);

            return *this;
        }

        void* Allocate(const Type& _value)
        {
            return fixed_vector_allocate(m_pMemoryInterface, m_pContainer, _value);
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_vector_at(m_pMemoryInterface, m_pContainer, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_vector_at(m_pMemoryInterface, m_pContainer, _ullIndex);
        }

        std::size_t Size() const
        {
            return fixed_vector_size(m_pMemoryInterface, m_pContainer) / sizeof(Type);
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
            return *(Type*)fixed_vector_begin(m_pMemoryInterface, m_pContainer);
        }

        Type& Begin()
        {
            return *(Type*)fixed_vector_begin(m_pMemoryInterface, m_pContainer);
        }

        const Type& Back() const
        {
            return *(Type*)fixed_vector_back(m_pMemoryInterface, m_pContainer);
        }

        Type& Back()
        {
            return *(Type*)fixed_vector_back(m_pMemoryInterface, m_pContainer);
        }

        bool Empty() const
        {
            return fixed_vector_empty(m_pMemoryInterface, m_pContainer);
        }

        bool Full() const
        {
            return fixed_vector_full(m_pMemoryInterface, m_pContainer);
        }

        void Resize(std::size_t _ullNewSize)
        {
            m_fnResize(m_pMemoryInterface, m_pAllocator, &m_pContainer, _ullNewSize);
        }

        Iterator begin()
        {
            return Iterator((Type*)m_pContainer->memory);
        }

        Iterator end()
        {
            return Iterator((Type*)(m_pContainer->memory + m_pContainer->used));
        }

        ConstIterator cbegin()
        {
            return ConstIterator((Type*)m_pContainer->memory);
        }

        ConstIterator cend()
        {
            return ConstIterator((Type*)(m_pContainer->memory + m_pContainer->used));
        }
    };

    // template <typename Type, typename Allocator>
    // class Vector : public BaseVector<Type, Allocator>
    // {
    // public:
    //     Vector()
    //     {
    //         m_pContainer = 0;
    //         m_pMemoryInterface = 0;
    //     }

    //     Vector(IMemory* _pMemoryInterface, Allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         m_pMemoryInterface = _pMemoryInterface;
    //         m_pContainer = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
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
    //         m_pContainer = 0;
    //         m_pMemoryInterface = 0;
    //     }

    //     Vector(__free_list_allocator* _pAllocator)
    //     {
    //         m_pAlloc = _pAllocator;
    //     }

    //     Vector(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         m_pMemoryInterface = _pMemoryInterface;
    //         m_pContainer = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
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
    //         m_pContainer = 0;
    //         m_pMemoryInterface = 0;
    //     }

    //     Vector(IMemory* _pMemoryInterface, __linear_allocator* _pAllocator, std::size_t _ullCount)
    //     {
    //         m_pMemoryInterface = _pMemoryInterface;
    //         // m_pContainer = m_pMemoryInterface->m_fnFreeListAllocateVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
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