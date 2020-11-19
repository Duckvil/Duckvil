#pragma once

#include "Memory/FixedVectorAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/FreeListAllocator.h"
#include "Memory/SpecifiedResizableContainer.h"

namespace Duckvil { namespace Memory {

// Copied Vector will allocate new memory and copy data from which we copied it

    template <typename Type>
    class Vector : public SpecifiedResizableContainer<Type, __fixed_vector_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

        // typedef void (*resize_callback)(IMemory* _pMemoryInterface, SpecifiedContainer* _pThis, std::size_t _ullNewSize);

    private:
        // resize_callback m_fnResize;

        static void free_list_copy(IMemory* _pMemoryInterface, const SpecifiedContainer<Type, __fixed_vector_allocator>& _specifiedContainer, SpecifiedContainer<Type, __fixed_vector_allocator>* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_specifiedContainer.m_pAllocator;
            const Vector& _vec = (const Vector&)_specifiedContainer;

            // _pThis->m_pContainer = free_list_allocate_allocator<__fixed_vector_allocator>(_pMemoryInterface, _allocator, sizeof(Type) * _vec.Size(), sizeof(Type), alignof(Type));

            _pThis->m_pContainer = (__fixed_vector_allocator*)free_list_allocate(_pMemoryInterface, _allocator, sizeof(__fixed_vector_allocator) + (sizeof(Type) * _vec.Size()), alignof(__fixed_vector_allocator));

            memcpy(_pThis->m_pContainer, _specifiedContainer.m_pContainer, sizeof(__fixed_vector_allocator) + (sizeof(Type) * _vec.Size()));

            _pThis->m_fnCopy = _specifiedContainer.m_fnCopy;
            _pThis->m_fnDestruct = _specifiedContainer.m_fnDestruct;
            ((Vector<Type>*)_pThis)->m_fnResize = _vec.m_fnResize;
            _pThis->m_pAllocator = _specifiedContainer.m_pAllocator;
        }

        static void free_list_destruct(IMemory* _pMemoryInterface, __allocator* _pAllocator, SpecifiedContainer<Type, __fixed_vector_allocator>* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pAllocator;
            uint32_t _size = fixed_vector_size(_pMemoryInterface, _pThis->m_pContainer) / sizeof(Type);

            if(std::is_base_of<SpecifiedResizableContainer<Type, __fixed_vector_allocator>, Type>::value)
            {
                for(uint32_t i = 0; i < _size; i++)
                {
                    SpecifiedResizableContainer<Type, __fixed_vector_allocator>* _container = (SpecifiedResizableContainer<Type, __fixed_vector_allocator>*)fixed_vector_at(_pMemoryInterface, _pThis->m_pContainer, i);

                    _container->m_fnDestruct(_pMemoryInterface, _pAllocator, _container);
                }
            }
            else
            {
                for(uint32_t i = 0; i < _size; i++)
                {
                    ((Type*)fixed_vector_at(_pMemoryInterface, _pThis->m_pContainer, i))->~Type();
                }
            }

            _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis->m_pContainer);
        }

