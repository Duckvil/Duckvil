#pragma once

#include "ECS/Entity.h"

namespace Duckvil { namespace Editor {

    struct EntitySelectedEvent
    {
        ECS::Entity m_entity;
    };

}}