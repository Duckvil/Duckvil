#pragma once

#include "Memory/FixedArrayAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/SpecifiedContainer.h"
#include "Memory/FreeListAllocator.h"

#include <type_traits>
#include <utility>
#include <cstring>

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Array : public SpecifiedContainer<Type, fixed_array_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

        using SContainer = SpecifiedContainer<Type, fixed_array_allocator>;

    private:
        static void free_list_copy(ftable* _pMemoryInterface, const SContainer& _specifiedContainer, SContainer* _pThis)
        {
            free_list_allocator* _allocator = (free_list_allocator*)_specifiedContainer.m_pAllocator;
            const Array& _array = (const Array&)_specifiedContainer;

            _pThis->m_pContainer = (fixed_array_allocator*)free_list_allocate(_pMemoryInterface, _allocator, sizeof(fixed_array_allocator) + (sizeof(Type) * _array.Size()), alignof(fixed_array_allocator));

            memcpy(_pThis->m_pContainer, _specifiedContainer.m_pContainer, sizeof(fixed_array_allocator) + (sizeof(Type) * _array.Size()));

            _pThis->m_fnCopy = _specifiedContainer.m_fnCopy;
            _pThis->m_fnDestruct = _specifiedContainer.m_fnDestruct;
            _pThis->m_pAllocator = _specifiedContainer.m_pAllocator;
        }

        static void free_list_destruct(ftable* _pMemoryInterface, allocator* _pAllocator, SContainer* _pThis)
        {
            free_list_allocator* _allocator = (free_list_allocator*)_pAllocator;
            uint32_t _size = fixed_array_size(_pMemoryInterface, _pThis->m_pContainer) / sizeof(Type);

            if(std::is_base_of<SContainer, Type>::value)
            {
                for(uint32_t i = 0; i < _size; ++i)
                {
                    SContainer* _container = (SContainer*)fixed_array_at(_pMemoryInterface, _pThis->m_pContainer, i);

                    _container->m_fnDestruct(_pMemoryInterface, _pAllocator, _container);
                }
            }
            else
            {
                for(uint32_t i = 0; i < _size; ++i)
                {
                    ((Type*)fixed_array_at(_pMemoryInterface, _pThis->m_pContainer, i))->~Type();
                }
            }

            // _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis->m_pContainer);
            free_list_free(_pMemoryInterface, _allocator, _pThis->m_pContainer);
        }

    public:
        Array()
        {

        }

        Array(ftable* _pMemoryInterface, linear_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            SContainer::m_pMemoryInterface = _pMemoryInterface;
            SContainer::m_pAllocator = _pAllocator;
            SContainer::m_pContainer = SContainer::m_pMemoryInterface->m_fnAllocateFixedArrayAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Array(ftable* _pMemoryInterface, free_list_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            SContainer::m_fnCopy = &free_list_copy;
            SContainer::m_fnDestruct = &free_list_destruct;
            SContainer::m_pContainer = _pMemoryInterface->m_fnFreeListAllocateFixedArrayAllocator(_pMemoryInterface, _pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Array(const Array& _array) :
            SContainer(_array.m_pMemoryInterface, _array.m_pAllocator)
        {
            _array.m_fnCopy(SContainer::m_pMemoryInterface, _array, this);
        }

        Array(Array&& _array) noexcept :
            SContainer(
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
            if(SContainer::m_pAllocator == nullptr || SContainer::m_pContainer == nullptr)
            {
                return;
            }

            SContainer::m_fnDestruct(SContainer::m_pMemoryInterface, SContainer::m_pAllocator, this);

            SContainer::m_pAllocator = nullptr;
            SContainer::m_pContainer = nullptr;
        }

        Array& operator=(Array&& _array)
        {
            if(&_array == this)
            {
                return *this;
            }

            SContainer::m_pContainer = std::move(_array.m_pContainer);
            SContainer::m_pMemoryInterface = std::move(_array.m_pMemoryInterface);
            SContainer::m_fnCopy = std::move(_array.m_fnCopy);
            SContainer::m_fnDestruct = std::move(_array.m_fnDestruct);
            SContainer::m_pAllocator = std::move(_array.m_pAllocator);

            _array.m_pContainer = nullptr;
            _array.m_pMemoryInterface = nullptr;
            _array.m_fnCopy = nullptr;
            _array.m_fnDestruct = nullptr;
            _array.m_pAllocator = nullptr;

            return *this;
        }

        Array& operator=(const Array& _array)
        {
            SContainer::m_pMemoryInterface = _array.m_pMemoryInterface;

            _array.m_fnCopy(SContainer::m_pMemoryInterface, _array, this);

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

                return fixed_array_allocate(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _container2);
            }
            else
            {
                return fixed_array_allocate(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _value);
            }
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_array_at(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_array_at(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _ullIndex);
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
            return *(Type*)fixed_array_begin(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        Type& Begin()
        {
            return *(Type*)fixed_array_begin(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        const Type& Back() const
        {
            return *(Type*)fixed_array_back(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        Type& Back()
        {
            return *(Type*)fixed_array_back(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        bool Empty() const
        {
            return fixed_array_empty(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        bool Full() const
        {
            return fixed_array_full(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        std::size_t Size() const
        {
            return fixed_array_size(SContainer::m_pMemoryInterface, SContainer::m_pContainer) / sizeof(Type);
        }

        void Clear() const
        {
            fixed_array_clear(SContainer::m_pMemoryInterface, SContainer::m_pContainer);
        }

        Iterator begin()
        {
            return Iterator(SContainer::GetWorkingMemory());
        }

        Iterator end()
        {
            return Iterator((Type*)((uint8_t*)SContainer::GetWorkingMemory() + SContainer::m_pContainer->m_ullUsed));
        }

        ConstIterator cbegin()
        {
            return ConstIterator(SContainer::GetWorkingMemory());
        }

        ConstIterator cend()
        {
            return ConstIterator((Type*)((uint8_t*)SContainer::GetWorkingMemory() + SContainer::m_pContainer->m_ullUsed));
        }
    };

}}