#pragma once

#include "Event/Channel.h"

namespace Duckvil { namespace Event {

    class Pool
    {
    private:
        Memory::Vector<IChannel*> m_aChannels;

        Memory::FreeList m_heap;
        RuntimeReflection::__data* m_pReflectionData;

    public:
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
        void Add(Handler* _pHandler)
        {
            IChannel* _channel = nullptr;

            for(uint32_t i = 0; i < m_aChannels.Size(); i++)
            {
                _channel = m_aChannels[i];

                if(_channel->m_ullTypeID == typeid(Message).hash_code())
                {
                    Channel<Message>* _right = (Channel<Message>*)_channel;

                    _right->Add(_pHandler);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message>* _newChannel = m_heap.Allocate<Channel<Message>>(m_heap, m_pReflectionData);

            m_aChannels.Allocate(_newChannel);

            _newChannel->Add(_pHandler);
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
                    Channel<Message>* _right = (Channel<Message>*)_channel;

                    _right->Remove(_pHandler);
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
                    Channel<Message>* _right = (Channel<Message>*)_channel;

                    _right->Broadcast(_message);
                }
            }
        }
    };

}}