#pragma once

#include "glm/glm.hpp"

#include "glm/gtx/quaternion.hpp"

#include "Engine/Entity.h"

#include <vector>

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Markers.h"

#include "Utils/EntitySerializer.h"

#include "Graphics/TransformComponent.generated.h"

namespace Duckvil { namespace Graphics {

    DUCKVIL_STRUCT("Component")
    struct TransformComponent
    {
        DUCKVIL_GENERATED_BODY

        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };

}}