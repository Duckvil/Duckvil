#pragma once

#include "glm/glm.hpp"

#include "glm/gtx/quaternion.hpp"

namespace Duckvil { namespace Graphics {

    struct TransformComponent
    {
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };

}}