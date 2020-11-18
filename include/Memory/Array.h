#pragma once

#include "Memory/FixedArrayAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/SpecifiedContainer.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Array : public SpecifiedContainer<Type, __fixed_array_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

    private:
        static void free_list_copy(IMemory* _pMemoryInterface, const SpecifiedContainer& _specifiedContainer, SpecifiedContainer* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_specifiedContainer.m_pAllocator;
            const Array& _array = (const Array&)_specifiedContainer;

            _pThis->m_pContainer = (__fixed_array_allocator*)free_list_allocate(_pMemoryInterface, _allocator, sizeof(__fixed_array_allocator) + (sizeof(Type) * _array.Size()), alignof(__fixed_array_allocator));

            memcpy(_pThis->m_pContainer, _specifiedContainer.m_pContainer, sizeof(__fixed_array_allocator) + (sizeof(Type) * _array.Size()));

            _pThis->m_fnCopy = _specifiedContainer.m_fnCopy;
            _pThis->m_fnDestruct = _specifiedContainer.m_fnDestruct;
            _pThis->m_pAllocator = _specifiedContainer.m_pAllocator;
        }

        static void free_list_destruct(IMemory* _pMemoryInterface, __allocator* _pAllocator, SpecifiedContainer* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pAllocator;
            uint32_t _size = fixed_array_size(_pMemoryInterface, _pThis->m_pContainer) / sizeof(Type);

            if(std::is_base_of<SpecifiedContainer<Type, __fixed_array_allocator>, Type>::value)
            {
                for(uint32_t i = 0; i < _size; i++)
                {
                    SpecifiedContainer<Type, __fixed_array_allocator>* _container = (SpecifiedContainer<Type, __fixed_array_allocator>*)fixed_array_at(_pMemoryInterface, _pThis->m_pContainer, i);

                    _container->m_fnDestruct(_pMemoryInterface, _pAllocator, _container);
                }
            }
            else
            {
                for(uint32_t i = 0; i < _size; i++)
                {
                    ((Type*)fixed_array_at(_pMemoryInterface, _pThis->m_pContainer, i))->~Type();
                }
            }

            _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis->m_pContainer);
        }

    public:
        Array()
        {
            m_pMemoryInterface = nullptr;
            m_pContainer = nullptr;
            m_pAllocator = nullptr;
            m_fnCopy = nullptr;
            m_fnDestruct = nullptr;
        }

        Array(IMemory* _pMemoryInterface, __linear_allocator* _pAllocator, std::size_t _ullCount)
        {
            m_pMemoryInterface = _pMemoryInterface;
            m_pAllocator = _pAllocator;
            m_pContainer = m_pMemoryInterface->m_fnAllocateFixedArrayAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Array(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            SpecifiedContainer(_pMemoryInterface, _pAllocator)
        {
            m_fnCopy = &free_list_copy;
            m_fnDestruct = &free_list_destruct;
            m_pContainer = (__fixed_array_allocator*)free_list_allocate(_pMemoryInterface, _pAllocator, sizeof(__fixed_array_allocator) + (_ullCount * sizeof(Type)), alignof(__fixed_array_allocator));

            m_pContainer->m_ullCapacity = _ullCount * sizeof(Type);
            m_pContainer->m_ullUsed = 0;
            m_pContainer->m_ullBlockSize = sizeof(Type);
            m_pContainer->m_pMemory = (uint8_t*)m_pContainer + sizeof(__fixed_array_allocator);
        }

        Array(const Array& _array) :
            SpecifiedContainer(_array.m_pMemoryInterface, _array.m_pAllocator)
        {
            _array.m_fnCopy(m_pMemoryInterface, _array, this);
        }

        Array(Array&& _array) noexcept :
            SpecifiedContainer(
                std::move(_array.m_pMemoryInterface),
                std::move(_array.m_pAllocator),
                std::move(_array.m_pContainer),
                std::move(_array.m_fnCopy),
                std::move(_array.m_fnDestruct)
            )
        {
            _array.m_pMemoryInterface = nullptr;
            _array.m_pContainer = nullptr;
            _array.m_pAllocator = nullptr;
            _array.m_fnCopy = nullptr;
            _array.m_fnDestruct = nullptr;
        }

        ~Array()
        {
            if(m_pAllocator == nullptr || m_pContainer == nullptr)
            {
                return;
            }

            m_fnDestruct(m_pMemoryInterface, m_pAllocator, this);

            m_pAllocator = nullptr;
            m_pContainer = nullptr;
        }

        Array& operator=(Array&& _array)
        {
            if(&_array == this)
            {
                return *this;
            }

            m_pContainer = std::move(_array.m_pContainer);
            m_pMemoryInterface = std::move(_array.m_pMemoryInterface);
            m_fnCopy = std::move(_array.m_fnCopy);
            m_fnDestruct = std::move(_array.m_fnDestruct);
            m_pAllocator = std::move(_array.m_pAllocator);

            _array.m_pContainer = nullptr;
            _array.m_pMemoryInterface = nullptr;
            _array.m_fnCopy = nullptr;
            _array.m_fnDestruct = nullptr;
            _array.m_pAllocator = nullptr;

            return *this;
        }

        Array& operator=(const Array& _array)
        {
            m_pMemoryInterface = _array.m_pMemoryInterface;

            _array.m_fnCopy(m_pMemoryInterface, _array, this);

            return *this;
        }

        void* Allocate(const Type& _value)
        {
            if constexpr(std::is_base_of<Container, Type>::value)
            {
                const SpecifiedContainer<typename Type::container, typename Type::type>& _container = (const SpecifiedContainer<typename Type::container, typename Type::type>&)_value;

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

                return fixed_array_allocate(m_pMemoryInterface, m_pContainer, _container2);
            }
            else
            {
                return fixed_array_allocate(m_pMemoryInterface, m_pContainer, _value);
            }
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_array_at(m_pMemoryInterface, m_pContainer, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_array_at(m_pMemoryInterface, m_pContainer, _ullIndex);
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
            return *(Type*)fixed_array_begin(m_pMemoryInterface, m_pContainer);
        }

        Type& Begin()
        {
            return *(Type*)fixed_array_begin(m_pMemoryInterface, m_pContainer);
        }

        const Type& Back() const
        {
            return *(Type*)fixed_array_back(m_pMemoryInterface, m_pContainer);
        }

        Type& Back()
        {
            return *(Type*)fixed_array_back(m_pMemoryInterface, m_pContainer);
        }

        bool Empty() const
        {
            return fixed_array_empty(m_pMemoryInterface, m_pContainer);
        }

        bool Full() const
        {
            return fixed_array_full(m_pMemoryInterface, m_pContainer);
        }

        std::size_t Size() const
        {
            return fixed_array_size(m_pMemoryInterface, m_pContainer);
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