#pragma once

#include "ECS/Entity.h"

namespace Duckvil {

    struct EntityDestroyedEvent
    {
        ECS::Entity m_entity;
    };

}