#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Graphics/Renderer/Renderer.h"

#include "Editor/Widgets/ViewportWidget.generated.h"

namespace Duckvil { namespace Editor {

    class ViewportWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        uint32_t m_uiTextureID;

        Graphics::Renderer::renderer_ftable* m_pRenderer;
        Graphics::Renderer::renderer_data* m_pRendererData;

    public:
        ViewportWidget(const Memory::FreeList& _heap);
        ~ViewportWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void SetViewportTexture(uint32_t _pTexture);
        void SetRenderer(Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData);
    };

}}