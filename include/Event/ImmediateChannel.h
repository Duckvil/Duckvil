#pragma once

#include "Event/Channel.h"

#include <type_traits>
#include <mutex>

#include "Utils/Function.h"

#include "HotReloader/ITrackKeeper.h"

namespace Duckvil { namespace Event {

    template <typename T, typename U>
    struct helper : helper<T, decltype(&U::operator())>
    {

    };

    template <typename T, typename C, typename R, typename... A>
    struct helper<T, R(C::*)(A...) const> 
    {
        static const bool value = std::is_convertible<T, R(*)(A...)>::value;
    };

    template<typename T>
    struct is_stateless
    {
        static const bool value = helper<T,T>::value;
    };

    struct reflected_event
    {
        RuntimeReflection::__duckvil_resource_function_t m_functionHandle;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        void* m_pObject;
    };

    struct tracked_event
    {
        RuntimeReflection::__duckvil_resource_function_t m_functionHandle;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        HotReloader::ITrackKeeper* m_pObject;
    };

    // Invoked immediately when broadcasted
    template <typename Message>
    class Channel<Message, mode::immediate> : public IChannel
    {
    public:
        typedef void (*Callback)(const Message& _message);

        struct imember_event
        {
            virtual void Invoke(const Message& _message) = 0;
        };

        template <typename Type>
        struct member_event : public imember_event
        {
            typedef void (Type::*Callback)(const Message&);

            member_event(Type* _pObject, const Callback& _fn) :
                m_pObject(_pObject),
                m_fn(_fn)
            {

            }

            Type* m_pObject;
            Callback m_fn;

            void Invoke(const Message& _message) override
            {
                (m_pObject->*(m_fn))(_message);
            }
        };

    private:
        Memory::Vector<reflected_event> m_aRefelctedEvents;
        Memory::Vector<tracked_event> m_aTrackedEvents;
        Memory::Vector<Callback> m_aCallbackEvents;
        Memory::Vector<imember_event*> m_aMemberEvents;

        RuntimeReflection::__data* m_pReflectionData;
        RuntimeReflection::__ftable* m_pReflection;

    public:
        Channel()
        {

        }

        Channel(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pReflectionData) :
            m_pReflectionData(_pReflectionData),
            m_pReflection(_pReflection)
        {
            _heap.Allocate(m_aRefelctedEvents, 1);
            _heap.Allocate(m_aTrackedEvents, 1);
            _heap.Allocate(m_aCallbackEvents, 1);
            _heap.Allocate(m_aMemberEvents, 1);
            DUCKVIL_DEBUG_MEMORY(m_aRefelctedEvents.GetAllocator(), "m_aRefelctedEvents");
        }

        ~Channel()
        {

        }

        void Add(void* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
        {
            reflected_event _event = {};

            _event.m_functionHandle = RuntimeReflection::get_function_handle<const Message&>(m_pReflection, m_pReflectionData, _typeHandle, "OnEvent");
            _event.m_typeHandle = _typeHandle;
            _event.m_pObject = _pHandler;

            if(m_aRefelctedEvents.Full())
            {
                m_aRefelctedEvents.Resize(m_aRefelctedEvents.Size() * 2);
            }

            m_aRefelctedEvents.Allocate(_event);
        }

        void Add(HotReloader::ITrackKeeper* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
        {
            tracked_event _event = {};

            _event.m_functionHandle = RuntimeReflection::get_function_handle<const Message&>(m_pReflection, m_pReflectionData, _typeHandle, "OnEvent");
            _event.m_typeHandle = _typeHandle;
            _event.m_pObject = _pHandler;

            if(m_aTrackedEvents.Full())
            {
                m_aTrackedEvents.Resize(m_aTrackedEvents.Size() * 2);
            }

            m_aTrackedEvents.Allocate(_event);
        }

        template <typename Handler>
        void Add(Handler* _pHandler)
        {
            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type<Handler>(m_pReflection, m_pReflectionData);

            Add(_pHandler, _typeHandle);
        }

        void Add(const Callback& _fn)
        {
            if(m_aCallbackEvents.Full())
            {
                m_aCallbackEvents.Resize(m_aCallbackEvents.Size() * 2);
            }

            m_aCallbackEvents.Allocate(_fn);
        }

        template <typename Handler>
        void Add(Handler* _pHandler, void (Handler::*_fn)(const Message&))
        {
            if(m_aMemberEvents.Full())
            {
                m_aMemberEvents.Resize(m_aMemberEvents.Size() * 2);
            }

            m_aMemberEvents.Allocate(new member_event<Handler>(_pHandler, _fn));
        }

        template <typename Handler>
        void Remove(Handler* _pHandler)
        {
            for(uint32_t i = 0; i < m_aRefelctedEvents.Size(); ++i)
            {
                reflected_event& _event = m_aRefelctedEvents[i];

                if(_event.m_pObject == _pHandler)
                {
                    m_aRefelctedEvents.Erase(i);
                }
            }
        }

        void Remove(const Callback& _fn)
        {
            for(uint32_t i = 0; i < m_aCallbackEvents.Size(); ++i)
            {
                void (*_fnI)(const Message&) = m_aCallbackEvents[i];

                if(_fnI == _fn)
                {
                    m_aCallbackEvents.Erase(i);
                }
            }
        }

        void Broadcast(const Message& _message)
        {
            for(reflected_event& _event : m_aRefelctedEvents)
            {
                RuntimeReflection::invoke_member<const Message&>(m_pReflection, m_pReflectionData, _event.m_typeHandle, _event.m_functionHandle, _event.m_pObject, _message);
            }

            for(tracked_event& _event : m_aTrackedEvents)
            {
                RuntimeReflection::invoke_member<const Message&>(m_pReflection, m_pReflectionData, _event.m_typeHandle, _event.m_functionHandle, _event.m_pObject->GetObject(), _message);
            }

            for(Callback _fn : m_aCallbackEvents)
            {
                _fn(_message);
            }

            for(imember_event* _event : m_aMemberEvents)
            {
                _event->Invoke(_message);
            }
        }

        Channel& operator+=(const Callback& _fn)
        {
            Add(_fn);

            return *this;
        }

        template <typename Handler>
        Channel& operator+=(Handler* _pHandler)
        {
            Add(_pHandler);

            return *this;
        }

        Channel& operator-=(const Callback& _fn)
        {
            Remove(_fn);

            return *this;
        }

        template <typename Handler>
        Channel& operator-=(Handler* _pHandler)
        {
            Remove(_pHandler);

            return *this;
        }

        void operator()(const Message& _message)
        {
            Broadcast(_message);
        }
    };

}}