        static void free_list_resize(IMemory* _pMemoryInterface, SpecifiedResizableContainer* _pThis, std::size_t _ullNewSize)
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
            SpecifiedResizableContainer(_pMemoryInterface, _pAllocator)
        {
            m_fnCopy = &free_list_copy;
            m_fnDestruct = &free_list_destruct;
            m_fnResize = &free_list_resize;
            m_pContainer = free_list_allocate_allocator<__fixed_vector_allocator>(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type), alignof(Type));
        }

        Vector(const Vector& _vector) :
            SpecifiedResizableContainer(_vector.m_pMemoryInterface, _vector.m_pAllocator)
        {
            _vector.m_fnCopy(m_pMemoryInterface, _vector, this);
        }

        Vector(Vector&& _vector) noexcept :
            /*SpecifiedContainer(
                std::move(_vector.m_pMemoryInterface),
                std::move(_vector.m_pAllocator),
                std::move(_vector.m_pContainer),
                std::move(_vector.m_fnCopy),
                std::move(_vector.m_fnDestruct)
            ),*/
            SpecifiedResizableContainer(
                std::move(_vector.m_pMemoryInterface),
                std::move(_vector.m_pAllocator),
                std::move(_vector.m_pContainer),
                std::move(_vector.m_fnCopy),
                std::move(_vector.m_fnDestruct),
                std::move(_vector.m_fnResize)
            )
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

            m_fnDestruct(m_pMemoryInterface, m_pAllocator, this);

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

        void* Allocate(const Type& _value)
        {
            if constexpr(std::is_base_of<Container, Type>::value)
            {
                if constexpr(std::is_base_of<SpecifiedResizableContainer<typename Type::container, typename Type::type>, Type>::value)
                {
                    typedef SpecifiedResizableContainer<typename Type::container, typename Type::type> ContainerType;

                    const ContainerType& _container =
                        (const ContainerType&)_value;

                    ContainerType _container2;

                    _container2.m_fnCopy = _container.m_fnCopy;
                    _container2.m_fnDestruct = _container.m_fnDestruct;
                    _container2.m_fnResize = _container.m_fnResize;
                    _container2.m_pAllocator = _container.m_pAllocator;
                    _container2.m_pMemoryInterface = _container.m_pMemoryInterface;

                    _container2.m_pContainer = (Type::type*)free_list_allocate(
                        _container.m_pMemoryInterface,
                        (__free_list_allocator*)_container.m_pAllocator,
                        sizeof(Type::type) + _container.m_pContainer->m_ullCapacity,
                        alignof(Type::type)
                    );

                    memcpy(_container2.m_pContainer, _container.m_pContainer, sizeof(Type::type));

                    _container2.m_pContainer->m_pMemory = (uint8_t*)_container2.m_pContainer + sizeof(Type::type);

                    memcpy(
                        _container2.m_pContainer->m_pMemory,
                        _container.m_pContainer->m_pMemory,
                        _container.m_pContainer->m_ullCapacity
                    );

                    return fixed_vector_allocate(m_pMemoryInterface, m_pContainer, _container2);
                }
                else
                {
                    const SpecifiedContainer<typename Type::container, typename Type::type>& _container =
                        (const SpecifiedContainer<typename Type::container, typename Type::type>&)_value;

                    SpecifiedContainer<typename Type::container, typename Type::type> _container2;

                    _container2.m_fnCopy = _container.m_fnCopy;
                    _container2.m_fnDestruct = _container.m_fnDestruct;
                    _container2.m_pAllocator = _container.m_pAllocator;
                    _container2.m_pMemoryInterface = _container.m_pMemoryInterface;

                    _container2.m_pContainer = (Type::type*)free_list_allocate(
                        _container.m_pMemoryInterface,
                        (__free_list_allocator*)_container.m_pAllocator,
                        sizeof(Type::type) + _container.m_pContainer->m_ullCapacity,
                        alignof(Type::type)
                    );

                    memcpy(_container2.m_pContainer, _container.m_pContainer, sizeof(Type::type));

                    _container2.m_pContainer->m_pMemory = (uint8_t*)_container2.m_pContainer + sizeof(Type::type);

                    memcpy(
                        _container2.m_pContainer->m_pMemory,
                        _container.m_pContainer->m_pMemory,
                        _container.m_pContainer->m_ullCapacity
                    );

                    return fixed_vector_allocate(m_pMemoryInterface, m_pContainer, _container2);
                }
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
            return Iterator((Type*)m_pContainer->m_pMemory);
        }

        Iterator end()
        {
            return Iterator((Type*)(m_pContainer->m_pMemory + m_pContainer->m_ullUsed));
        }

        ConstIterator cbegin()
        {
            return ConstIterator((Type*)m_pContainer->m_pMemory);
        }

        ConstIterator cend()
        {
            return ConstIterator((Type*)(m_pContainer->m_pMemory + m_pContainer->m_ullUsed));
        }
    };

}}