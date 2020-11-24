#pragma once

#include <mutex>
#include <functional>
#include <queue>
#include <stack>

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Event {

    enum class mode
    {
        immediate, // Call function right after broadcast
        buffered // Buffer the message for later use
    };

    struct IChannel
    {
        struct event
        {
            RuntimeReflection::__duckvil_resource_function_t m_functionHandle;
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
            void* m_pObject;
        };

        IChannel(RuntimeReflection::__data* _pReflectionData) :
            m_pReflectionData(_pReflectionData)
        {
            
        }

        std::mutex m_mutex;
        std::size_t m_ullTypeID;
        mode m_bBlocking;

        RuntimeReflection::__data* m_pReflectionData;
    };

    template <typename Message, mode Blocking>
    class Channel : public IChannel
    {
    public:
        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            IChannel(_pReflectionData)
        {
            m_ullTypeID = typeid(Message).hash_code();
        }

        ~Channel()
        {

        }

        template <typename Handler>
        void Add(Handler* _pHandler) { }

        template <typename Handler>
        void Remove(Handler* _pHandler) { }

        void Broadcast(const Message& _message) { }
    };

    // Invoked immediately when broadcasted
    template <typename Message>
    class Channel<Message, mode::immediate> : public IChannel
    {
    private:
        Memory::Vector<event> m_aHandlers;

    public:
        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            IChannel(_pReflectionData)
        {
            m_ullTypeID = typeid(Message).hash_code();
            m_bBlocking = mode::immediate;

            _heap.Allocate(m_aHandlers, 1);
        }

        ~Channel()
        {

        }

        template <typename Handler>
        void Add(Handler* _pHandler)
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            RuntimeReflection::__duckvil_resource_type_t _type = RuntimeReflection::get_type<Handler>(m_pReflectionData);
            event _event = {};

            _event.m_functionHandle = RuntimeReflection::get_function_handle<const Message&>(m_pReflectionData, _type, "OnEvent");
            _event.m_typeHandle = _type;
            _event.m_pObject = _pHandler;

            if(m_aHandlers.Full())
            {
                m_aHandlers.Resize(m_aHandlers.Size() * 2);
            }

            m_aHandlers.Allocate(_event);
        }

        template <typename Handler>
        void Remove(Handler* _pHandler)
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            for(uint32_t i = 0; i < m_aHandlers.Size(); i++)
            {
                event& _event = m_aHandlers[i];

                if(_event.m_pObject == _pHandler)
                {
                    m_aHandlers.Erase(i);
                }
            }
        }

        void Broadcast(const Message& _message)
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            for(event& _event : m_aHandlers)
            {
                RuntimeReflection::invoke<void, const Message&>(m_pReflectionData, _event.m_typeHandle, _event.m_functionHandle, _event.m_pObject, _message);
            }
        }
    };

    struct BufferedChannel : public IChannel
    {
        BufferedChannel(RuntimeReflection::__data* _pReflectionData) :
            IChannel(_pReflectionData)
        {

        }

        bool (*m_fnAnyEvents)(BufferedChannel* _pChannel);
        void (*m_fnReset)(BufferedChannel* _pChannel);

        uint32_t m_uiIndex;
        uint32_t m_uiEventsCount;
    };

    // When broadcasted the message is pushed to messages queue. It can be catched by GetMessage function
    template <typename Message>
    class Channel<Message, mode::buffered> : public BufferedChannel
    {
    private:
        std::queue<Message> m_aMessages;
        std::stack<Message> m_aNotHandledMessages;

        static bool AnyEvents(BufferedChannel* _pChannel)
        {
            if(_pChannel->m_uiEventsCount == 0)
            {
                return false;
            }

            bool _any = _pChannel->m_uiIndex != _pChannel->m_uiEventsCount;

            if(_any)
            {
                _pChannel->m_uiIndex++;
            }

            return _any;
        }

        static void Reset(BufferedChannel* _pChannel)
        {
            Channel<Message, mode::buffered>* _channel = (Channel<Message, mode::buffered>*)_pChannel;

            while(!_channel->m_aNotHandledMessages.empty())
            {
                _channel->m_aMessages.push(_channel->m_aNotHandledMessages.top());

                _channel->m_aNotHandledMessages.pop();
            }
        }

    public:
        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            BufferedChannel(_pReflectionData)
        {
            m_ullTypeID = typeid(Message).hash_code();
            m_bBlocking = mode::buffered;
            m_fnAnyEvents = &AnyEvents;
            m_fnReset = &Reset;
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
            std::lock_guard<std::mutex> lock(m_mutex);

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
        }

        void EventHandled(const Message& _message)
        {
            m_aNotHandledMessages.push(_message);
        }
    };

}}