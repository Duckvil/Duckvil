#pragma once

#include "flecs/flecs.h"

#define DUCKVIL_RUNTIME_REFLECTION_PAUSE
#define DUCKVIL_RUNTIME_REFLECTION_RESUME

namespace Duckvil {

    class Entity
    {
        friend class EntityFactory;
    private:
        flecs::entity m_entity;

        Entity(const flecs::entity& _entity) :
            m_entity(_entity)
        {

        }

    public:
DUCKVIL_RUNTIME_REFLECTION_PAUSE
        Entity() = delete;
DUCKVIL_RUNTIME_REFLECTION_RESUME
        ~Entity()
        {

        }

        template <typename Type>
        inline Entity& Add(const Type& _component)
        {
            m_entity.set(_component);

            return *this;
        }

        template <typename Type>
        inline Entity& Add()
        {
            m_entity.add<Type>();

            return *this;
        }

        template <typename Type>
        inline Entity& Remove()
        {
            m_entity.remove<Type>();

            return *this;
        }

        template <typename Type>
        inline bool Has() const
        {
            return m_entity.has<Type>();
        }

        template <typename Type>
        inline const Type& Get() const
        {
            return *m_entity.get<Type>();
        }
    };

}