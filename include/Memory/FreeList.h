#pragma once

#include "Memory/Memory.h"
#include "Memory/FreeListAllocator.h"
#include "Memory/Vector.h"
#include "Memory/Stack.h"
#include "Memory/Array.h"
#include "Memory/Queue.h"

#include "DependencyInjection/Injectable.h"

#include "RuntimeReflection/Markers.h"

#include "DependencyInjection/IDependencyInjector.h"

#include "Memory/FreeList.generated.h"

namespace Duckvil { namespace Memory {

// Copied FreeList will share the same memory region from which we copied it
    
    DUCKVIL_CLASS(Duckvil::DependencyInjection::Injectable(Duckvil::DependencyInjection::Scope::TRANSIENT))
    class FreeList
    {
        DUCKVIL_GENERATED_BODY
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

        template <typename Type>
        Type* Allocate(const Type& _data)
        {
            // void* _pointer = free_list_allocate<Type>(m_pMemory, m_pContainer);

            // return new(_pointer) Type(_vArgs...); // (Type*)free_list_allocate(m_pContainer, sizeof(Type), alignof(Type));

            return free_list_allocate(m_pMemory, m_pContainer, _data);
        }

        template <typename Type, typename... Args>
        Type* Allocate(Args&&... _vArgs)
        {
            // void* _pointer = free_list_allocate<Type>(m_pMemory, m_pContainer);

            // return new(_pointer) Type(_vArgs...); // (Type*)free_list_allocate(m_pContainer, sizeof(Type), alignof(Type));

            return free_list_emplace_back<Type>(m_pMemory, m_pContainer, std::forward<Args>(_vArgs)...);
        }

        template <typename Type, typename... Args>
        Type* Allocate(Args&&... _vArgs) const
        {
            // void* _pointer = free_list_allocate<Type>(m_pMemory, m_pContainer);

            // return new(_pointer) Type(_vArgs...); // (Type*)free_list_allocate(m_pContainer, sizeof(Type), alignof(Type));

            return free_list_emplace_back<Type>(m_pMemory, m_pContainer, std::forward<Args>(_vArgs)...);
        }

        template <typename ContainerType>
        void Allocate(ContainerType& _container, std::size_t _ullSize)
        {
            throw std::exception("Not implemented");
        }

        template <typename ContainerType>
        void Allocate(ContainerType& _container, std::size_t _ullSize) const
        {
            throw std::exception("Not implemented");
        }

        template <>
        void Allocate(FreeList& _container, std::size_t _ullSize)
        {
            _container = FreeList(m_pMemory, m_pContainer, _ullSize);
        }

        template <>
        void Allocate(FreeList& _container, std::size_t _ullSize) const
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
        Type* AllocateArray(std::size_t _ullCount) const
        {
            return static_cast<Type*>(free_list_allocate(m_pMemory, m_pContainer, sizeof(Type) * _ullCount, 8));
        }

        template <typename Type>
        void Free(Vector<Type>& _container)
        {
            _container.~Vector<Type>();
        }

        void Free(void* _pData)
        {
            free_list_free(m_pMemory, m_pContainer, _pData);
        }

        template <typename Type>
        void Free(Type* _pObject)
        {
            free_list_free(m_pMemory, m_pContainer, _pObject);
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

    struct free_list_context
    {
        FreeList m_heap;
        bool m_bInitialized;

        free_list_context()
        {
            m_bInitialized = false;
        }

        free_list_context(const FreeList& _heap) :
            m_heap(_heap)
        {
            m_bInitialized = false;
        }
    };

    inline free_list_context g_heapContext;

    static void heap_make_current(const free_list_context& _context)
    {
        if(g_heapContext.m_bInitialized)
        {
            return;
        }

        g_heapContext.m_heap = _context.m_heap;
        g_heapContext.m_bInitialized = true;
    }

    static const free_list_context& heap_get_current()
    {
        return g_heapContext;
    }

}}