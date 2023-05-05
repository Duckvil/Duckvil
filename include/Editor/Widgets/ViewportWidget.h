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

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "Network/NetworkSystem.h"
#include "Network/IConnection.h"
#include "Network/IClient.h"
#include "Network/IServer.h"

#include "Engine/UUIDComponent.h"
#include "Engine/EntityFactory.h"

#include "Editor/Events/EntitySelectedEvent.h"
#include "Engine/Events/EntityDestroyedEvent.h"

#undef GetMessage
#undef min

#include "Editor/Widgets/ViewportWidget.generated.h"

namespace Duckvil { namespace Editor {

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_UserSystem)
    class ViewportWidget : public Widget, public ISystem, public Network::NetworkSystem
    {
        DUCKVIL_GENERATED_BODY
    public:
        enum NetworkCommands
        {
            PlayerMovement,
            AddPlayer,
            EntityCreate,
            EntityDestroy,
            EntityUpdate
        };

        enum EntityUpdateKind
        {
            EntityUpdateKind_Transform
        };

        struct NetworkComponent
        {
            uint32_t m_uiID;
        };

    private:
        Memory::FreeList m_heap;

        Graphics::Renderer::renderer_ftable* m_pRenderer;
        Graphics::Renderer::renderer_data* m_pRendererData;

        ImVec2 m_oldSize;

        Event::Pool<Event::mode::buffered>* m_pWindowEventPool;
        Event::Pool<Event::mode::immediate>* m_pEditorEventPool;
        Event::Pool<Event::mode::immediate>* m_pProjectEventPool;

        viewport m_viewport;

        bool m_aKeys[Window::key_size];
        //bool m_bIsWindowFocused = false;

        bool m_bSkip = false;

        flecs::world* m_pECS;
        EntityFactory* m_pEntityFactory;

        Network::IConnection::Owner m_owner;

        Network::IServer* m_pServer;
        Network::IClient* m_pClient;

        flecs::query<ViewportWidget::NetworkComponent, Graphics::TransformComponent> m_networkQuery;
        flecs::query<UUIDComponent, Graphics::TransformComponent> m_selectQuery;

        Entity m_selectedEntity;

        glm::vec2 m_viewportBounds[2];

        bool m_bLookingAround;
        double m_dDelta = 0;

#ifndef DUCKVIL_TRACY_EDITOR
        uint32_t m_uiReaderHandle;
#endif

        bool m_bSimulating;
        bool m_bPaused;

        void SpwanPlayer(uint32_t _uiID);

    public:
        ViewportWidget();
        ViewportWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Window") Event::Pool<Event::mode::buffered>* _pWindowEventPool, Network::IServer* _pServer, Network::IClient* _pClient, DUCKVIL_ARGUMENT("Editor") Event::Pool<Event::mode::immediate>* _pEditorEventPool, flecs::world* _pECS, EntityFactory* _pEntityFactory, DUCKVIL_ARGUMENT("Project") Event::Pool<Event::mode::immediate>* _pProjectEventPool);
        ~ViewportWidget();

        bool Init();
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void SetRenderer(Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData);
        void SetECS(flecs::world* _pECS);

        void SetOwner(Duckvil::Network::IConnection::Owner _owner) override;

        bool OnMessage(const Duckvil::Network::Message& _message, std::shared_ptr<Duckvil::Network::IConnection> _pClient) override;

        void OnEvent(const EntitySelectedEvent& _event);
        void OnEvent(const EntityDestroyedEvent& _event);
        void OnEvent(EntityCreatedEvent& _event);
    };

}}