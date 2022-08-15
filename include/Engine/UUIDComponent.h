#pragma once

#include "Utils/UUID.h"

#include "imgui/imgui.h"

#include "Engine/UUIDComponent.generated.h"

namespace Duckvil {

    DUCKVIL_STRUCT("Component")
    struct UUIDComponent
    {
        DUCKVIL_GENERATED_BODY

        UUID m_uuid;

        static void InitEditor(void* _pContext)
        {
            ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pContext));
        }

        static void Draw(const char* _sLabel, UUID* _pValue)
        {
            ImGui::Text("UUID: %llu", _pValue->Hash());
        }
    };

}