#pragma once

#include "Memory/FixedVectorAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/FreeListAllocator.h"
#include "Memory/SpecifiedResizableContainer.h"

#include <type_traits>
#include <utility>
#include <cstring>

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace Memory {

// Copied Vector will allocate new memory and copy data from which we copied it

    template <typename Type>
    class Vector : public SpecifiedResizableContainer<Type, fixed_vector_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

        using SContainer = SpecifiedResizableContainer<Type, fixed_vector_allocator>;

        typedef void (*erase_callback)(const ftable* _pMemoryInterface, SContainer* _pThis, uint32_t _uiIndex);

    private:
        erase_callback m_fnErase;

        static void free_list_copy(const SpecifiedContainer<Type, fixed_vector_allocator>& _specifiedContainer, SpecifiedContainer<Type, fixed_vector_allocator>* _pThis)
        {
            const Vector& _vec = static_cast<const Vector&>(_specifiedContainer);

            _pThis->m_pContainer = static_cast<fixed_vector_allocator*>(free_list_allocate(_pThis->m_pMemoryInterface, static_cast<free_list_allocator*>(_vec.m_pAllocator), sizeof(fixed_vector_allocator) + (sizeof(Type) * _vec.Size()), alignof(fixed_vector_allocator)));

            Memory::fixed_vector_copy<Type>(_pThis->m_pMemoryInterface, _vec.m_pContainer, _pThis->m_pContainer);

            // DUCKVIL_DEBUG_MEMORY(_pThis->m_pContainer, "Copied Vector");

            _pThis->m_fnCopy = _vec.m_fnCopy;
            _pThis->m_fnDestruct = _vec.m_fnDestruct;
            _pThis->m_pAllocator = _vec.m_pAllocator;
            static_cast<Vector*>(_pThis)->m_fnErase = _vec.m_fnErase;
            static_cast<Vector*>(_pThis)->m_fnResize = _vec.m_fnResize;
        }

        static void free_list_destruct(allocator* _pAllocator, SpecifiedContainer<Type, fixed_vector_allocator>* _pThis)
        {
            while(!fixed_vector_empty(_pThis->m_pMemoryInterface, _pThis->m_pContainer))
            {
                fixed_vector_pop_back<Type>(_pThis->m_pMemoryInterface, _pThis->m_pContainer);
            }

            free_list_free(_pThis->m_pMemoryInterface, static_cast<free_list_allocator*>(_pAllocator), _pThis->m_pContainer);
        }

        static void free_list_resize(const ftable* _pMemoryInterface, SContainer* _pThis, std::size_t _ullNewSize)
        {
            fixed_vector_resize(_pMemoryInterface, static_cast<free_list_allocator*>(_pThis->m_pAllocator), &_pThis->m_pContainer, _ullNewSize);
        }

        static void free_list_erase(const ftable* _pMemoryInterface, SContainer* _pThis, uint32_t _uiIndex)
        {
            fixed_vector_erase<Type>(_pMemoryInterface, _pThis->m_pContainer, _uiIndex);
        }

    public:
        Vector() :
            SContainer(
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr
            ),
            m_fnErase(nullptr)
        {

        }

        Vector(const ftable* _pMemoryInterface, free_list_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            SContainer::m_fnCopy = &free_list_copy;
            SContainer::m_fnDestruct = &free_list_destruct;
            SContainer::m_fnResize = &free_list_resize;
            SContainer::m_pContainer = _pMemoryInterface->m_fnFreeListAllocateFixedVectorAllocator(_pMemoryInterface, _pAllocator, sizeof(Type) * _ullCount, sizeof(Type));

            m_fnErase = &free_list_erase;
        }

        Vector(const Vector& _vector) :
            SContainer(_vector.m_pMemoryInterface, _vector.m_pAllocator)
        {
            _vector.m_fnCopy(_vector, this);
        }

        Vector(Vector&& _vector) noexcept :
            SContainer(
                std::move(_vector.m_pMemoryInterface),
                std::move(_vector.m_pAllocator),
                std::move(_vector.m_pContainer),
                std::move(_vector.m_fnCopy),
                std::move(_vector.m_fnDestruct),
                std::move(_vector.m_fnResize)
            ),
            m_fnErase(std::move(_vector.m_fnErase))
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

            SContainer::m_fnDestruct(SContainer::m_pAllocator, this);

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

            m_fnErase = std::move(_vector.m_fnErase);

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

            _vector.m_fnCopy(_vector, this);

            return *this;
        }

        Type* Allocate(const Type& _value)
        {
            return fixed_vector_allocate<Type>(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _value);
        }

        Type* Allocate(Type&& _value)
        {
            return fixed_vector_allocate<Type>(SContainer::m_pMemoryInterface, SContainer::m_pContainer, std::move(_value));
        }

        const Type& At(std::size_t _ullIndex) const
        {
            return *static_cast<Type*>(fixed_vector_at(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _ullIndex));
        }

        Type& At(std::size_t _ullIndex)
        {
            return *static_cast<Type*>(fixed_vector_at(SContainer::m_pMemoryInterface, SContainer::m_pContainer, _ullIndex));
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
            m_fnErase(SContainer::m_pMemoryInterface, this, _uiIndex);
        }

        Iterator begin() const
        {
            // return Iterator((Type*)SContainer::m_pContainer->m_pMemory);
            return Iterator(static_cast<Type*>(fixed_vector_begin(SContainer::m_pMemoryInterface, SContainer::m_pContainer)));
        }

        Iterator end() const
        {
            // return Iterator((Type*)(SContainer::m_pContainer->m_pMemory + SContainer::m_pContainer->m_ullUsed));
            return Iterator(static_cast<Type*>(fixed_vector_end(SContainer::m_pMemoryInterface, SContainer::m_pContainer)));
        }

        ConstIterator cbegin() const
        {
            return ConstIterator(static_cast<Type*>(SContainer::GetWorkingMemory()));
        }

        ConstIterator cend() const
        {
            return ConstIterator(static_cast<Type*>((static_cast<uint8_t*>(SContainer::GetWorkingMemory()) + SContainer::m_pContainer->m_ullUsed)));
        }
    };

}}