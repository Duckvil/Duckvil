#pragma once

#include "Memory/FixedQueueAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/SpecifiedContainer.h"
#include "Memory/FreeListAllocator.h"

#include <type_traits>
#include <utility>
#include <cstring>

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Queue : public SpecifiedContainer<Type, __fixed_queue_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

        using SContainer = SpecifiedContainer<Type, __fixed_queue_allocator>;

    private:
        // __fixed_queue_allocator* m_pAllocator;
        // IMemory* m_pMemoryInterface;

        static void free_list_copy(IMemory* _pMemoryInterface, const SContainer& _specifiedContainer, SContainer* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_specifiedContainer.m_pAllocator;
            const Queue& _queue = (const Queue&)_specifiedContainer;

            _pThis->m_pContainer = (__fixed_queue_allocator*)free_list_allocate(_pMemoryInterface, _allocator, sizeof(__fixed_queue_allocator) + (sizeof(Type) * _queue.GetSize()), alignof(__fixed_queue_allocator));

            memcpy(_pThis->m_pContainer, _specifiedContainer.m_pContainer, sizeof(__fixed_queue_allocator) + (sizeof(Type) * _queue.GetSize()));

            _pThis->m_fnCopy = _specifiedContainer.m_fnCopy;
            _pThis->m_fnDestruct = _specifiedContainer.m_fnDestruct;
            _pThis->m_pAllocator = _specifiedContainer.m_pAllocator;
        }

        static void free_list_destruct(IMemory* _pMemoryInterface, __allocator* _pAllocator, SContainer* _pThis)
        {
            __free_list_allocator* _allocator = (__free_list_allocator*)_pAllocator;
            uint32_t _size = fixed_queue_size(_pMemoryInterface, _pThis->m_pContainer) / sizeof(Type);

            if(std::is_base_of<SContainer, Type>::value)
            {
                while(!fixed_queue_empty(_pMemoryInterface, _pThis->m_pContainer))
                {
                    SContainer* _container = (SContainer*)fixed_queue_begin(_pMemoryInterface, _pThis->m_pContainer);

                    _container->m_fnDestruct(_pMemoryInterface, _pAllocator, _container);
                }
            }
            else
            {
                while(!fixed_queue_empty(_pMemoryInterface, _pThis->m_pContainer))
                {
                    Type* _data = (Type*)fixed_queue_begin(_pMemoryInterface, _pThis->m_pContainer);

                    _data->~Type();

                    fixed_queue_pop(_pMemoryInterface, _pThis->m_pContainer);
                }
            }

            // _pMemoryInterface->m_fnFreeListFree_(_allocator, _pThis->m_pContainer);
            free_list_free(_pMemoryInterface, _allocator, _pThis->m_pContainer);
        }

    public:
        Queue()
        {

        }

        Queue(IMemory* _pMemoryInterface, __linear_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            this->m_pMemoryInterface = _pMemoryInterface;
            this->m_pAllocator = _pAllocator;
            this->m_pContainer = this->m_pMemoryInterface->m_fnLinearAllocateFixedQueueAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Queue(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            this->m_fnCopy = &free_list_copy;
            this->m_fnDestruct = &free_list_destruct;
            this->m_pContainer = _pMemoryInterface->m_fnFreeListAllocateFixedQueueAllocator(_pMemoryInterface, _pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Queue(const Queue& _queue) :
            SContainer(_queue.m_pMemoryInterface, _queue.m_pAllocator)
        {
            _queue.m_fnCopy(this->m_pMemoryInterface, _queue, this);
        }

        Queue(Queue&& _queue) noexcept :
            SContainer(
                std::move(_queue.m_pMemoryInterface),
                std::move(_queue.m_pAllocator),
                std::move(_queue.m_pContainer),
                std::move(_queue.m_fnCopy),
                std::move(_queue.m_fnDestruct)
            )
        {
            _queue.m_pMemoryInterface = nullptr;
            _queue.m_pContainer = nullptr;
            _queue.m_pAllocator = nullptr;
            _queue.m_fnCopy = nullptr;
            _queue.m_fnDestruct = nullptr;
        }

        ~Queue()
        {
            if(this->m_pAllocator == nullptr || this->m_pContainer == nullptr)
            {
                return;
            }

            this->m_fnDestruct(this->m_pMemoryInterface, this->m_pAllocator, this);

            this->m_pAllocator = nullptr;
            this->m_pContainer = nullptr;
        }

        Queue& operator=(Queue&& _queue)
        {
            if(&_queue == this)
            {
                return *this;
            }

            this->m_pContainer = std::move(_queue.m_pContainer);
            this->m_pMemoryInterface = std::move(_queue.m_pMemoryInterface);
            this->m_fnCopy = std::move(_queue.m_fnCopy);
            this->m_fnDestruct = std::move(_queue.m_fnDestruct);
            this->m_pAllocator = std::move(_queue.m_pAllocator);

            _queue.m_pContainer = nullptr;
            _queue.m_pMemoryInterface = nullptr;
            _queue.m_fnCopy = nullptr;
            _queue.m_fnDestruct = nullptr;
            _queue.m_pAllocator = nullptr;

            return *this;
        }

        Queue& operator=(const Queue& _queue)
        {
            this->m_pMemoryInterface = _queue.m_pMemoryInterface;

            _queue.m_fnCopy(this->m_pMemoryInterface, _queue, this);

            return *this;
        }

    // // TODO: Fix allocating other allocators in Allocator.cpp
    //     Queue(IMemory* _pMemoryInterface, __free_list_allocator* _pAllocator, std::size_t _ullCount) :
    //         m_pMemoryInterface(_pMemoryInterface)
    //     {
    //         m_pAllocator = m_pMemoryInterface->m_fnFreeListAllocateFixedQueueAllocator(_pMemoryInterface, _pAllocator, _ullCount * sizeof(Type), sizeof(Type));
    //     }

    //     Queue(IMemory* _pMemoryInterface, __fixed_queue_allocator* _pAllocator) :
    //         m_pMemoryInterface(_pMemoryInterface),
    //         m_pAllocator(_pAllocator)
    //     {

    //     }

    //     ~Queue()
    //     {

    //     }

        inline Type* Allocate(const Type& _data)
        {
            return fixed_queue_allocate(this->m_pMemoryInterface, this->m_pContainer, _data);
        }

        inline const Type& Begin() const
        {
            return *(Type*)fixed_queue_begin(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline bool Empty() const
        {
            return fixed_queue_empty(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline bool Full() const
        {
            return fixed_queue_full(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline void Pop()
        {
            fixed_queue_pop(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline std::size_t GetSize() const
        {
            return fixed_queue_size(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline std::size_t GetCapacity() const
        {
            return fixed_queue_capacity(this->m_pMemoryInterface, this->m_pContainer);
        }
    };

}}