#pragma once

#include "glm/glm.hpp"

#include "glm/gtx/quaternion.hpp"

#include "Engine/Entity.h"

#include <vector>

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Markers.h"

#include "Utils/EntitySerializer.h"

#include "imgui/imgui.h"

#include "Graphics/TransformComponent.generated.h"

namespace Duckvil { namespace Graphics {

    DUCKVIL_STRUCT("Component")
    struct TransformComponent
    {
        DUCKVIL_GENERATED_BODY

        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        static void InitEditor(void* _pContext)
        {
            ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pContext));
        }

        static void Draw(const char* _sLabel, glm::vec3* _pValue)
        {
            ImGui::DragFloat3(_sLabel, (float*)_pValue, 0.1f);
        }

        static void Draw(const char* _sLabel, glm::quat* _pValue)
        {
            auto& _quat = *_pValue;
            glm::vec3 _rotation = glm::eulerAngles(_quat);

            _rotation = glm::degrees(_rotation);

            ImGui::DragFloat3(_sLabel, (float*)&_rotation, 0.1f);

            _rotation = glm::radians(_rotation);

            _quat = glm::quat(_rotation);
        }
    };

}}