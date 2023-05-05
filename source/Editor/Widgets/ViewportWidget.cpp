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

#include "ProjectManager/Events/SimulationStartEvent.h"
#include "ProjectManager/Events/SimulationStopEvent.h"

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

    ViewportWidget::ViewportWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::buffered>* _pWindowEventPool, Network::IServer* _pServer, Network::IClient* _pClient, Event::Pool<Event::mode::immediate>* _pEditorEventPool, flecs::world* _pECS, EntityFactory* _pEntityFactory, Event::Pool<Event::mode::immediate>* _pProjectEventPool) :
        m_heap(_heap),
        m_pWindowEventPool(_pWindowEventPool),
        m_pClient(_pClient),
        m_pServer(_pServer),
        m_pEditorEventPool(_pEditorEventPool),
        m_pECS(_pECS),
        m_pEntityFactory(_pEntityFactory),
        m_pProjectEventPool(_pProjectEventPool),
		m_bSimulating(false),
		m_bPaused(false)
    {
        _pEditorEventPool->Add<EntitySelectedEvent>(this, &ViewportWidget::OnEvent);
        _pEditorEventPool->Add<EntityDestroyedEvent>(this, &ViewportWidget::OnEvent);

        _pEntityFactory->GetEventPool()->Add<EntityCreatedEvent>(this, &ViewportWidget::OnEvent);
    }

    ViewportWidget::~ViewportWidget()
    {

    }

    bool ViewportWidget::Init()
    {
        ecs_os_set_api_defaults();

        return true;
    }

    void ViewportWidget::Update(double _dDelta)
    {
        m_dDelta = _dDelta;

        ZoneScopedN("Viewport update");

        m_pWindowEventPool->Reset();

        while(m_pWindowEventPool->AnyEvents())
        {
            static Window::KeyDownEvent _keyDownEvent;
            static Window::KeyUpEvent _keyUpEvent;
            static Window::MouseMotionEvent _mouseMotionEvent;

            if(m_pWindowEventPool->GetMessage(&_keyDownEvent))
            {
                if(m_bLookingAround)
                {
                    m_aKeys[_keyDownEvent.m_key] = true;
                }

                m_pWindowEventPool->EventHandled(_keyDownEvent);
            }
            else if(m_pWindowEventPool->GetMessage(&_keyUpEvent))
            {
            	m_aKeys[_keyUpEvent.m_key] = false;

                m_pWindowEventPool->EventHandled(_keyUpEvent);
            }
            else if(m_pWindowEventPool->GetMessage(&_mouseMotionEvent))
            {
                m_pWindowEventPool->EventHandled<Window::MouseMotionEvent>();
            }
            else
            {
                m_pWindowEventPool->Skip();
            }
        }

        bool _anyMovement = false;

        if(m_bLookingAround)
        {
	        if(m_aKeys[Window::key_a])
	        {
	            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * (float)_dDelta * 4.f;

	            _anyMovement = true;
	        }

	        if(m_aKeys[Window::key_d])
	        {
	            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * (float)_dDelta * 4.f;

	            _anyMovement = true;
	        }

	        if(m_aKeys[Window::key_w])
	        {
	            m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * (float)_dDelta * 4.f;

	            _anyMovement = true;
	        }

	        if(m_aKeys[Window::key_s])
	        {
	            m_viewport.m_position -= glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, 1)) * (float)_dDelta * 4.f;

	            _anyMovement = true;
	        }
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

#ifndef DUCKVIL_TRACY_EDITOR
        m_uiReaderHandle = m_pRenderer->m_fnCreateFrameBufferReader(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pRendererData, 320, 180, Utils::Graphics::FrameBufferReader::RGBA, Utils::Graphics::FrameBufferReader::UNSIGNED_BYTE);
