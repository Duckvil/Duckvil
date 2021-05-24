#define STB_IMAGE_IMPLEMENTATION
#include "Editor/Widgets/ViewportWidget.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Window/Events/KeyDownEvent.h"
#include "Window/Events/KeyUpEvent.h"
#include "Window/Events/MouseMotionEvent.h"

#include "glm/gtx/rotate_vector.hpp"

namespace Duckvil { namespace Editor {

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap) :
        m_heap(_heap)
    {
        m_uiTextureID = 1;
    }

    ViewportWidget::~ViewportWidget()
    {

    }

    void ViewportWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);

        setup_viewport(
            &m_viewport,
            m_heap.GetMemoryInterface(),
            m_heap.GetAllocator(),
            m_pRenderer,
            m_pRendererData
        );

        m_setMousePosition.m_iX = 1920 / 2;
        m_setMousePosition.m_iY = 1080 / 2;
    }

    void ViewportWidget::OnDraw()
    {
        while(m_pWindowEventPool->AnyEvents())
        {
            Window::KeyDownEvent _keyDownEvent;
            Window::KeyUpEvent _keyUpEvent;
            Window::MouseMotionEvent _mouseMotionEvent;

            if(m_pWindowEventPool->GetMessage(&_keyDownEvent))
            {
                m_aKeys[_keyDownEvent.m_key] = true;

                m_pWindowEventPool->EventHandled<Window::KeyDownEvent>();
            }
            else if(m_pWindowEventPool->GetMessage(&_keyUpEvent))
            {
                m_aKeys[_keyUpEvent.m_key] = false;

                m_pWindowEventPool->EventHandled<Window::KeyUpEvent>();
            }
            else if(m_pWindowEventPool->GetMessage(&_mouseMotionEvent))
            {
                if(!m_bWrapCamera)
                {
                    m_pWindowEventPool->EventHandled<Window::MouseMotionEvent>();

                    continue;
                }

                int _x = (1920 / 2) - _mouseMotionEvent.m_iX;
                int _y = (1080 / 2) - _mouseMotionEvent.m_iY;

                m_viewport.m_rotation = glm::normalize(glm::angleAxis(_y * 0.001f, m_viewport.m_rotation * glm::vec3(-1, 0, 0)) * m_viewport.m_rotation);
                m_viewport.m_rotation = glm::normalize(glm::angleAxis(_x * 0.001f, glm::vec3(0, -1, 0)) * m_viewport.m_rotation);

                m_pWindowEventPool->EventHandled<Window::MouseMotionEvent>();
                m_pWindowEventPool->Broadcast(m_setMousePosition);
            }
        }

        if(m_aKeys[Window::key_a])
        {
            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * 0.001f;
        }

        if(m_aKeys[Window::key_d])
        {
            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * 0.001f;
        }

        if(m_aKeys[Window::key_w])
        {
            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * 0.001f;
        }

        if(m_aKeys[Window::key_s])
        {
            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * 0.001f;
        }

        if(m_aKeys[Window::key_k] && m_bIsWindowFocused)
        {
            m_bWrapCamera = !m_bWrapCamera;
        }

        render_viewport(&m_viewport, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pRenderer, m_pRendererData);

        ImGui::Begin("Viewport");

        ImVec2 _size = ImGui::GetContentRegionAvail();

        if((_size.x != m_oldSize.x || _size.y != m_oldSize.y) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "New viewport size: %f %f", _size.x, _size.y);

            m_oldSize = _size;
        }

        uint32_t* _texture = (uint32_t*)m_pRenderer->m_fnGetTextures(m_pRendererData, m_uiTextureID);

        ImGui::Image((void*)(intptr_t)*_texture, m_oldSize, ImVec2(0, 1), ImVec2(1, 0));

        m_bIsWindowFocused = ImGui::IsWindowFocused();

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

    void ViewportWidget::SetEventPool(Event::Pool<Event::mode::buffered>* _pEventPool)
    {
        m_pWindowEventPool = _pEventPool;
    }

}}