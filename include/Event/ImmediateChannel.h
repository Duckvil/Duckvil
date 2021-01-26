#pragma once

#include "Event/Channel.h"

namespace Duckvil { namespace Event {

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
            _heap.Allocate(m_aHandlers, 1);
        }

        ~Channel()
        {

        }

        void Add(void* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
        {
            event _event = {};

            _event.m_functionHandle = RuntimeReflection::get_function_handle<const Message&>(m_pReflectionData, _typeHandle, "OnEvent");
            _event.m_typeHandle = _typeHandle;
            _event.m_pObject = _pHandler;

            if(m_aHandlers.Full())
            {
                m_aHandlers.Resize(m_aHandlers.Size() * 2);
            }

            m_aHandlers.Allocate(_event);
        }

        template <typename Handler>
        void Add(Handler* _pHandler)
        {
            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type<Handler>(m_pReflectionData);

            Add(_pHandler, _typeHandle);
        }

        template <typename Handler>
        void Remove(Handler* _pHandler)
        {
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
            for(event& _event : m_aHandlers)
            {
                RuntimeReflection::invoke_member<const Message&>(m_pReflectionData, _event.m_typeHandle, _event.m_functionHandle, _event.m_pObject, _message);
            }
        }
    };

}}