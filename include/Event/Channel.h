#pragma once

#include <mutex>
#include <functional>

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Event {

    struct IChannel
    {
        struct event
        {
            RuntimeReflection::__duckvil_resource_function_t m_functionHandle;
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
            void* m_pObject;
        };

        IChannel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            m_pReflectionData(_pReflectionData)
        {
            _heap.Allocate(m_aHandlers, 1);
        }

        std::mutex m_mutex;
        std::size_t m_ullTypeID;

        Memory::Vector<event> m_aHandlers;

        RuntimeReflection::__data* m_pReflectionData;
    };

    template <typename Message>
    class Channel : public IChannel
    {
    public:
        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            IChannel(_heap, _pReflectionData)
        {
            m_ullTypeID = typeid(Message).hash_code();
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

}}