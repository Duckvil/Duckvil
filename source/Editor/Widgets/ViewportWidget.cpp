#define STB_IMAGE_IMPLEMENTATION
#include "Editor/Widgets/ViewportWidget.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Window/Events/KeyDownEvent.h"
#include "Window/Events/KeyUpEvent.h"
#include "Window/Events/MouseMotionEvent.h"

#include "glm/gtx/rotate_vector.hpp"

#include "tracy/Tracy.hpp"

#include "Graphics/ModelLoader.h"

namespace Duckvil { namespace Editor {

    ViewportWidget::ViewportWidget()
    {

    }

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::buffered>* _pWindowEventPool, Network::IServer* _pServer, Network::IClient* _pClient) :
        m_heap(_heap),
        m_pWindowEventPool(_pWindowEventPool),
        m_pClient(_pClient),
        m_pServer(_pServer)
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

        bool _anyMovement = false;

        if(m_aKeys[Window::key_a])
        {
            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * (float)_dDelta * 2.f;

            _anyMovement = true;
        }

        if(m_aKeys[Window::key_d])
        {
            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * (float)_dDelta * 2.f;

            _anyMovement = true;
        }

        if(m_aKeys[Window::key_w])
        {
            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * (float)_dDelta * 2.f;

            _anyMovement = true;
        }

        if(m_aKeys[Window::key_s])
        {
            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * (float)_dDelta * 2.f;

            _anyMovement = true;
        }

        if(m_aKeys[Window::key_k])
        {
            m_bWrapCamera = !m_bWrapCamera;
        }

        if(_anyMovement)
        {
            Network::Message _message(NetworkCommands::PlayerMovement);

            _message << m_viewport.m_position;

            if(m_pServer)
            {
                _message << -1;

                m_pServer->MessageAllClients(_message);
            }
            else if(m_pClient)
            {
                _message << m_pClient->GetID();

                m_pClient->Send(_message);
            }
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

        ecs_os_set_api_defaults();

        m_networkQuery = m_pECS->query<NetworkComponent, Graphics::TransformComponent>();
    }

    void ViewportWidget::SetOwner(Duckvil::Network::IConnection::Owner _owner)
    {
        m_owner = _owner;
    }

    void ViewportWidget::SpwanPlayer(uint32_t _uiID)
    {
        const auto& _modelLoaderHandle = RuntimeReflection::get_type("ModelLoader", { "Duckvil", "Graphics" });
        
        std::filesystem::path _cwd = DUCKVIL_CWD;

        void* _modelLoader = RuntimeReflection::create<const char*>(m_heap, _modelLoaderHandle, false, (_cwd / "resource/object/test.obj").string().c_str());

        const auto& _modelLoaderGetRawHandle = RuntimeReflection::get_function_handle(_modelLoaderHandle, "GetRaw");
        const Graphics::ModelLoader::Raw& _raw = RuntimeReflection::invoke_member_result<const Graphics::ModelLoader::Raw&>(_modelLoaderHandle, _modelLoaderGetRawHandle, _modelLoader);

        Graphics::Renderer::vertex_buffer_object_descriptor _desc[] =
        {
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aVertices, _raw.m_aVertices.size(), 4), // size of vertices should be specified here
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aTexCoords, _raw.m_aTexCoords.size(), 2),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ELEMENT_ARRAY_BUFFER, _raw.m_aIndices, _raw.m_aIndices.size(), 1)
        };

        m_pECS->entity().set([&](Graphics::MeshComponent& _mesh, Graphics::TransformComponent& _transform, NetworkComponent& _netComponent)
        {
            _mesh.m_uiID = m_pRenderer->m_fnCreateVAO(
                m_heap.GetMemoryInterface(),
                m_heap.GetAllocator(),
                m_pRendererData,
                Graphics::Renderer::vertex_array_object_descriptor
                {
                    sizeof(_desc) / sizeof(_desc[0]),
                    _desc
                }
            );

            _transform.m_position = glm::vec3(0, 2, 2);
            _transform.m_rotation = glm::quat(0, 0, 0, 1);
            _transform.m_scale = glm::vec3(1, 1, 1);

            _netComponent.m_uiID = _uiID;
        });
    }

    bool ViewportWidget::OnMessage(const Duckvil::Network::Message& _message, std::shared_ptr<Duckvil::Network::IConnection> _pClient)
    {
        Network::Message _msg = _message;

        if(_message == NetworkCommands::PlayerMovement)
        {
            uint32_t _clientID = -1;

            _msg >> _clientID;

            if(m_owner == Network::IConnection::Owner::CLIENT)
            {
                m_networkQuery.each(
                    [&_msg, this, _clientID](const NetworkComponent& _netComponent, Graphics::TransformComponent& _transform)
                    {
                        if(_clientID == _netComponent.m_uiID)
                        {
                            _msg >> _transform.m_position;
                        }
                    }
                );
            }
            else
            {
                m_networkQuery.each(
                    [&_msg, _clientID](const NetworkComponent& _netComponent, Graphics::TransformComponent& _transform)
                    {
                        if(_clientID == _netComponent.m_uiID)
                        {
                            _msg >> _transform.m_position;
                        }
                    }
                );

                m_pServer->MessageAllClients(_message);
            }

            return true;
        }
        else if(_message == Duckvil::Network::CommonCommands::Client_AssignID)
        {
            if(m_owner == Network::IConnection::Owner::CLIENT)
            {
                SpwanPlayer(-1);

                return true;
            }
        }
        else if(_message == Duckvil::Network::CommonCommands::Client_RegisterWithServer)
        {
            if(m_owner == Network::IConnection::Owner::SERVER)
            {
                uint32_t _clientID = _pClient->GetID();

                SpwanPlayer(_clientID);

                Network::Message _message(Network::CommonCommands::Client_AssignID);

                _message << _clientID;

                m_pServer->MessageClient(_pClient, _message);

                _message = Network::Message(NetworkCommands::AddPlayer);

                _message << _clientID;

                m_pServer->MessageAllClients(_message, _pClient);

                m_networkQuery.each(
                    [this, _pClient](const NetworkComponent& _netComponent, Graphics::TransformComponent& _transform)
                    {
                        Network::Message _message(NetworkCommands::AddPlayer);

                        _message << _netComponent.m_uiID;

                        m_pServer->MessageClient(_pClient, _message);
                    }
                );

                return true;
            }
        }
        else if(_message == NetworkCommands::AddPlayer)
        {
            if(m_owner == Network::IConnection::Owner::CLIENT)
            {
                uint32_t _clientID = -1;

                _msg >> _clientID;

                SpwanPlayer(_clientID);

                return true;
            }
        }

        return false;
    }

}}