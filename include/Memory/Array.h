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
        static void free_list_copy(const SContainer& _specifiedContainer, SContainer* _pThis)
        {
            const Array& _array = (const Array&)_specifiedContainer;

            _pThis->m_pContainer = (fixed_array_allocator*)free_list_allocate(_pThis->m_pMemoryInterface, static_cast<free_list_allocator*>(_array.m_pAllocator), sizeof(fixed_array_allocator) + (sizeof(Type) * _array.Size()), alignof(fixed_array_allocator));

            static_cast<fixed_array_allocator*>(_pThis->m_pContainer)->m_ullBlockSize = static_cast<fixed_array_allocator*>(_array.m_pContainer)->m_ullBlockSize;
            static_cast<fixed_array_allocator*>(_pThis->m_pContainer)->m_ullCapacity = static_cast<fixed_array_allocator*>(_array.m_pContainer)->m_ullCapacity;
            static_cast<fixed_array_allocator*>(_pThis->m_pContainer)->m_ullUsed = 0;

            for(uint32_t _i = 0; _i < _array.Size(); ++_i)
            {
                fixed_array_allocate<Type>(&_pThis->m_pMemoryInterface->m_fixedArrayContainer, _pThis->m_pContainer, _array[_i]);
            }

            _pThis->m_fnCopy = _array.m_fnCopy;
            _pThis->m_fnDestruct = _array.m_fnDestruct;
            _pThis->m_pAllocator = _array.m_pAllocator;
        }

        static void free_list_destruct(allocator* _pAllocator, SContainer* _pThis)
        {
            uint32_t _size = fixed_array_size(&_pThis->m_pMemoryInterface->m_fixedArrayContainer, _pThis->m_pContainer) / sizeof(Type);

            for(uint32_t _i = 0; _i < _size; ++_i)
            {
                Type* _data = fixed_array_at<Type>(&_pThis->m_pMemoryInterface->m_fixedArrayContainer, _pThis->m_pContainer, _i);

                _data->~Type();
            }

            // _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis->m_pContainer);
            free_list_free(_pThis->m_pMemoryInterface, static_cast<free_list_allocator*>(_pAllocator), _pThis->m_pContainer);
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
            _array.m_fnCopy(_array, this);
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

            SContainer::m_fnDestruct(SContainer::m_pAllocator, this);

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

            _array.m_fnCopy(_array, this);

            return *this;
        }

        void* Allocate(const Type& _value)
        {
            return fixed_array_allocate(&this->m_pMemoryInterface->m_fixedArrayContainer, this->m_pContainer, _value);
        }

        inline Type* Allocate(Type&& _data)
        {
            return fixed_array_allocate(&this->m_pMemoryInterface->m_fixedArrayContainer, this->m_pContainer, std::move(_data));
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *(Type*)fixed_array_at(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer, _ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            return *(Type*)fixed_array_at(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer, _ullIndex);
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
            return *(Type*)fixed_array_begin(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
        }

        Type& Begin()
        {
            return *(Type*)fixed_array_begin(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
        }

        const Type& Back() const
        {
            return *(Type*)fixed_array_back(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
        }

        Type& Back()
        {
            return *(Type*)fixed_array_back(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
        }

        bool Empty() const
        {
            return fixed_array_empty(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
        }

        bool Full() const
        {
            return fixed_array_full(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
        }

        std::size_t Size() const
        {
            return fixed_array_size(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer) / sizeof(Type);
        }

        void Clear() const
        {
            fixed_array_clear(&SContainer::m_pMemoryInterface->m_fixedArrayContainer, SContainer::m_pContainer);
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