#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "Engine/Events/EntityCreatedEvent.h"

#include "nlohmann/json.hpp"

#include "ECS/EntityFactory.h"

namespace Duckvil { namespace Serializer {

    DUCKVIL_CLASS(ReflectionFlags_AutoEventsAdding, Duckvil::ReflectionFlags_AutoInstantiate = false, ReflectionFlags_AutoEventsAdding)
    class EntitySerializerSystem : public ISystem
    {
    private:
        nlohmann::json m_j;

        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;
        Event::Pool<Event::mode::immediate>* m_pEntityFactoryEventPool;

        Memory::Vector<ECS::Entity> m_entities;

        ECS::EntityFactory* m_pEntityFactory;

        nlohmann::json Serialize(const ECS::Entity& _entity);
        void Deserialize(const nlohmann::json& _jEntity, ECS::Entity& _entity);

    public:
        EntitySerializerSystem(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Engine") Event::Pool<Event::mode::immediate>* _pEngineEventPool, ECS::EntityFactory* _pEntityFactory);
        ~EntitySerializerSystem();

        bool Init();
        void Update(double _dDelta);

        void Load(const std::filesystem::path& _sFile, void (*_fn)(ECS::Entity& _entity) = nullptr);
        void Save(const std::filesystem::path& _sFile);

        void OnEvent(EntityCreatedEvent& _event);
    };

}}