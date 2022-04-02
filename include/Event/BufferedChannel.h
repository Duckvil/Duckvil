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
        Memory::Queue<Message> m_aMessages;

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

            _heap.Allocate(m_aMessages, 20);
        }

        ~Channel()
        {

        }

        template <typename Handler>
        void Remove(Handler* _pHandler)
        {

        }

        void Broadcast(const Message& _message)
        {
            m_aMessages.Allocate(_message);

            m_uiEventsCount++;
        }

        bool GetMessage(Message* _pMessage)
        {
            if(m_aMessages.Empty())
            {
                return false;
            }

            const Message& _message = m_aMessages.Begin();

            *_pMessage = _message;

            return true;
        }

        void EventHandled()
        {
            m_aMessages.Pop();

            m_uiEventsCount--;
            m_uiIndex--;
        }

        void EventHandled(const Message& _message)
        {
        // Here we are moving not handled event to the end of queue, to give other events chance to be catched
            m_aMessages.Pop();
            m_aMessages.Allocate(_message);
        }
    };

}}