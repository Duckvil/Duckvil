#pragma once

#include "Engine/Entity.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil {

    struct ComponentAttachedEvent
    {
        Entity m_entity;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    };

}