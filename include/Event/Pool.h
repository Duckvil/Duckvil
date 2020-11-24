#pragma once

#include "Event/Channel.h"

namespace Duckvil { namespace Event {

    // TODO: Maybe specific channel type pool? Then it could be used to grouping event channels

    // Mixed container for any channel type(immediate, buffered, buffer_invoked) and any event
    class Pool
    {
    private:
        Memory::Vector<IChannel*> m_aChannels;

        Memory::FreeList m_heap;
        RuntimeReflection::__data* m_pReflectionData;

        uint32_t m_uiIndex;

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

            m_uiIndex = 0;
        }

        ~Pool()
        {

        }

        template <typename Message, typename Handler>
        void Add(Handler* _pHandler, mode _bBlocking = mode::immediate)
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    if(_channel->m_bBlocking == mode::immediate)
                    {
                        Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

                        _right->Add(_pHandler);
                    }
                    else if(_channel->m_bBlocking == mode::buffered)
                    {
                        Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel;

                        _right->Add(_pHandler);
                    }

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            if(_bBlocking == mode::immediate)
            {
                Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflectionData);

                m_aChannels.Allocate(_newChannel);

                _newChannel->Add(_pHandler);
            }
            else if(_bBlocking == mode::buffered)
            {
                Channel<Message, mode::buffered>* _newChannel = m_heap.Allocate<Channel<Message, mode::buffered>>(m_heap, m_pReflectionData);

                m_aChannels.Allocate(_newChannel);

                _newChannel->Add(_pHandler);
            }
        }

        template <typename Message>
        void Add()
        {
            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::buffered>* _newChannel = m_heap.Allocate<Channel<Message, mode::buffered>>(m_heap, m_pReflectionData);

            m_aChannels.Allocate(_newChannel);
        }

        template <typename Message, typename Handler>
        void Remove(Handler* _pHandler)
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    if(_channel->m_bBlocking == mode::immediate)
                    {
                        Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

                        _right->Remove(_pHandler);
                    }
                    else if(_channel->m_bBlocking == mode::buffered)
                    {
                        Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel;

                        _right->Remove(_pHandler);
                    }
                }
            }
        }

        template <typename Message>
        void Remove()
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    m_aChannels.Erase(i);
                }
            }
        }

        template <typename Message>
        void Broadcast(const Message& _message)
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    if(_channel->m_bBlocking == mode::immediate)
                    {
                        Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

                        _right->Broadcast(_message);
                    }
                    else if(_channel->m_bBlocking == mode::buffered)
                    {
                        Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel;

                        _right->Broadcast(_message);
                    }
                }
            }
        }

        // Check if there are any pending events
        bool AnyEvents() const
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_bBlocking != mode::immediate)
                {
                    BufferedChannel* _right = (BufferedChannel*)_channel;

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
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code() && _channel->m_bBlocking == mode::buffered)
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel;

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
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel;

                    _right->EventHandled();
                }
            }
        }

        // Call it if the catched event can not be handled.
        // The message will be pushed back to queue after 'Reset' function call
        template <typename Message>
        void EventHandled(const Message& _message)
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::buffered>* _right = (Channel<Message, mode::buffered>*)_channel;

                    _right->EventHandled(_message);
                }
            }
        }

        // After each event loop it have to be called
        void Reset()
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_bBlocking == Event::mode::buffered)
                {
                    Event::BufferedChannel* _right = (Event::BufferedChannel*)_channel;

                    _right->m_uiIndex = 0;
                    _right->m_fnReset(_right);
                }
            }
        }
    };

}}