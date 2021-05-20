#include "Editor/Widgets/ViewportWidget.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

namespace Duckvil { namespace Editor {

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap)
    {
        m_pTextureID = 0;
    }

    ViewportWidget::~ViewportWidget()
    {

    }

    void ViewportWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void ViewportWidget::OnDraw()
    {
        ImGui::Begin("Viewport");

        ImGui::Image(m_pTextureID, ImVec2(500, 500));

        ImGui::End();
    }

    void ViewportWidget::SetViewportTexture(void* _pTexture)
    {
        m_pTextureID = _pTexture;
    }

}}