#pragma once

#include "Memory/FixedVectorAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/FreeListAllocator.h"
#include "Memory/SpecifiedResizableContainer.h"

#include <type_traits>
#include <utility>
#include <cstring>

namespace Duckvil { namespace Memory {

// Copied Vector will allocate new memory and copy data from which we copied it

    template <typename Type>
    class Vector : public SpecifiedResizableContainer<Type, fixed_vector_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

        using SContainer = SpecifiedResizableContainer<Type, fixed_vector_allocator>;

        // typedef void (*resize_callback)(IMemory* _pMemoryInterface, SpecifiedContainer* _pThis, std::size_t _ullNewSize);

    private:
        // resize_callback m_fnResize;

        static void free_list_copy(IMemory* _pMemoryInterface, const SpecifiedContainer<Type, fixed_vector_allocator>& _specifiedContainer, SpecifiedContainer<Type, fixed_vector_allocator>* _pThis)
        {
            free_list_allocator* _allocator = (free_list_allocator*)_specifiedContainer.m_pAllocator;
            const Vector& _vec = (const Vector&)_specifiedContainer;

            // _pThis->m_pContainer = free_list_allocate_allocator<fixed_vector_allocator>(_pMemoryInterface, _allocator, sizeof(Type) * _vec.Size(), sizeof(Type), alignof(Type));

            _pThis->m_pContainer = (fixed_vector_allocator*)free_list_allocate(_pMemoryInterface, _allocator, sizeof(fixed_vector_allocator) + (sizeof(Type) * _vec.Size()), alignof(fixed_vector_allocator));

            memcpy(_pThis->m_pContainer, _specifiedContainer.m_pContainer, sizeof(fixed_vector_allocator) + (sizeof(Type) * _vec.Size()));

            // DUCKVIL_DEBUG_MEMORY(_pThis->m_pContainer, "Copied Vector");

            _pThis->m_fnCopy = _specifiedContainer.m_fnCopy;
            _pThis->m_fnDestruct = _specifiedContainer.m_fnDestruct;
            ((Vector<Type>*)_pThis)->m_fnResize = _vec.m_fnResize;
            _pThis->m_pAllocator = _specifiedContainer.m_pAllocator;
        }

        static void free_list_destruct(IMemory* _pMemoryInterface, allocator* _pAllocator, SpecifiedContainer<Type, fixed_vector_allocator>* _pThis)
        {
            free_list_allocator* _allocator = (free_list_allocator*)_pAllocator;
            uint32_t _size = fixed_vector_size(_pMemoryInterface, _pThis->m_pContainer) / sizeof(Type);

            if(std::is_base_of<SContainer, Type>::value)
            {
                for(uint32_t i = 0; i < _size; ++i)
                {
                    SContainer* _container = (SContainer*)fixed_vector_at(_pMemoryInterface, _pThis->m_pContainer, i);

                    _container->m_fnDestruct(_pMemoryInterface, _pAllocator, _container);
                }
            }
            else
            {
                for(uint32_t i = 0; i < _size; ++i)
                {
                    ((Type*)fixed_vector_at(_pMemoryInterface, _pThis->m_pContainer, i))->~Type();
                }
            }

            free_list_free(_pMemoryInterface, _allocator, _pThis->m_pContainer);
        }

        static void free_list_resize(IMemory* _pMemoryInterface, SContainer* _pThis, std::size_t _ullNewSize)
        {
            free_list_allocator* _allocator = (free_list_allocator*)_pThis->m_pAllocator;

            fixed_vector_resize(_pMemoryInterface, _allocator, &_pThis->m_pContainer, _ullNewSize);
        }

        static void free_list_erase(IMemory* _pMemoryInterface, SContainer* _pThis, uint32_t _uiIndex)
        {
            free_list_allocator* _allocator = (free_list_allocator*)_pThis->m_pAllocator;

            fixed_vector_erase(_pMemoryInterface, _allocator, &_pThis->m_pContainer, _uiIndex);
        }

    public:
        Vector()
        {

        }

