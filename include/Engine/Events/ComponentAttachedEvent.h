#pragma once

#include "ECS/Entity.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil {

    struct ComponentAttachedEvent
    {
        ECS::Entity m_entity;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    };

}