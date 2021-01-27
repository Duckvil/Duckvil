#pragma once

#include "Event/Channel.h"

namespace Duckvil { namespace Event {

    struct BufferedChannel : public IChannel
    {
        bool (*m_fnAnyEvents)(BufferedChannel* _pChannel);

        uint32_t m_uiIndex;
        uint32_t m_uiEventsCount;
    };

    // When broadcasted the message is pushed to messages queue. It can be catched by GetMessage function
    template <typename Message>
    class Channel<Message, mode::buffered> : public BufferedChannel
    {
    private:
        std::queue<Message> m_aMessages;

        RuntimeReflection::__data* m_pReflectionData;

        static bool AnyEvents(BufferedChannel* _pChannel)
        {
            if(_pChannel->m_uiEventsCount == 0)
            {
                return false;
            }

            bool _any = _pChannel->m_uiIndex < _pChannel->m_uiEventsCount;

            if(_any)
            {
                _pChannel->m_uiIndex++;
            }

            return _any;
        }

    public:
        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            m_pReflectionData(_pReflectionData)
        {
            m_fnAnyEvents = &AnyEvents;
            m_uiIndex = 0;
            m_uiEventsCount = 0;
        }

        ~Channel()
        {

        }

        template <typename Handler>
        void Add(Handler* _pHandler)
        {

        }

        template <typename Handler>
        void Remove(Handler* _pHandler)
        {

        }

        void Broadcast(const Message& _message)
        {
            m_aMessages.push(_message);

            m_uiEventsCount++;
        }

        bool GetMessage(Message* _pMessage)
        {
            if(m_aMessages.empty())
            {
                return false;
            }

            const Message& _message = m_aMessages.front();

            *_pMessage = _message;

            return true;
        }

        void EventHandled()
        {
            m_aMessages.pop();

            m_uiEventsCount--;
            m_uiIndex--;
        }

        void EventHandled(const Message& _message)
        {
            m_aMessages.pop();
            m_aMessages.push(_message);
        }
    };

}}