#endif

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
    }

    void AlignForWidth(float width, float alignment = 0.5f)
	{
	    float avail = ImGui::GetContentRegionAvail().x;
	    float off = (avail - width) * alignment;

	    if(off > 0.0f)
	        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

    void ViewportWidget::OnDraw()
    {
        ZoneScopedN("Viewport draw");

        FrameMarkStart("Mouse logic");

        static ImVec2 _mousePos;

        if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right) || ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Middle))
        {
            _mousePos = ImGui::GetMousePos();
        }

        ImGuiIO& _io = ImGui::GetIO();

        if(ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Middle))
        {
            auto _newMousePos = ImGui::GetMousePos();

            if(_mousePos.x != _newMousePos.x || _mousePos.y != _newMousePos.y)
            {
                if(_io.KeyShift)
                {
                    m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(0, 0, -1)) * ((_newMousePos.y - _mousePos.y) * (float)m_dDelta * 8.f);
                }
                else
                {
                    m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(1, 0, 0)) * ((_newMousePos.x - _mousePos.x) * (float)m_dDelta * 8.f);
                    m_viewport.m_position += glm::normalize(m_viewport.m_rotation * glm::vec3(0, 1, 0)) * ((_newMousePos.y - _mousePos.y) * (float)m_dDelta * 8.f);
                }
            }

            _mousePos = _newMousePos;
        }

        FrameMarkEnd("Mouse logic");

        FrameMarkStart("Pre-render");
#ifndef DUCKVIL_TRACY_EDITOR
        Graphics::Renderer::frame_buffer_reader_update(m_heap.GetMemoryInterface(), m_pRendererData, m_uiReaderHandle, [](void* _pTextureData, uint32_t _uiWidth, uint32_t _uiHeight, uint32_t _uiOffset)
            {
                FrameImage(_pTextureData, _uiWidth, _uiHeight, _uiOffset, true);
            });
#endif

        render_viewport(&m_viewport, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pRenderer, m_pRendererData);

#ifndef DUCKVIL_TRACY_EDITOR
        Graphics::Renderer::frame_buffer_reader_read(m_heap.GetMemoryInterface(), m_pRendererData, m_uiReaderHandle, m_viewport.m_fbo, m_viewport.m_uiWidth, m_viewport.m_uiHeight, 0);
