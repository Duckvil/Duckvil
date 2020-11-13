#pragma once

#include "Memory/FixedVectorAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/FreeListAllocator.h"

namespace Duckvil { namespace Memory {

// Copied Vector will allocate new memory and copy data from which we copied it

    template <typename Type>
    struct Container
    {
        typedef void (*copy_callback)(IMemory* _pMemoryInterface, const Container& _vector, Container* _pThis);
        typedef void (*destruct_callback)(IMemory* _pMemoryInterface, __allocator*, __fixed_vector_allocator* _pThis);
        typedef void (*resize_callback)(IMemory* _pMemoryInterface, Container* _pThis, std::size_t _ullNewSize); 

        Container()
        {
            m_pMemoryInterface = nullptr;
            m_pAllocator = nullptr;
            m_pContainer = nullptr;
        }

        Container(IMemory* _pMemory, __allocator* _pAllocator) :
            m_pMemoryInterface(_pMemory),
            m_pAllocator(_pAllocator)
        {

        }

        Container(IMemory* _pMemory, __allocator* _pAllocator, Type* _pContainer) :
            m_pMemoryInterface(_pMemory),
            m_pAllocator(_pAllocator),
            m_pContainer(_pContainer)
        {

        }

        Container(IMemory* _pMemory, __allocator* _pAllocator, Type* _pContainer, const copy_callback& _fnCopy, const destruct_callback& _fnDestruct, const resize_callback& _fnResize) :
            m_pMemoryInterface(_pMemory),
            m_pAllocator(_pAllocator),
            m_pContainer(_pContainer),
            m_fnCopy(_fnCopy),
            m_fnDestruct(_fnDestruct),
            m_fnResize(_fnResize)
        {

        }

        // Here we do not want to have any copy constructor or operator

        copy_callback m_fnCopy;
        destruct_callback m_fnDestruct;
        resize_callback m_fnResize;

        Type* m_pContainer; // Like fixed_vector_allocator, fixed_stack_allocator, fixed_queue_allocator, etc.
        IMemory* m_pMemoryInterface;
        __allocator* m_pAllocator;
    };

    template <typename Type>
    class Vector : public Container<__fixed_vector_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

    private:
        // __fixed_vector_allocator* m_pContainer;
        // IMemory* m_pMemoryInterface;
        // void (*m_fnCopy)(IMemory* _pMemoryInterface, const Vector& _vector, Vector* _pThis);
        // void (*m_fnDestruct)(IMemory* _pMemoryInterface, __allocator*, __fixed_vector_allocator* _pThis);
        // void (*m_fnResize)(IMemory* _pMemoryInterface, Vector* _pThis, std::size_t _ullNewSize);
        // __allocator* m_pAllocator;

        static void free_list_copy(IMemory* _pMemoryInterface, const Container& _vector, Container* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_vector.m_pAllocator;
            const Vector& _vec = (const Vector&)_vector;

            _pThis->m_pContainer = free_list_allocate_allocator<__fixed_vector_allocator>(_pMemoryInterface, _allocator, sizeof(Type) * _vec.Size(), sizeof(Type), alignof(Type));

            // _pThis->m_pContainer = (__fixed_vector_allocator*)free_list_allocate(_pMemoryInterface, _allocator, sizeof(__fixed_vector_allocator) + (sizeof(Type) * _vec.Size()), alignof(__fixed_vector_allocator));

            memcpy(_pThis->m_pContainer, _vector.m_pContainer, sizeof(__fixed_vector_allocator) + (sizeof(Type) * _vec.Size()));

            _pThis->m_fnCopy = _vector.m_fnCopy;
            _pThis->m_fnDestruct = _vector.m_fnDestruct;
            _pThis->m_fnResize = _vector.m_fnResize;
            _pThis->m_pAllocator = _vector.m_pAllocator;
        }

        static void free_list_destruct(IMemory* _pMemoryInterface, __allocator* _pAllocator, __fixed_vector_allocator* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pAllocator;

            _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis);
        }

        static void free_list_resize(IMemory* _pMemoryInterface, Container* _pThis, std::size_t _ullNewSize)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pThis->m_pAllocator;

