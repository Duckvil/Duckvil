#define STB_IMAGE_IMPLEMENTATION
#include "Editor/Widgets/ViewportWidget.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Window/Events/KeyDownEvent.h"
#include "Window/Events/KeyUpEvent.h"
#include "Window/Events/MouseMotionEvent.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "tracy/Tracy.hpp"

#include "Graphics/ModelLoader.h"

#include "ImGuizmo/ImGuizmo.h"

#include "Editor/Events/EntitySelectedEvent.h"

namespace Duckvil { namespace Editor {

    void decompose(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale)
    {
        pos = m[3];

        for(int i = 0; i < 3; i++)
            scale[i] = glm::length(glm::vec3(m[i]));

        const glm::mat3 rotMtx(
            glm::vec3(m[0]) / scale[0],
            glm::vec3(m[1]) / scale[1],
            glm::vec3(m[2]) / scale[2]);

        rot = glm::quat_cast(rotMtx);
    }

    ViewportWidget::ViewportWidget()
    {

    }

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::buffered>* _pWindowEventPool, Network::IServer* _pServer, Network::IClient* _pClient, Event::Pool<Event::mode::immediate>* _pEditorEventPool, flecs::world* _pECS, EntityFactory* _pEntityFactory) :
        m_heap(_heap),
        m_pWindowEventPool(_pWindowEventPool),
        m_pClient(_pClient),
        m_pServer(_pServer),
        m_pEditorEventPool(_pEditorEventPool),
        m_pECS(_pECS),
        m_pEntityFactory(_pEntityFactory)
    {
        _pEditorEventPool->Add(
            Utils::lambda(
                [this](const EntitySelectedEvent& _event)
                {
                    m_selectedEntity = _event.m_entity;
                }
            )
        );
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

        auto[_mx, _my] = ImGui::GetMousePos();

        _mx -= m_viewportBounds[0].x;
        _my -= m_viewportBounds[0].y;

        glm::vec2 _viewportSize = m_viewportBounds[1] - m_viewportBounds[0];

        _my = _viewportSize.y - _my;

        if(_mx >= 0 && _my >= 0 && _mx < _viewportSize.x && _my < _viewportSize.y && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing())
        {
            Graphics::Renderer::read_pixel_colors(
                m_heap.GetMemoryInterface(),
                m_pRendererData,
                1,
                (int)_mx,
                (int)_my,
                Utils::lambda([this](int _iValue)
                {
                    if(_iValue == -1)
                    {
                        m_selectedEntity.m_bIsValid = false;

                        return;
                    }

                    if(_iValue == 0)
                    {
                        return;
                    }

                    m_selectedEntity = m_pEntityFactory->FromID(_iValue);
                })
            );
        }

        ImGui::Begin("Viewport");

        auto _viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto _viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto _viewportOffset = ImGui::GetWindowPos();

        m_viewportBounds[0] = glm::vec2(_viewportMinRegion.x + _viewportOffset.x, _viewportMinRegion.y + _viewportOffset.y);
        m_viewportBounds[1] = glm::vec2(_viewportMaxRegion.x + _viewportOffset.x, _viewportMaxRegion.y + _viewportOffset.y);

        ImVec2 _size = ImGui::GetContentRegionAvail();

        if((_size.x != m_oldSize.x || _size.y != m_oldSize.y) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "New viewport size: %f %f", _size.x, _size.y);

            m_viewport.m_uiWidth = _size.x;
            m_viewport.m_uiHeight = _size.y;

            recreate_viewport(&m_viewport, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pRenderer, m_pRendererData, _size.x, _size.y);
            reset(&m_viewport);

            m_oldSize = _size;
        }

        uint32_t* _texture = (uint32_t*)m_pRenderer->m_fnGetTextures(m_pRendererData, m_viewport.m_fboTextureObject);

        ImGui::Image((void*)(intptr_t)*_texture, _size, ImVec2(0, 1), ImVec2(1, 0));

        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(
            m_viewportBounds[0].x,
            m_viewportBounds[0].y,
            m_viewportBounds[1].x - m_viewportBounds[0].x,
            m_viewportBounds[1].y - m_viewportBounds[0].y
        );

        const auto& _view = m_viewport.m_view;
        const auto& _projection = m_viewport.m_projection;

        if(m_selectedEntity.m_bIsValid)
        {
            // TODO: Change that...
            m_selectQuery.each(
                [&](const UUIDComponent& _uuid, Graphics::TransformComponent& _transform)
                {
                    if(!m_selectedEntity.Has<UUIDComponent>() || m_selectedEntity.Get<UUIDComponent>().m_uuid != _uuid.m_uuid)
                    {
                        return;
                    }

                    glm::mat4 _model = glm::translate(_transform.m_position) * glm::toMat4(_transform.m_rotation) * glm::scale(_transform.m_scale);

                    ImGuizmo::Manipulate(glm::value_ptr(_view), glm::value_ptr(_projection), ImGuizmo::OPERATION::UNIVERSAL, ImGuizmo::LOCAL, glm::value_ptr(_model));

                    if(ImGuizmo::IsUsing())
                    {
                        glm::vec3 _position;
                        glm::quat _rotation;
                        glm::vec3 _scale;

                        decompose(_model, _position, _rotation, _scale);
                        
                        _transform.m_position = _position;
                        _transform.m_rotation = _rotation;
                        _transform.m_scale = _scale;
                        
                    }
                }
            );
        }

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
        m_selectQuery = m_pECS->query<UUIDComponent, Graphics::TransformComponent>();
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
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aVertices, 4), // size of vertices should be specified here
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aTexCoords, 2),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ELEMENT_ARRAY_BUFFER, _raw.m_aIndices, 1)
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