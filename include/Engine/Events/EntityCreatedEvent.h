#pragma once

#include "ECS/Entity.h"

namespace Duckvil {

    struct EntityCreatedEvent
    {
        ECS::Entity m_entity;
    };

}