#endif

        FrameMarkEnd("Pre-render");

        auto[_mx, _my] = ImGui::GetMousePos();

        _mx -= m_viewportBounds[0].x;
        _my -= m_viewportBounds[0].y;

        glm::vec2 _viewportSize = m_viewportBounds[1] - m_viewportBounds[0];

        _my = _viewportSize.y - _my;

        if(_mx >= 0 && _my >= 0 && _mx < _viewportSize.x && _my < _viewportSize.y && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing())
        {
            Graphics::Renderer::read_pixel(
                m_heap.GetMemoryInterface(),
                m_pRendererData,
                1,
                (int)_mx,
                (int)_my,
                m_viewport.m_fbo,
                Utils::lambda([&](uint32_t _uiValue)
                {
                    if(_uiValue == 0)
                    {
                        m_selectedEntity = Entity();

                        m_pEditorEventPool->Broadcast(EntitySelectedEvent{ m_selectedEntity });

                        return;
                    }

                    m_pEditorEventPool->Broadcast(EntitySelectedEvent{ m_pEntityFactory->FromID(_uiValue) });
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

        if((_size.x != m_oldSize.x || _size.y != m_oldSize.y) && _size.x >= 320 && _size.y >= 180 && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            DUCKVIL_LOG_INFO(LoggerChannelID::Default, "New viewport size: %f %f", _size.x, _size.y);

            m_viewport.m_uiWidth = _size.x;
            m_viewport.m_uiHeight = _size.y;

            recreate_viewport(&m_viewport, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pRenderer, m_pRendererData, _size.x, _size.y);
            reset(&m_viewport);

            m_oldSize = _size;
        }

        ImGuiStyle& style = ImGui::GetStyle();

        float width = 0.0f;
        
        if(m_bSimulating)
        {
            width += ImGui::CalcTextSize("Stop").x;
            width += style.ItemSpacing.x;
            width += ImGui::CalcTextSize("Pause").x;

            if(m_bPaused)
            {
                width += style.ItemSpacing.x;
                width += ImGui::CalcTextSize("Resume").x;
            }

            AlignForWidth(width);

            if(ImGui::Button("Stop"))
            {
                m_bSimulating = false;

                m_pProjectEventPool->Broadcast(ProjectManager::SimulationStopEvent{});
            }

            ImGui::SameLine();

            if(ImGui::Button("Pause"))
            {
                m_bPaused = true;
            }

            if(m_bPaused)
            {
	            ImGui::SameLine();

	            if(ImGui::Button("Resume"))
	            {
	                m_bPaused = false;
	            }
            }
        }
        else
        {
            width += ImGui::CalcTextSize("Start").x;

            AlignForWidth(width);

	        if(ImGui::Button("Start"))
	        {
                m_bSimulating = true;
                m_bPaused = false;

                m_pProjectEventPool->Broadcast(ProjectManager::SimulationStartEvent{});
	        }
        }

        uint32_t* _texture = (uint32_t*)m_pRenderer->m_fnGetTextures(m_pRendererData, m_viewport.m_fboTextureObject);

        if(_size.x < 320)
        {
            _size.x = 320;
        }

        if (_size.y < 180)
        {
            _size.y = 180;
        }

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

        if(m_pEntityFactory->IsValid(m_selectedEntity) && m_selectedEntity.Has<Graphics::TransformComponent>())
        {
            auto _t = m_selectedEntity.m_entity.get_mut<Graphics::TransformComponent>();

            glm::mat4 _model = glm::translate(_t->m_position) * glm::toMat4(_t->m_rotation) * glm::scale(_t->m_scale);

            ImGuizmo::Manipulate(glm::value_ptr(_view), glm::value_ptr(_projection), ImGuizmo::OPERATION::UNIVERSAL, ImGuizmo::LOCAL, glm::value_ptr(_model));

            if(ImGuizmo::IsUsing())
            {
                glm::vec3 _position;
                glm::quat _rotation;
                glm::vec3 _scale;

                decompose(_model, _position, _rotation, _scale);

                _t->m_position = _position;
                _t->m_rotation = _rotation;
                _t->m_scale = _scale;

                Network::Message _message(NetworkCommands::EntityUpdate);
                char _bytes[16];

                m_selectedEntity.Get<UUIDComponent>().m_uuid.GetBytes(_bytes);

                _message << _position << _rotation << _scale << _bytes << EntityUpdateKind::EntityUpdateKind_Transform;

                if(m_pServer != nullptr)
                {
                    m_pServer->MessageAllClients(_message);
                }
                else if(m_pClient != nullptr)
                {
                    m_pClient->Send(_message);
                }
            }
        }

        bool _isWindowHovered = ImGui::IsWindowHovered();
        static bool _looked = false;

        if(_isWindowHovered && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            m_bLookingAround = true;
        }

        if(_isWindowHovered && !_looked && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("##viewportContextMenuPopup");
        }

        if(ImGui::BeginPopup("##viewportContextMenuPopup"))
        {
            if(m_pEntityFactory->IsValid(m_selectedEntity))
            {
                if(ImGui::Button("Delete"))
                {
                    Entity _entity = m_selectedEntity;

                    m_selectedEntity = Entity();

                    m_pEditorEventPool->Broadcast(EntitySelectedEvent{ m_selectedEntity });

                    m_pEntityFactory->Destroy(_entity);

                    ImGui::CloseCurrentPopup();
                }

                ImGui::Separator();
            }

            ImGui::Text("Some window context content");

            ImGui::EndPopup();
        }

        if(m_bLookingAround && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            m_bLookingAround = false;
            _looked = false;
        }

        if(m_bLookingAround)
        {
            if(_mousePos.x >= ImGui::GetWindowPos().x + ImGui::GetWindowSize().x)
            {
                _mousePos = ImVec2(ImGui::GetWindowPos().x + 1, _mousePos.y);

                m_pWindowEventPool->Broadcast(Window::SetMousePositionEvent{ (int)_mousePos.x, (int)_mousePos.y, true });
            }
            else if(_mousePos.x <= ImGui::GetWindowPos().x)
            {
                _mousePos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 1, _mousePos.y);

                m_pWindowEventPool->Broadcast(Window::SetMousePositionEvent{ (int)_mousePos.x, (int)_mousePos.y, true });
            }
            else if(_mousePos.y >= ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)
            {
                _mousePos = ImVec2(_mousePos.x, ImGui::GetWindowPos().y + 1);

                m_pWindowEventPool->Broadcast(Window::SetMousePositionEvent{ (int)_mousePos.x, (int)_mousePos.y, true });
            }
            else if(_mousePos.y <= ImGui::GetWindowPos().y)
            {
                _mousePos = ImVec2(_mousePos.x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 1); // ImGui::GetMousePos();

                m_pWindowEventPool->Broadcast(Window::SetMousePositionEvent{ (int)_mousePos.x, (int)_mousePos.y, true });
            }
            else
            {
                auto _newMousePos = ImGui::GetMousePos();

                if(_mousePos.x != _newMousePos.x || _mousePos.y != _newMousePos.y)
                {
                    float _delta = static_cast<float>(m_dDelta);
                    auto _x = ImVec2(_newMousePos.x * _delta, _newMousePos.y * _delta);
                    auto _y = ImVec2(_mousePos.x * _delta, _mousePos.y * _delta);
                    const float _speed = 0.1f;

                    m_viewport.m_rotation = glm::normalize(glm::angleAxis((_x.y - _y.y) * _speed, m_viewport.m_rotation * glm::vec3(1, 0, 0)) *
                        glm::angleAxis((_x.x - _y.x) * _speed, glm::vec3(0, 1, 0))) * m_viewport.m_rotation;

                    _looked = true;
                }

                _mousePos = _newMousePos;
            }
        }

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

        std::vector<int> _id(_raw.m_aVertices.size(), 0);

        Graphics::Renderer::vertex_buffer_object_descriptor _desc[] =
        {
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aVertices, 4), // size of vertices should be specified here
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _raw.m_aTexCoords, 2),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _id, 1),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ELEMENT_ARRAY_BUFFER, _raw.m_aIndices, 1)
        };

        _desc[0].m_type = GL_FLOAT;
        _desc[1].m_type = GL_FLOAT;
        _desc[2].m_type = GL_UNSIGNED_INT;
        _desc[3].m_type = GL_FLOAT;

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
        else if(_message == NetworkCommands::EntityCreate)
        {
            char _uuidBytes[16];

            _msg >> _uuidBytes;

            bool _exists = false;

            m_pECS->query<UUIDComponent>().each(
                [&_uuidBytes, &_exists](const UUIDComponent& _uuidComponent)
                {
                    char _uuidBytes2[16];

                    _uuidComponent.m_uuid.GetBytes(_uuidBytes2);

                    if(memcmp(_uuidBytes2, _uuidBytes, 16) == 0)
                    {
                        _exists = true;
                    }
                }
            );

            if(_exists)
            {
                return true;
            }

            m_pEntityFactory->Make(
                Utils::lambda(
                    [&_uuidBytes](Entity& _entity)
                    {
                        _entity.Add(UUIDComponent{ UUID(_uuidBytes) });

                        Graphics::TransformComponent _t;

                        _t.m_position = glm::vec3(0, 0, 0);
                        _t.m_scale = glm::vec3(1, 1, 1);
                        _t.m_rotation = glm::quat(0, 0, 0, 1);

                        _entity.Add(_t);
                    }
                )
            );

            if(m_owner == Network::IConnection::Owner::SERVER)
            {
                m_pServer->MessageAllClients(_message, _pClient);
            }

            return true;
        }
        else if(_message == NetworkCommands::EntityUpdate)
        {
            EntityUpdateKind _kind;
            char _uuidBytes[16];

            _msg >> _kind >> _uuidBytes;

            if(_kind == EntityUpdateKind::EntityUpdateKind_Transform)
            {
                glm::vec3 _transformPosition;
                glm::quat _transformRotation;
                glm::vec3 _transformScale;

                _msg >> _transformScale >> _transformRotation >> _transformPosition;

                m_selectQuery.each(
                    [&](flecs::entity _entity, const UUIDComponent& _uuidComponent, Graphics::TransformComponent& _transformComponent)
                    {
                        char _uuidBytes2[16];

                        _uuidComponent.m_uuid.GetBytes(_uuidBytes2);

                        if(memcmp(_uuidBytes2, _uuidBytes, 16) != 0)
                        {
                            return;
                        }

                        if(!_entity.is_valid() || !_entity.is_alive() || !_entity.has<Graphics::TransformComponent>())
                        {
                            return;
                        }

                        _transformComponent.m_position = _transformPosition;
                        _transformComponent.m_rotation = _transformRotation;
                        _transformComponent.m_scale = _transformScale;
                    }
                );
            }

            if(m_owner == Network::IConnection::Owner::SERVER)
            {
                m_pServer->MessageAllClients(_message, _pClient);
            }

            return true;
        }
        else if(_message == NetworkCommands::EntityDestroy)
        {
            char _uuidBytes[16];

            _msg >> _uuidBytes;

            Entity _entityToDisable;

            m_pECS->query<UUIDComponent>().each(
                [&_uuidBytes, &_entityToDisable, this](flecs::entity _entity, const UUIDComponent& _uuidComponent)
                {
                    char _uuidBytes2[16];

                    _uuidComponent.m_uuid.GetBytes(_uuidBytes2);

                    if(memcmp(_uuidBytes2, _uuidBytes, 16) == 0)
                    {
                        _entityToDisable = m_pEntityFactory->Clone(_entity);
                    }
                }
            );

            if(_entityToDisable.m_entity == m_selectedEntity.m_entity)
            {
                m_selectedEntity = Entity();
            }

            if(_entityToDisable.m_entity.is_valid())
            {
                _entityToDisable.m_entity.disable();
            }

            if(m_owner == Network::IConnection::Owner::SERVER)
            {
                m_pServer->MessageAllClients(_message, _pClient);
            }

            return true;
        }

        return false;
    }

    void ViewportWidget::OnEvent(const EntitySelectedEvent& _event)
    {
        m_selectedEntity = _event.m_entity;
    }

    void ViewportWidget::OnEvent(const EntityDestroyedEvent& _event)
    {
        char _bytes[16];

        Network::Message _message(NetworkCommands::EntityDestroy);

        m_selectedEntity.Get<UUIDComponent>().m_uuid.GetBytes(_bytes);

        _message << _bytes;

        if(m_pClient != nullptr)
        {
            m_pClient->Send(_message);
        }
        else if(m_pServer != nullptr)
        {
            m_pServer->MessageAllClients(_message);
        }

        m_selectedEntity = Entity();
    }

    void ViewportWidget::OnEvent(EntityCreatedEvent& _event)
    {
        if(!_event.m_entity.Has<UUIDComponent>())
        {
            return;
        }

        char _bytes[16];

        _event.m_entity.Get<UUIDComponent>().m_uuid.GetBytes(_bytes);

        Network::Message _message(NetworkCommands::EntityCreate);

        _message << _bytes;

        if(m_pClient != nullptr)
        {
            m_pClient->Send(_message);
        }
        else if(m_pServer != nullptr)
        {
            m_pServer->MessageAllClients(_message);
        }
    }

}}