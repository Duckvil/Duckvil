#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "Engine/Events/EntityCreatedEvent.h"

#include "nlohmann/json.hpp"

#include "Engine/EntityFactory.h"

namespace Duckvil { namespace Serializer {

    DUCKVIL_CLASS(ReflectionFlags_AutoEventsAdding, Duckvil::ReflectionFlags_AutoInstantiate = false)
    class EntitySerializerSystem : public ISystem
    {
    private:
        nlohmann::json m_j;

        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;
        Event::Pool<Event::mode::immediate>* m_pEntityFactoryEventPool;

        Memory::Vector<Entity> m_entities;

        EntityFactory* m_pEntityFactory;

        nlohmann::json Serialize(const Entity& _entity);
        void Deserialize(const nlohmann::json& _jEntity, Entity& _entity);

    public:
        EntitySerializerSystem(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Engine") Event::Pool<Event::mode::immediate>* _pEngineEventPool, DUCKVIL_ARGUMENT("EntityFactory") Event::Pool<Event::mode::immediate>* _pEntityFactoryEventPool, EntityFactory* _pEntityFactory);
        ~EntitySerializerSystem();

        bool Init();
        void Update(double _dDelta);

        void Load(const std::filesystem::path& _sFile, void (*_fn)(Entity& _entity) = nullptr);
        void Save(const std::filesystem::path& _sFile);
    };

}}