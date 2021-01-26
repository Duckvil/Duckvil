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

    struct event
    {
        RuntimeReflection::__duckvil_resource_function_t m_functionHandle;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        void* m_pObject;
    };

    struct IChannel
    {
        IChannel(RuntimeReflection::__data* _pReflectionData) :
            m_pReflectionData(_pReflectionData)
        {

        }

        RuntimeReflection::__data* m_pReflectionData;
    };

    struct event_lookup
    {
        size_t m_ullMessageTypeID;
        mode m_mode;
        IChannel* m_pChannel;
    };

    template <typename Message, mode Blocking>
    class Channel : public IChannel
    {
    public:
        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            IChannel(_pReflectionData)
        {

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

}}