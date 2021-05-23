#include "Editor/Widgets/ViewportWidget.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

namespace Duckvil { namespace Editor {

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap)
    {
        m_uiTextureID = 1;
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

        uint32_t* _texture = (uint32_t*)m_pRenderer->m_fnGetTextures(m_pRendererData, m_uiTextureID);

        ImGui::Image((void*)(intptr_t)*_texture, ImVec2(500, 500), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    void ViewportWidget::SetViewportTexture(uint32_t _pTexture)
    {
        m_uiTextureID = _pTexture;
    }

    void ViewportWidget::SetRenderer(Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData)
    {
        m_pRenderer = _pRenderer;
        m_pRendererData = _pRendererData;
    }

}}