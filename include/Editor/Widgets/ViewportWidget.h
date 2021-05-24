#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Graphics/Renderer/Renderer.h"

#include "imgui/imgui.h"

#include "Logger/Logger.h"

#include "Event/BufferedPool.h"

#include "Editor/Viewport.h"

#include "Window/Key.h"

#include "Window/Events/SetMousePositionEvent.h"

#include "Editor/Widgets/ViewportWidget.generated.h"

namespace Duckvil { namespace Editor {

    class ViewportWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;

        uint32_t m_uiTextureID;

        Graphics::Renderer::renderer_ftable* m_pRenderer;
        Graphics::Renderer::renderer_data* m_pRendererData;

        ImVec2 m_oldSize;

        Event::Pool<Event::mode::buffered>* m_pWindowEventPool;

        viewport m_viewport;

        bool m_bWrapCamera = false;
        bool m_aKeys[Window::key_size];
        bool m_bIsWindowFocused = false;

        Window::SetMousePositionEvent m_setMousePosition;

    public:
        ViewportWidget(const Memory::FreeList& _heap);
        ~ViewportWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void SetViewportTexture(uint32_t _pTexture);
        void SetRenderer(Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData);
        void SetEventPool(Event::Pool<Event::mode::buffered>* _pEventPool);
    };

}}