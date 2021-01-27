#pragma once

#include "Event/Pool.h"
#include "Event/BufferedChannel.h"
#include "Event/ImmediateChannel.h"

namespace Duckvil { namespace Event {

    // Mixed container for any channel type(immediate, buffered, buffer_invoked) and any event
    template <>
    class Pool<mode::mixed>
    {
    private:
        Memory::Vector<event_lookup> m_aChannels;

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
            _heap.Allocate(m_aChannels, 2);
        }

        ~Pool()
        {

        }

        template <typename Message, typename Handler>
        void Add(Handler* _pHandler, mode _bBlocking = mode::immediate)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    if(_channel.m_mode == mode::immediate)
                    {
                        Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                        _right->Add(_pHandler);
                    }
                    else if(_channel.m_mode == mode::buffered)
                    {
                        Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                        _right->Add(_pHandler);
                    }

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            event_lookup _event = {};

            _event.m_mode = _bBlocking;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();

            if(_bBlocking == mode::immediate)
            {
                Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflectionData);

                _event.m_pChannel = _newChannel;

                _newChannel->Add(_pHandler);
            }
            else if(_bBlocking == mode::buffered)
            {
                Channel<Message, mode::buffered>* _newChannel = m_heap.Allocate<Channel<Message, mode::buffered>>(m_heap, m_pReflectionData);

                _event.m_pChannel = _newChannel;

                _newChannel->Add(_pHandler);
            }

            m_aChannels.Allocate(_event);
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

        template <typename Message, typename Handler>
        void Remove(Handler* _pHandler)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    if(_channel.m_mode == mode::immediate)
                    {
                        Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                        _right->Remove(_pHandler);
                    }
                    else if(_channel.m_mode == mode::buffered)
                    {
                        Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                        _right->Remove(_pHandler);
                    }
                }
            }
        }

        template <typename Message>
        void Remove()
        {
            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                const auto& _channel = m_aChannels[i];

                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    m_aChannels.Erase(i);
                }
            }
        }

        template <typename Message>
        void Broadcast(const Message& _message)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    if(_channel.m_mode == mode::immediate)
                    {
                        Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                        _right->Broadcast(_message);
                    }
                    else if(_channel.m_mode == mode::buffered)
                    {
                        Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel.m_pChannel;

                        _right->Broadcast(_message);
                    }
                }
            }
        }

        // Check if there are any pending events
        bool AnyEvents() const
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_mode != mode::immediate)
                {
                    BufferedChannel* _right = (BufferedChannel*)_channel.m_pChannel;

                    if(_right->m_fnAnyEvents(_right))
                    {
                        return true;
                    }
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
                if(_channel.m_mode == mode::buffered && _channel.m_ullMessageTypeID == typeid(Message).hash_code())
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
                if(_channel.m_mode == mode::buffered && _channel.m_ullMessageTypeID == typeid(Message).hash_code())
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
                if(_channel.m_mode == mode::buffered)
                {
                    Event::BufferedChannel* _right = (Event::BufferedChannel*)_channel.m_pChannel;

                    _right->m_uiIndex = 0;
                }
            }
        }
    };

}}