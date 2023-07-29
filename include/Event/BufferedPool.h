#pragma once

#include "Event/Pool.h"

#include "Memory/QueueAllocator.h"
#include "Memory/StackAllocator.h"

#undef GetMessage

namespace Duckvil { namespace Event {

    template <>
    class Pool<mode::buffered>
    {
    public:
        struct EventType
        {
            std::size_t m_ullMessageTypeID;
            std::size_t m_ullMessageTypeSize;
            uint8_t m_ucAlignment;
        };

        // template <typename Message>
        // struct EventMessage : public EventType
        // {
        //     Message m_message;
        // };

        struct EventMessage : public EventType
        {
            void* m_pMessage;
        };

    private:
        Memory::FreeList m_heap;
        RuntimeReflection::__data* m_pReflectionData;

        Memory::fixed_queue_allocator* m_pMessages;
        Memory::fixed_stack_allocator* m_pRemaining;

    public:
        Pool()
        {
            m_pReflectionData = nullptr;
        }

        Pool(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            m_heap(_heap),
            m_pReflectionData(_pReflectionData)
        {
            m_pMessages = static_cast<Memory::fixed_queue_allocator*>(_heap.GetMemoryInterface()->m_fnFreeListAllocate_(_heap.GetAllocator(), sizeof(Memory::fixed_queue_allocator) + 1024, alignof(Memory::fixed_queue_allocator)));

            m_pMessages->m_ullCapacity = 1024;
            m_pMessages->m_ullUsed = 0;
            m_pMessages->m_ullTail = 0;
            m_pMessages->m_ullHead = 0;
            m_pMessages->m_ullBlockSize = sizeof(EventMessage);

            m_pRemaining = static_cast<Memory::fixed_stack_allocator*>(_heap.GetMemoryInterface()->m_fnFreeListAllocate_(_heap.GetAllocator(), sizeof(Memory::fixed_stack_allocator) + 1024, alignof(Memory::fixed_stack_allocator)));

            m_pRemaining->m_ullCapacity = 1024;
            m_pRemaining->m_ullUsed = 0;
            m_pRemaining->m_ullBlockSize = sizeof(EventMessage);
        }

        Pool(const Pool& _pool) noexcept
        {
            m_heap = _pool.m_heap;
            m_pReflectionData = _pool.m_pReflectionData;
            m_pMessages = _pool.m_pMessages;
            m_pRemaining = _pool.m_pRemaining;
        }

        Pool(Pool&& _pool) noexcept :
            m_heap(std::move(_pool.m_heap)),
            m_pReflectionData(std::move(_pool.m_pReflectionData)),
            m_pMessages(std::move(_pool.m_pMessages)),
            m_pRemaining(std::move(_pool.m_pRemaining))
        {
            _pool.m_pReflectionData = nullptr;
            _pool.m_pMessages = nullptr;
            _pool.m_pRemaining = nullptr;
        }

        ~Pool()
        {

        }

        Pool& operator=(const Pool& _pool) noexcept
        {
            if(&_pool == this)
            {
                return *this;
            }

            m_heap = _pool.m_heap;
            m_pReflectionData = _pool.m_pReflectionData;
            m_pMessages = _pool.m_pMessages;
            m_pRemaining = _pool.m_pRemaining;

            return *this;
        }

        Pool& operator=(Pool&& _pool) noexcept
        {
            if(&_pool == this)
            {
                return *this;
            }

            this->m_heap = std::move(_pool.m_heap);
            this->m_pReflectionData = std::move(_pool.m_pReflectionData);
            this->m_pMessages = std::move(_pool.m_pMessages);
            this->m_pRemaining = std::move(_pool.m_pRemaining);

            _pool.m_pReflectionData = nullptr;
            _pool.m_pMessages = nullptr;
            _pool.m_pRemaining = nullptr;

            return *this;
        }

        template <typename Message>
        void Broadcast(const Message& _message)
        {
            EventMessage _event;

            // _event.m_message = _message;
            _event.m_pMessage = m_heap.Allocate(_message);
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_ullMessageTypeSize = sizeof(EventMessage);
            _event.m_ucAlignment = alignof(EventMessage);

            Memory::fixed_queue_allocate(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages, _event);
        }

        // Check if there are any pending events
        bool AnyEvents() const
        {
            return !Memory::fixed_queue_empty(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages);
        }

        // Catch the message
        template <typename Message>
        bool GetMessage(Message* _pMessage)
        {
            EventType* _event = static_cast<EventType*>(Memory::fixed_queue_begin(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages));

            if(_event->m_ullMessageTypeID == typeid(Message).hash_code())
            {
                EventMessage* _right = static_cast<EventMessage*>(_event);

                // *_pMessage = _right->m_message;
                memcpy(_pMessage, _right->m_pMessage, sizeof(Message));

                return true;
            }

            return false;
        }

        // Call it if the message is handled.
        // The message will be removed from queue
        template <typename Message>
        void EventHandled()
        {
            EventType* _event = static_cast<EventType*>(Memory::fixed_queue_begin(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages));

            if(_event->m_ullMessageTypeID == typeid(Message).hash_code())
            {
                EventMessage* _right = static_cast<EventMessage*>(_event);

                m_heap.Free(_right->m_pMessage);

                Memory::fixed_queue_pop(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages);
            }
        }

        // Push event back to queue
        template <typename Message>
        void EventHandled(const Message& _message)
        {
            EventType* _event = static_cast<EventType*>(Memory::fixed_queue_begin(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages));

            if(_event->m_ullMessageTypeID == typeid(Message).hash_code())
            {
                EventMessage* _right = static_cast<EventMessage*>(_event);
                // Message _message = _right->m_message;

                EventMessage _newEvent;

                // _newEvent.m_message = _message;
                _newEvent.m_pMessage = m_heap.Allocate(*static_cast<Message*>(_right->m_pMessage));
                _newEvent.m_ullMessageTypeID = typeid(Message).hash_code();
                _newEvent.m_ullMessageTypeSize = sizeof(EventMessage);
                _newEvent.m_ucAlignment = alignof(EventMessage);

                m_heap.Free(_right->m_pMessage);

                // Memory::queue_allocate(m_heap.GetMemoryInterface(), m_pMessages2, _newEvent);
                Memory::fixed_stack_allocate(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining, _newEvent);
                Memory::fixed_queue_pop(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages);
            }
        }

        // Before iterating over events, it should be called in order to get all events which wasn't handled
        void Reset()
        {
            while(!Memory::fixed_stack_empty(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining))
            {
                EventType* _event = static_cast<EventType*>(Memory::fixed_stack_top(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining));

                m_heap.GetMemoryInterface()->m_fixedQueueContainer.m_fnAllocate(m_pMessages, _event, _event->m_ullMessageTypeSize, _event->m_ucAlignment);

                Memory::fixed_stack_pop(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining);
            }
        }

        // Clear all remaining events
        void Clear()
        {
            while(!Memory::fixed_queue_empty(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages))
            {
                EventMessage* _event = static_cast<EventMessage*>(Memory::fixed_queue_begin(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages));

                m_heap.Free(_event->m_pMessage);

                Memory::fixed_queue_pop(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages);
            }

            while(!Memory::fixed_stack_empty(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining))
            {
                EventMessage* _event = static_cast<EventMessage*>(Memory::fixed_stack_top(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining));

                m_heap.Free(_event->m_pMessage);

                Memory::fixed_stack_pop(&m_heap.GetMemoryInterface()->m_fixedStackContainer, m_pRemaining);
            }

            // Memory::queue_clear(m_heap.GetMemoryInterface(), m_pMessages);
            // Memory::stack_clear(m_heap.GetMemoryInterface(), m_pRemaining);
        }

        void Skip()
        {
            EventType* _event = static_cast<EventType*>(Memory::fixed_queue_begin(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages));

            // m_heap.GetMemoryInterface()->m_fnQueueAllocate_(m_pMessages2, _event, _event->m_ullMessageTypeSize, _event->m_ucAlignment);
            m_heap.GetMemoryInterface()->m_fixedStackContainer.m_fnAllocate(m_pRemaining, _event, _event->m_ullMessageTypeSize, _event->m_ucAlignment);

            Memory::fixed_queue_pop(&m_heap.GetMemoryInterface()->m_fixedQueueContainer, m_pMessages);
        }
    };

}}