        Vector(IMemory* _pMemoryInterface, free_list_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            SContainer::m_fnCopy = &free_list_copy;
            SContainer::m_fnDestruct = &free_list_destruct;
            SContainer::m_fnResize = &free_list_resize;
            SContainer::m_fnErase = &free_list_erase;
            SContainer::m_pContainer = _pMemoryInterface->m_fnFreeListAllocateFixedVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type));
        }

        Vector(const Vector& _vector) :
            SContainer(_vector.m_pMemoryInterface, _vector.m_pAllocator)
        {
            _vector.m_fnCopy(SContainer::m_pMemoryInterface, _vector, this);
        }

        Vector(Vector&& _vector) noexcept :
            SContainer(
                std::move(_vector.m_pMemoryInterface),
                std::move(_vector.m_pAllocator),
                std::move(_vector.m_pContainer),
                std::move(_vector.m_fnCopy),
                std::move(_vector.m_fnDestruct),
                std::move(_vector.m_fnResize),
                std::move(_vector.m_fnErase)
            )
        {
            _vector.m_pMemoryInterface = nullptr;
            _vector.m_pContainer = nullptr;
            _vector.m_pAllocator = nullptr;
            _vector.m_fnCopy = nullptr;
            _vector.m_fnDestruct = nullptr;
            _vector.m_fnResize = nullptr;
            _vector.m_fnErase = nullptr;
        }

        ~Vector()
        {
            if(SContainer::m_pAllocator == nullptr || SContainer::m_pContainer == nullptr)
            {
                return;
            }

            SContainer::m_fnDestruct(SContainer::m_pMemoryInterface, SContainer::m_pAllocator, this);

            SContainer::m_pAllocator = nullptr;
            SContainer::m_pContainer = nullptr;
        }

        Vector& operator=(Vector&& _vector)
        {
            if(&_vector == this)
            {
                return *this;
            }

            SContainer::m_pContainer = std::move(_vector.m_pContainer);
            SContainer::m_pMemoryInterface = std::move(_vector.m_pMemoryInterface);
            SContainer::m_fnCopy = std::move(_vector.m_fnCopy);
            SContainer::m_fnDestruct = std::move(_vector.m_fnDestruct);
            SContainer::m_fnResize = std::move(_vector.m_fnResize);
            SContainer::m_pAllocator = std::move(_vector.m_pAllocator);
            SContainer::m_fnErase = std::move(_vector.m_fnErase);

            _vector.m_pContainer = nullptr;
            _vector.m_pMemoryInterface = nullptr;
            _vector.m_fnCopy = nullptr;
            _vector.m_fnDestruct = nullptr;
            _vector.m_fnResize = nullptr;
            _vector.m_pAllocator = nullptr;
            _vector.m_fnErase = nullptr;

            return *this;
        }

        Vector& operator=(const Vector& _vector)
        {
            SContainer::m_pMemoryInterface = _vector.m_pMemoryInterface;

            _vector.m_fnCopy(SContainer::m_pMemoryInterface, _vector, this);

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

                    _container2.m_pContainer = (typename Type::type*)free_list_allocate(
                        _container.m_pMemoryInterface,
                        (free_list_allocator*)_container.m_pAllocator,
                        sizeof(typename Type::type) + _container.m_pContainer->m_ullCapacity,
                        alignof(typename Type::type)
                    );

                    memcpy(_container2.m_pContainer, _container.m_pContainer, sizeof(typename Type::type));
                    memcpy(
                        _container2.GetWorkingMemory(),
                        _container.GetWorkingMemory(),
                        _container.m_pContainer->m_ullCapacity
                    );

                    return fixed_vector_allocate(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _container2);
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

                    _container2.m_pContainer = (typename Type::type*)free_list_allocate(
                        _container.m_pMemoryInterface,
                        (free_list_allocator*)_container.m_pAllocator,
                        sizeof(typename Type::type) + _container.m_pContainer->m_ullCapacity,
                        alignof(typename Type::type)
                    );

                    memcpy(_container2.m_pContainer, _container.m_pContainer, sizeof(typename Type::type));
                    memcpy(
                        _container2.GetWorkingMemory(),
                        _container.GetWorkingMemory(),
                        _container.m_pContainer->m_ullCapacity
                    );

                    return fixed_vector_allocate(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _container2);
                }
            }
            else
            {
                return fixed_vector_allocate(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _value);
            }
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_vector_at(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_vector_at(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _ullIndex);
        }

        std::size_t Size() const
        {
            return fixed_vector_size(SContainer::m_pMemoryInterface, SContainer::m_pContainer) / sizeof(Type);
        }

        std::size_t Capacity() const
        {
            return fixed_vector_capacity(SContainer::m_pMemoryInterface, SContainer::m_pContainer) / sizeof(Type);
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
            return *(Type*)fixed_vector_begin(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        Type& Begin()
        {
            return *(Type*)fixed_vector_begin(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        const Type& Back() const
        {
            return *(Type*)fixed_vector_back(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        Type& Back()
        {
            return *(Type*)fixed_vector_back(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        void Clear()
        {
            fixed_vector_clear(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        bool Empty() const
        {
            return fixed_vector_empty(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        bool Full() const
        {
            return fixed_vector_full(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        void Resize(std::size_t _ullNewSize)
        {
            std::size_t _size = Capacity();

            if(_ullNewSize == _size)
            {
                return;
            }

            SContainer::m_fnResize(SContainer::m_pMemoryInterface, this, _ullNewSize);
        }

        void Erase(uint32_t _uiIndex)
        {
            SContainer::m_fnErase(SContainer::m_pMemoryInterface, this, _uiIndex);
        }

        Iterator begin() const
        {
            // return Iterator((Type*)SContainer::m_pContainer->m_pMemory);
            return Iterator((Type*)fixed_vector_begin(SContainer::m_pMemoryInterface, SContainer::m_pContainer));
        }

        Iterator end() const
        {
            // return Iterator((Type*)(SContainer::m_pContainer->m_pMemory + SContainer::m_pContainer->m_ullUsed));
            return Iterator((Type*)fixed_vector_end(SContainer::m_pMemoryInterface, SContainer::m_pContainer));
        }

        ConstIterator cbegin() const
        {
            return ConstIterator((Type*)SContainer::GetWorkingMemory());
        }

        ConstIterator cend() const
        {
            return ConstIterator((Type*)((uint8_t*)SContainer::GetWorkingMemory() + SContainer::m_pContainer->m_ullUsed));
        }
    };

}}