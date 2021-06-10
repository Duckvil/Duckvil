#pragma once

#include "Event/Pool.h"
#include "Event/BufferedChannel.h"

namespace Duckvil { namespace Event {

    template <>
    class Pool<mode::buffered>
    {
    private:
        Memory::Vector<event_lookup> m_aChannels; // Each one for specific message type

        Memory::FreeList m_heap;
        RuntimeReflection::__data* m_pReflectionData;

    public:
        Pool()
        {
            m_pReflectionData = nullptr;
        }

        Pool(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            m_heap(_heap),
            m_pReflectionData(_pReflectionData)
        {
            _heap.Allocate(m_aChannels, 1);
        }

        Pool(Pool&& _pool) noexcept :
            m_aChannels(std::move(_pool.m_aChannels)),
            m_heap(std::move(_pool.m_heap)),
            m_pReflectionData(std::move(_pool.m_pReflectionData))
        {
            _pool.m_pReflectionData = nullptr;
        }

        ~Pool()
        {

        }

        Pool& operator=(Pool&& _pool) noexcept
        {
            if(&_pool == this)
            {
                return *this;
            }

            this->m_aChannels = std::move(_pool.m_aChannels);
            this->m_heap = std::move(_pool.m_heap);
            this->m_pReflectionData = std::move(_pool.m_pReflectionData);

            _pool.m_pReflectionData = nullptr;

            return *this;
        }

        template <typename Message>
        void Add()
        {
            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            event_lookup _event = {};

            _event.m_mode = mode::buffered;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();

            Channel<Message, mode::buffered>* _newChannel = m_heap.Allocate<Channel<Message, mode::buffered>>(m_heap, m_pReflectionData);

            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);
        }

        template <typename Message>
        void Broadcast(const Message& _message)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                    _right->Broadcast(_message);
                }
            }
        }

        // Check if there are any pending events
        bool AnyEvents() const
        {
            for(const auto& _channel : m_aChannels)
            {
                BufferedChannel* _right = (BufferedChannel*)_channel.m_pChannel;

                if(_right->m_fnAnyEvents(_right))
                {
                    return true;
                }
            }

            return false;
        }

        // Catch the message
        template <typename Message>
        bool GetMessage(Message* _pMessage)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                    return _right->GetMessage(_pMessage);
                }
            }

            return false;
        }

        // Call it if the message is handled.
        // The message will be removed from queue
        template <typename Message>
        void EventHandled()
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                    _right->EventHandled();
                }
            }
        }

        // Call it if the catched event can not be handled.
        // The message will be pushed back to queue after 'Reset' function call
        template <typename Message>
        void EventHandled(const Message& _message)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                    _right->EventHandled(_message);
                }
            }
        }

        // After each event loop it have to be called
        void Reset()
        {
            for(const auto& _channel : m_aChannels)
            {
                Event::BufferedChannel* _right = (Event::BufferedChannel*)_channel.m_pChannel;

                _right->m_uiIndex = 0;
            }
        }
    };

}}