            fixed_vector_resize(_pMemoryInterface, _allocator, &_pThis->m_pContainer, _ullNewSize);
        }

    public:
        Vector()
        {
            m_pMemoryInterface = nullptr;
            m_pContainer = nullptr;
            m_pAllocator = nullptr;
            m_fnCopy = nullptr;
            m_fnDestruct = nullptr;
            m_fnResize = nullptr;
        }

        Vector(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            // m_pMemoryInterface(_pMemoryInterface),
            // m_pAllocator(_pAllocator)
            Container(_pMemoryInterface, _pAllocator)
        {
            m_fnCopy = &free_list_copy;
            m_fnDestruct = &free_list_destruct;
            m_fnResize = &free_list_resize;
            m_pContainer = free_list_allocate_allocator<__fixed_vector_allocator>(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
        }

        Vector(const Vector& _vector) :
            Container(_vector.m_pMemoryInterface, _vector.m_pAllocator)
        {
            _vector.m_fnCopy(m_pMemoryInterface, _vector, this);
        }

        Vector(Vector&& _vector) noexcept :
            Container(
                std::move(_vector.m_pMemoryInterface),
                std::move(_vector.m_pAllocator),
                std::move(_vector.m_pContainer),
                std::move(_vector.m_fnCopy),
                std::move(_vector.m_fnDestruct),
                std::move(_vector.m_fnResize)
            )
            // m_pContainer(std::move(_vector.m_pContainer)),
            // m_pMemoryInterface(std::move(_vector.m_pMemoryInterface)),
            // m_fnCopy(std::move(_vector.m_fnCopy)),
            // m_fnDestruct(std::move(_vector.m_fnDestruct)),
            // m_fnResize(std::move(_vector.m_fnResize))
            // m_pAllocator(std::move(_vector.m_pAllocator))
        {
            _vector.m_pMemoryInterface = nullptr;
            _vector.m_pContainer = nullptr;
            _vector.m_pAllocator = nullptr;
            _vector.m_fnCopy = nullptr;
            _vector.m_fnDestruct = nullptr;
            _vector.m_fnResize = nullptr;
        }

        ~Vector()
        {
            if(m_pAllocator == nullptr || m_pContainer == nullptr)
            {
                return;
            }

            if(std::is_base_of<Container<__fixed_vector_allocator>, Type>::value)
            {
                uint32_t _size = Size();

                for(uint32_t i = 0; i < _size; i++)
                {
                    Container<__fixed_vector_allocator>& _container = (Container<__fixed_vector_allocator>&)At(i);

                    m_fnDestruct(m_pMemoryInterface, m_pAllocator, _container.m_pContainer);
                }
            }
            else
            {
                uint32_t _size = Size();

                for (uint32_t i = 0; i < _size; i++)
                {
                    At(i).~Type();
                }
            }

            m_fnDestruct(m_pMemoryInterface, m_pAllocator, m_pContainer);

            m_pAllocator = nullptr;
            m_pContainer = nullptr;
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
            m_fnResize = std::move(_vector.m_fnResize);
            m_pAllocator = std::move(_vector.m_pAllocator);

            _vector.m_pContainer = nullptr;
            _vector.m_pMemoryInterface = nullptr;
            _vector.m_fnCopy = nullptr;
            _vector.m_fnDestruct = nullptr;
            _vector.m_fnResize = nullptr;
            _vector.m_pAllocator = nullptr;

            return *this;
        }

        Vector& operator=(const Vector& _vector)
        {
            m_pMemoryInterface = _vector.m_pMemoryInterface;

            _vector.m_fnCopy(m_pMemoryInterface, _vector, this);

            return *this;
        }

        Vector& operator=(const Container<__fixed_vector_allocator>& _vector)
        {
            m_pMemoryInterface = _vector.m_pMemoryInterface;

            _vector.m_fnCopy(m_pMemoryInterface, _vector, this);

            return *this;
        }

        void* Allocate(const Type& _value)
        {
            if(std::is_base_of<Container<__fixed_vector_allocator>, Type>::value)
            {
                // If we are allocating a container like Vector, Array, Stack, Queue, etc. we need to allocate new memory for it?

                const Container<__fixed_vector_allocator>& _container = (const Container<__fixed_vector_allocator>&)_value;

                Container<__fixed_vector_allocator> _container2;

                _container2.m_fnCopy = _container.m_fnCopy;
                _container2.m_fnDestruct = _container.m_fnDestruct;
                _container2.m_fnResize = _container.m_fnResize;
                _container2.m_pAllocator = _container.m_pAllocator;
                _container2.m_pMemoryInterface = _container.m_pMemoryInterface;

                _container2.m_pContainer = (__fixed_vector_allocator*)free_list_allocate(
                    _container.m_pMemoryInterface,
                    (__free_list_allocator*)_container.m_pAllocator,
                    sizeof(__fixed_vector_allocator) + (fixed_vector_capacity(m_pMemoryInterface, _container.m_pContainer)),
                    alignof(__fixed_vector_allocator)
                );

                _container2.m_pContainer->used = _container.m_pContainer->used;
                _container2.m_pContainer->capacity = _container.m_pContainer->capacity;
                _container2.m_pContainer->m_ullBlockSize = _container.m_pContainer->m_ullBlockSize;
                _container2.m_pContainer->memory = (uint8_t*)_container2.m_pContainer + sizeof(__fixed_vector_allocator);

                memcpy(
                    _container2.m_pContainer->memory,
                    _container.m_pContainer->memory,
                    fixed_vector_size(m_pMemoryInterface, _container.m_pContainer)
                );

                return fixed_vector_allocate(m_pMemoryInterface, m_pContainer, _container2);
            }
            else
            {
                return fixed_vector_allocate(m_pMemoryInterface, m_pContainer, _value);
            }
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

        std::size_t Capacity() const
        {
            return fixed_vector_capacity(m_pMemoryInterface, m_pContainer) / sizeof(Type);
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
            std::size_t _size = Capacity();

            if(_ullNewSize == _size)
            {
                return;
            }

            m_fnResize(m_pMemoryInterface, this, _ullNewSize);
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

}}