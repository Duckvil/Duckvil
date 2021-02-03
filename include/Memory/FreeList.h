#pragma once

#include "Memory/Memory.h"
#include "Memory/FreeListAllocator.h"
#include "Memory/Vector.h"
#include "Memory/Stack.h"
#include "Memory/Array.h"

namespace Duckvil { namespace Memory {

// Copied FreeList will share the same memory region from which we copied it

    class FreeList
    {
    private:
        IMemory* m_pMemory;
        __free_list_allocator* m_pContainer;

    public:
        FreeList()
        {
            m_pContainer = nullptr;
            m_pMemory = nullptr;
        }

    // Allocate FreeList allocator uisng LinearAllocator with specified size
        FreeList(IMemory* _pMemory, __linear_allocator* _pAllocator, std::size_t _ullSize) :
            m_pMemory(_pMemory),
            m_pContainer(_pMemory->m_fnLinearAllocateFreeListAllocator(_pAllocator, _ullSize))
        {

        }

        FreeList(IMemory* _pMemory, __free_list_allocator* _pAllocator, std::size_t _ullSize) :
            m_pMemory(_pMemory),
            m_pContainer(_pMemory->m_fnFreeListAllocateFreeListAllocator(_pMemory, _pAllocator, _ullSize))
        {

        }

    // Use existing FreeList allocator
        FreeList(IMemory* _pMemory, __free_list_allocator* _pAllocator) :
            m_pMemory(_pMemory),
            m_pContainer(_pAllocator)
        {

        }

        FreeList(const FreeList& _freeList) :
            m_pMemory(_freeList.m_pMemory),
            m_pContainer(_freeList.m_pContainer)
        {

        }

        FreeList(FreeList&& _freeList) noexcept:
            m_pMemory(std::move(_freeList.m_pMemory)),
            m_pContainer(std::move(_freeList.m_pContainer))
        {
            _freeList.m_pMemory = nullptr;
            _freeList.m_pContainer = nullptr;
        }

        ~FreeList()
        {

        }

        FreeList& operator=(const FreeList& _freeList)
        {
            m_pMemory = _freeList.m_pMemory;
            m_pContainer = _freeList.m_pContainer;

            return *this;
        }

        FreeList& operator=(FreeList&& _freeList)
        {
            m_pMemory = std::move(_freeList.m_pMemory);
            m_pContainer = std::move(_freeList.m_pContainer);

            _freeList.m_pMemory = nullptr;
            _freeList.m_pContainer = nullptr;

            return *this;
        }

        template <typename Type, typename... Args>
        Type* Allocate(const Args&... _vArgs)
        {
            void* _pointer = m_pMemory->m_fnFreeListAllocate_(m_pContainer, sizeof(Type), alignof(Type));

            return new(_pointer) Type(_vArgs...); // (Type*)free_list_allocate(m_pContainer, sizeof(Type), alignof(Type));
        }

        template <typename Type, typename... Args>
        Type* Allocate(const Args&... _vArgs) const
        {
            void* _pointer = m_pMemory->m_fnFreeListAllocate_(m_pContainer, sizeof(Type), alignof(Type));

            return new(_pointer) Type(_vArgs...); // (Type*)free_list_allocate(m_pContainer, sizeof(Type), alignof(Type));
        }

        template <typename Type>
        void Allocate(Vector<Type>& _container, std::size_t _ullCount)
        {
            _container = Vector<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        void Allocate(FreeList& _container, std::size_t _ullSize)
        {
            _container = FreeList(m_pMemory, m_pContainer, _ullSize);
        }

        template <typename Type>
        void Allocate(Vector<Type>& _container, std::size_t _ullCount) const
        {
            _container = Vector<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Free(Vector<Type>& _container)
        {
            _container.~Vector<Type>();
        }

        void Free(void* _pData)
        {
            m_pMemory->m_fnFreeListFree_(m_pContainer, _pData);
        }

        template <typename Type>
        void Allocate(Stack<Type>& _container, std::size_t _ullCount)
        {
            // _container = Vector<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Allocate(Array<Type>& _container, std::size_t _ullCount)
        {
            _container = Array<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        IMemory* GetMemoryInterface() const
        {
            return m_pMemory;
        }

        __free_list_allocator* GetAllocator() const
        {
            return m_pContainer;
        }

    };

}}