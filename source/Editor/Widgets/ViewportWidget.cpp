#define STB_IMAGE_IMPLEMENTATION
#include "Editor/Widgets/ViewportWidget.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Window/Events/KeyDownEvent.h"
#include "Window/Events/KeyUpEvent.h"
#include "Window/Events/MouseMotionEvent.h"

#include "glm/gtx/rotate_vector.hpp"

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace Editor {

    ViewportWidget::ViewportWidget()
    {

    }

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::buffered>* _pWindowEventPool) :
        m_heap(_heap),
        m_pWindowEventPool(_pWindowEventPool)
    {

    }

    ViewportWidget::~ViewportWidget()
    {

    }

    bool ViewportWidget::Init()
    {
        return true;
    }

    void ViewportWidget::Update(double _dDelta)
    {
        ZoneScopedN("Viewport update");

        m_pWindowEventPool->Reset();

        while(m_pWindowEventPool->AnyEvents())
        {
            static Window::KeyDownEvent _keyDownEvent;
            static Window::KeyUpEvent _keyUpEvent;
            static Window::MouseMotionEvent _mouseMotionEvent;

            if(m_pWindowEventPool->GetMessage(&_keyDownEvent))
            {
                if(m_bIsWindowFocused)
                {
                    m_aKeys[_keyDownEvent.m_key] = true;
                }

                m_pWindowEventPool->EventHandled(_keyDownEvent);
            }
            else if(m_pWindowEventPool->GetMessage(&_keyUpEvent))
            {
                if(m_bIsWindowFocused)
                {
                    m_aKeys[_keyUpEvent.m_key] = false;
                }

                m_pWindowEventPool->EventHandled(_keyUpEvent);
            }
            else if(m_pWindowEventPool->GetMessage(&_mouseMotionEvent))
            {
                if(!m_bWrapCamera || !m_bIsWindowFocused)
                {
                    m_pWindowEventPool->EventHandled<Window::MouseMotionEvent>();

                    continue;
                }

                int _x = (1920 / 2) - _mouseMotionEvent.m_iX;
                int _y = (1080 / 2) - _mouseMotionEvent.m_iY;

                m_viewport.m_rotation = glm::normalize(glm::angleAxis(_y * (float)_dDelta, m_viewport.m_rotation * glm::vec3(-1, 0, 0)) * m_viewport.m_rotation);
                m_viewport.m_rotation = glm::normalize(glm::angleAxis(_x * (float)_dDelta, glm::vec3(0, -1, 0)) * m_viewport.m_rotation);

                m_pWindowEventPool->EventHandled(_mouseMotionEvent);
                m_pWindowEventPool->Broadcast(m_setMousePosition);
            }
            else
            {
                m_pWindowEventPool->Skip();
            }
        }

        if(m_aKeys[Window::key_a])
        {
            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * (float)_dDelta * 2.f;
        }

        if(m_aKeys[Window::key_d])
        {
            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * (float)_dDelta * 2.f;
        }

        if(m_aKeys[Window::key_w])
        {
            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * (float)_dDelta * 2.f;
        }

        if(m_aKeys[Window::key_s])
        {
            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * (float)_dDelta * 2.f;
        }

        if(m_aKeys[Window::key_k])
        {
            m_bWrapCamera = !m_bWrapCamera;
        }
    }

    void ViewportWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);

        if(!m_bSkip)
        {
            m_viewport.m_uiWidth = 1920;
            m_viewport.m_uiHeight = 1080;

            setup_viewport(
                &m_viewport,
                m_heap.GetMemoryInterface(),
                m_heap.GetAllocator(),
                m_pRenderer,
                m_pRendererData,
                m_pECS
            );

            m_bSkip = true;
        }

        m_setMousePosition.m_iX = 1920 / 2;
        m_setMousePosition.m_iY = 1080 / 2;
    }

    void ViewportWidget::OnDraw()
    {
        ZoneScopedN("Viewport draw");

        render_viewport(&m_viewport, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pRenderer, m_pRendererData);

        ImGui::Begin("Viewport");

        ImVec2 _size = ImGui::GetContentRegionAvail();

        if((_size.x != m_oldSize.x || _size.y != m_oldSize.y) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "New viewport size: %f %f", _size.x, _size.y);

            m_viewport.m_uiWidth = _size.x;
            m_viewport.m_uiHeight = _size.y;

            reset(&m_viewport);

            m_oldSize = _size;
        }

        uint32_t* _texture = (uint32_t*)m_pRenderer->m_fnGetTextures(m_pRendererData, m_viewport.m_fboTextureObject);

        ImGui::Image((void*)(intptr_t)*_texture, _size, ImVec2(0, 1), ImVec2(1, 0));

        m_bIsWindowFocused = ImGui::IsWindowFocused();

        ImGui::End();
    }

    void ViewportWidget::SetRenderer(Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData)
    {
        m_pRenderer = _pRenderer;
        m_pRendererData = _pRendererData;
    }

    void ViewportWidget::SetECS(flecs::world* _pECS)
    {
        m_pECS = _pECS;
    }

}}