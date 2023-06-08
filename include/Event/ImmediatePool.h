#pragma once

#include "Event/Pool.h"
#include "Event/ImmediateChannel.h"

namespace Duckvil { namespace Event {

    class EventEventNotFound: public std::exception
    {
    public:
        const char* what() const throw() override
        {
            return "Could not find specified event";
        }
    };

    // TODO: Check support for template specialized class reflection

    // This pool can handle many different event messages
    template <>
    class Pool<mode::immediate>
    {
    private:
        Memory::Vector<event_lookup> m_aChannels; // Each one for specific message type

        Memory::FreeList m_heap;
        RuntimeReflection::__data* m_pReflectionData;
        const RuntimeReflection::__ftable* m_pReflection;

    public:
        Pool()
        {
            m_pReflectionData = nullptr;
        }

        Pool(const Memory::FreeList& _heap, const RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pReflectionData) :
            m_heap(_heap),
            m_pReflectionData(_pReflectionData),
            m_pReflection(_pReflection)
        {
            _heap.Allocate(m_aChannels, 10);
        }

        Pool(const Memory::FreeList& _heap) :
            m_heap(_heap)
        {
            m_pReflection = RuntimeReflection::get_current().m_pReflection;
            m_pReflectionData = RuntimeReflection::get_current().m_pReflectionData;

            _heap.Allocate(m_aChannels, 10);
        }

        Pool(const Pool& _pool) noexcept
        {
            m_heap = _pool.m_heap;
            m_aChannels = _pool.m_aChannels;
            m_pReflectionData = _pool.m_pReflectionData;
            m_pReflection = _pool.m_pReflection;
        }

        Pool(Pool&& _pool) noexcept :
            m_aChannels(std::move(_pool.m_aChannels)),
            m_heap(std::move(_pool.m_heap)),
            m_pReflectionData(std::move(_pool.m_pReflectionData)),
            m_pReflection(std::move(_pool.m_pReflection))
        {
            _pool.m_pReflectionData = nullptr;
            _pool.m_pReflection = nullptr;
        }

        ~Pool()
        {

        }

        Pool& operator=(const Pool& _pool)
        {
            if(&_pool == this)
            {
                return *this;
            }

            this->m_aChannels = _pool.m_aChannels;
            this->m_heap = _pool.m_heap;
            this->m_pReflectionData = _pool.m_pReflectionData;
            this->m_pReflection = _pool.m_pReflection;

            return *this;
        }

        Pool& operator=(Pool&& _pool)
        {
            if(&_pool == this)
            {
                return *this;
            }

            this->m_aChannels = std::move(_pool.m_aChannels);
            this->m_heap = std::move(_pool.m_heap);
            this->m_pReflectionData = std::move(_pool.m_pReflectionData);
            this->m_pReflection = std::move(_pool.m_pReflection);

            _pool.m_pReflectionData = nullptr;
            _pool.m_pReflection = nullptr;

            return *this;
        }

        template <typename Type, typename Message>
        void Add(Type* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
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

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler, _typeHandle);
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

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler, _typeHandle);
        }

        template <typename Message>
        void Add(HotReloader::ITrackKeeper* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
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

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler, _typeHandle);
        }

        template <typename Message, typename Handler>
        void Add(Handler* _pHandler)
        {
            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type<Handler>(m_pReflection, m_pReflectionData);

            Add<Message>(_pHandler, _typeHandle);
        }

        template <typename Message>
        void Add(void (*_fn)(const Message&))
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Add(_fn);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_fn);
        }

        template <typename Message, typename Handler>
        void Add(Handler* _pHandler, void (Handler::*_fn)(const Message&))
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Add(_pHandler, _fn);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler, _fn);
        }

        template <typename Message>
        void Add(void (*_fn)(Message&))
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Add(_fn);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_fn);
        }

        template <typename Message, typename Handler>
        void Add(Handler* _pHandler, void (Handler::*_fn)(Message&))
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Add(_pHandler, _fn);

                    return;
                }
            }

            if(m_aChannels.Full())
            {
                m_aChannels.Resize(m_aChannels.Size() * 2);
            }

            Channel<Message, mode::immediate>* _newChannel = m_heap.Allocate<Channel<Message, mode::immediate>>(m_heap, m_pReflection, m_pReflectionData);

            event_lookup _event = {};

            _event.m_mode = mode::immediate;
            _event.m_ullMessageTypeID = typeid(Message).hash_code();
            _event.m_pChannel = _newChannel;

            m_aChannels.Allocate(_event);

            _newChannel->Add(_pHandler, _fn);
        }

        template <typename Message, class F, std::enable_if_t<is_stateless<F>::value, bool> = true>
        void AddA(const F& _fn)
        {
            Add<Message>(_fn);
        }

        template <typename Message, class F, std::enable_if_t<!is_stateless<F>::value, bool> = true>
        void AddA(F&& _fn)
        {
            typename Utils::lambda_traits<F>::pointer _ptr = Utils::lambda(std::forward<F>(_fn));
            Add<Message>(_ptr);
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
        }

        template <typename Message>
        void Remove()
        {
            for(uint32_t i = 0; i < m_aChannels.Size(); ++i)
            {
                const auto& _channel = m_aChannels[i];

                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    m_heap.Free(_channel.m_pChannel);

                    m_aChannels.Erase(i);
                }
            }
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
        }

        template <typename Message>
        void Broadcast(const Message& _message) const
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Broadcast(_message);
                }
            }
        }

        template <typename Message>
        void Broadcast(Message& _message)
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Broadcast(_message);
                }
            }
        }

        template <typename Message>
        void Broadcast(Message& _message) const
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    Channel<Message, mode::immediate>* _right = (Channel<Message, mode::immediate>*)_channel.m_pChannel;

                    _right->Broadcast(_message);
                }
            }
        }

        template <typename Message>
        Channel<Message, mode::immediate>& GetChannel() const
        {
            for(const auto& _channel : m_aChannels)
            {
                if(_channel.m_ullMessageTypeID == typeid(Message).hash_code())
                {
                    return *(Channel<Message, mode::immediate>*)_channel.m_pChannel;
                }
            }

            EventEventNotFound _exception;

            throw _exception;
        }
    };

}}