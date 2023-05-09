#pragma once

#include "flecs/flecs.h"

namespace Duckvil { namespace ECS {

    class EntityFactory;

    class Entity
    {
        friend class EntityFactory;
    private:
        

        Entity(const flecs::entity& _entity) :
            m_entity(_entity)
        {

        }

        Entity(size_t _ullID, flecs::world_t* _pWorld) :
            m_entity(_pWorld, _ullID)
        {

        }

    public:
        Entity()
        {

        }

        ~Entity()
        {

        }

        flecs::entity m_entity;

        template <typename Type>
        inline Entity& Add(const Type& _component)
        {
            m_entity.set(_component);

            return *this;
        }

        inline Entity& Add(const Entity& _component)
        {
            m_entity.add(_component.m_entity);

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

        inline bool Has(const Entity& _entity) const
        {
            return m_entity.has(_entity.m_entity);
        }

        template <typename Type>
        inline const Type& Get() const
        {
            return *m_entity.get<Type>();
        }

        template <typename Type>
        inline Type* GetMut() const
        {
            return m_entity.get_mut<Type>();
        }

        template <typename Func>
        void Each(const Func _fn) const
        {
            m_entity.each(_fn);
        }

        size_t ID() const
        {
            return m_entity;
        }
    };

}}