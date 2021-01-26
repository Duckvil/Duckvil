#pragma once

#include "Event/Pool.h"
#include "Event/ImmediateChannel.h"

namespace Duckvil { namespace Event {

    // This pool can handle many different event messages
    template <>
    class Pool<pool_specification::pool_specification_immediate>
    {
    private:
        // Memory::Vector<IChannel*> m_aChannels; // Each one for specific message type
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

        template <typename Message>
        void Add(void* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Add(_pHandler, _typeHandle);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler, _typeHandle);

            // IChannel* _channel = nullptr;

            // for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            // {
            //     _channel = m_aChannels[i];

            //     if(_channel->m_ullTypeID == typeid(Message).hash_code())
            //     {
            //         Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

            //         _right->Add(_pHandler, _typeHandle);

            //         return;
            //     }
            // }

            // if(m_aChannels.Full())
            // {
            //     m_aChannels.Resize(m_aChannels.Size() * 2);
            // }

            // Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflectionData);

            // m_aChannels.Allocate(_newChannel);

            // _newChannel->Add(_pHandler, _typeHandle);
        }

        template <typename Message, typename Handler>
        void Add(Handler* _pHandler)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Add(_pHandler);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler);

            // IChannel* _channel = nullptr;

            // for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            // {
            //     _channel = m_aChannels[i];

            //     if(_channel->m_ullTypeID == typeid(Message).hash_code())
            //     {
            //         Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

            //         _right->Add(_pHandler);

            //         return;
            //     }
            // }

            // if(m_aChannels.Full())
            // {
            //     m_aChannels.Resize(m_aChannels.Size() * 2);
            // }

            // Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflectionData);

            // m_aChannels.Allocate(_newChannel);

            // _newChannel->Add(_pHandler);
        }

        template <typename Message, typename Handler>
        void Remove(Handler* _pHandler)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Remove(_pHandler);
                }
            }

            // IChannel* _channel = nullptr;

            // for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            // {
            //     _channel = m_aChannels[i];

            //     if(_channel->m_ullTypeID == typeid(Message).hash_code())
            //     {
            //         Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

            //         _right->Remove(_pHandler);
            //     }
            // }
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

            // IChannel* _channel = nullptr;

            // for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            // {
            //     _channel = m_aChannels[i];

            //     if(_channel->m_ullTypeID == typeid(Message).hash_code())
            //     {
            //         m_aChannels.Erase(i);
            //     }
            // }
        }

        template <typename Message>
        void Broadcast(const Message& _message)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Broadcast(_message);
                }
            }

            // IChannel* _channel = nullptr;

            // for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            // {
            //     _channel = m_aChannels[i];

            //     if(_channel->m_ullTypeID == typeid(Message).hash_code())
            //     {
            //         Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel;

            //         _right->Broadcast(_message);
            //     }
            // }
        }
    };

}}