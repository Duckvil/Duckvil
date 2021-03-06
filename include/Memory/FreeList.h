#pragma once

#include "Memory/Memory.h"
#include "Memory/FreeListAllocator.h"
#include "Memory/Vector.h"
#include "Memory/Stack.h"
#include "Memory/Array.h"
#include "Memory/Queue.h"

namespace Duckvil { namespace Memory {

// Copied FreeList will share the same memory region from which we copied it

    class FreeList
    {
    private:
        ftable* m_pMemory;
        free_list_allocator* m_pContainer;

    public:
        FreeList()
        {
            m_pContainer = nullptr;
            m_pMemory = nullptr;
        }

    // Allocate FreeList allocator uisng LinearAllocator with specified size
        FreeList(ftable* _pMemory, linear_allocator* _pAllocator, std::size_t _ullSize) :
            m_pMemory(_pMemory),
            m_pContainer(_pMemory->m_fnLinearAllocateFreeListAllocator(_pAllocator, _ullSize))
        {

        }

        FreeList(ftable* _pMemory, free_list_allocator* _pAllocator, std::size_t _ullSize) :
            m_pMemory(_pMemory),
            m_pContainer(_pMemory->m_fnFreeListAllocateFreeListAllocator(_pMemory, _pAllocator, _ullSize))
        {

        }

    // Use existing FreeList allocator
        FreeList(ftable* _pMemory, free_list_allocator* _pAllocator) :
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

        FreeList& operator=(FreeList&& _freeList) noexcept
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

        void Allocate(FreeList& _container, std::size_t _ullSize)
        {
            _container = FreeList(m_pMemory, m_pContainer, _ullSize);
        }

        template <typename Type>
        void Allocate(Vector<Type>& _container, std::size_t _ullCount)
        {
            _container = Vector<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Allocate(Vector<Type>& _container, std::size_t _ullCount) const
        {
            _container = Vector<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Allocate(Stack<Type>& _container, std::size_t _ullCount)
        {
            _container = Stack<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Allocate(Array<Type>& _container, std::size_t _ullCount)
        {
            _container = Array<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Allocate(Queue<Type>& _container, std::size_t _ullCount)
        {
            _container = Queue<Type>(m_pMemory, m_pContainer, _ullCount);
        }

        template <typename Type>
        void Allocate(Queue<Type>& _container, std::size_t _ullCount) const
        {
            _container = Queue<Type>(m_pMemory, m_pContainer, _ullCount);
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

        ftable* GetMemoryInterface() const
        {
            return m_pMemory;
        }

        free_list_allocator* GetAllocator() const
        {
            return m_pContainer;
        }

    };

}}