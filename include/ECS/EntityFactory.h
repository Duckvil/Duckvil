#pragma once

#include "ECS/Entity.h"

#include "Engine/Events/EntityCreatedEvent.h"
#include "Engine/Events/InjectConstructorArgumentEvent.h"

#include "RuntimeReflection/Meta.h"

#include "Event/ImmediatePool.h"

#include "Utils/ECS.h"

#include "DependencyInjection/ReflectionFlags.h"

#include "HotReloader/ReflectionFlags.h"

#include "ECS/EntityFactory.generated.h"

#define DUCKVIL_RUNTIME_REFLECTION_PAUSE
#define DUCKVIL_RUNTIME_REFLECTION_RESUME

namespace Duckvil { namespace ECS {

    DUCKVIL_CLASS(Duckvil::DependencyInjection::INJECTABLE, Duckvil::HotReloader::NOT_SERIALIZABLE)
    class EntityFactory
    {
        DUCKVIL_GENERATED_BODY
    private:
        flecs::world& m_ecs;
        const Memory::FreeList& m_heap;

        Event::Pool<Event::mode::immediate> m_eventPool;
        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;

    public:
DUCKVIL_RUNTIME_REFLECTION_PAUSE
        EntityFactory() = delete;
DUCKVIL_RUNTIME_REFLECTION_RESUME

        EntityFactory(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool, flecs::world& _ecs) :
            m_ecs(_ecs),
            m_pEngineEventPool(_pEngineEventPool),
            m_heap(_heap)
        {

        }

        ~EntityFactory()
        {

        }

        void Init()
        {
            m_eventPool = Event::Pool<Event::mode::immediate>(m_heap);

            m_pEngineEventPool->Add(Utils::lambda(
                [this](InjectConstructorArgumentEvent& _e)
                {
                    if (_e.m_bSuccess)
                    {
                        return;
                    }

                    static const size_t _eventPoolTypeID = typeid(Event::Pool<Event::mode::immediate>*).hash_code();
                    static const size_t _cEventPoolTypeID = typeid(const Event::Pool<Event::mode::immediate>*).hash_code();

                    {
                        const auto& _eventPool = RuntimeReflection::get_meta(_e.m_info.m_typeHandle, _e.m_info.m_constructorHandle, _e.m_info.m_uiArgumentIndex, "EntityFactory");

                        if (_eventPool.m_ullTypeID == typeid(bool).hash_code() && _eventPool.m_pData && *(bool*)_eventPool.m_pData && (_eventPoolTypeID == _e.m_argument.m_ullTypeID || _cEventPoolTypeID == _e.m_argument.m_ullTypeID))
                        {
                            _e.m_pFAP->Push(&m_eventPool);

                            _e.m_bSuccess = true;
                        }
                    }
                }
            ));
        }

        Entity Make()
        {
            Entity _entity = m_ecs.entity();
            EntityCreatedEvent e{ _entity };

            m_eventPool.Broadcast(e);

            return _entity;
        }

        Entity Make(void (*_fn)(Entity& _entity))
        {
            Entity _entity = m_ecs.entity();

            _fn(_entity);

            EntityCreatedEvent e{ _entity };

            m_eventPool.Broadcast(e);

            return _entity;
        }

        void Destroy(Entity& _entity)
        {
            ecs_delete(m_ecs, _entity.m_entity);
        }

        static Entity Clone(const flecs::entity& _entity)
        {
            return Entity(_entity);
        }

        Entity FromID(size_t _ullID)
        {
            return Entity(_ullID, m_ecs);
        }

        bool IsValid(const Entity& _entity) const
        {
            return _entity.m_entity.is_alive();
        }

        template <typename ComponentType, typename... Args>
        Entity RegisterComponent(Args &&... args) const
        {
            return m_ecs.component<ComponentType>(args...);
        }

        Entity LookupComponent(const char* _sComponentTypeName) const
        {
            return m_ecs.lookup(_sComponentTypeName);
        }

        inline Event::Pool<Event::mode::immediate>* GetEventPool() { return &m_eventPool; }
    };

}}