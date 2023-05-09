#include "Editor/Widgets/SceneHierarchyWidget.h"

#include "imgui/imgui.h"

#include "Utils/ECS.h"

#include "Editor/Events/EntitySelectedEvent.h"

#include "Engine/Events/EntityDestroyedEvent.h"

#include "ECS/EntityFactory.h"

#include "Graphics/TransformComponent.h"

namespace Duckvil { namespace Editor {

    SceneHierarchyWidget::SceneHierarchyWidget(const Memory::FreeList& _heap, flecs::world* _pECS, Event::Pool<Event::mode::immediate>* _pEditorEventPool, ECS::EntityFactory* _pEntityFactory) :
        m_pECS(_pECS),
        m_pEditorEventPool(_pEditorEventPool),
        m_pEntityFactory(_pEntityFactory)
    {
        Utils::init_ecs();

        m_entityQuery = m_pECS->query<UUIDComponent>();

        _pEditorEventPool->Add<EntitySelectedEvent>(this, &SceneHierarchyWidget::OnEvent);
    }

    SceneHierarchyWidget::~SceneHierarchyWidget()
    {

    }

    void SceneHierarchyWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    void SceneHierarchyWidget::OnDraw()
    {
        ImGui::Begin("Scene Hierarchy");

        if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("##SceneHierarchyContextMenu");
        }

        if(ImGui::BeginPopup("##SceneHierarchyContextMenu"))
        {
            if(ImGui::MenuItem("Create Entity"))
            {
                ECS::Entity _e = m_pEntityFactory->Make(
                    [](ECS::Entity& _entity)
                    {
                        _entity.Add<UUIDComponent>();

                        Graphics::TransformComponent _t;

                        _t.m_position = glm::vec3(0, 0, 0);
                        _t.m_scale = glm::vec3(1, 1, 1);
                        _t.m_rotation = glm::quat(0, 0, 0, 1);

                        _entity.Add(_t);
                    }
                );
            }

            ImGui::EndPopup();
        }

        m_entityQuery.each(
            [this](flecs::entity _e, const UUIDComponent& _uuid)
            {
                size_t hash = _uuid.m_uuid.Hash();

                if(ImGui::Selectable(std::to_string(_uuid.m_uuid.Hash()).c_str(), m_selectedEntity.m_entity == _e))
                {
                    m_pEditorEventPool->Broadcast(EntitySelectedEvent{ ECS::EntityFactory::Clone(_e) });
                }

                if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    // ImGui::SetTooltip("%s", _uuid.m_uuid.ToString().c_str());

                    ImGui::OpenPopup("##EntityContextMenu");
                }
            }
        );

        if(ImGui::BeginPopup("##EntityContextMenu"))
        {
            if(m_pEntityFactory->IsValid(m_selectedEntity) && ImGui::MenuItem("Delete"))
            {
                m_pEditorEventPool->Broadcast(EntityDestroyedEvent{ m_selectedEntity });

                m_selectedEntity.m_entity.disable();

                m_selectedEntity = ECS::Entity();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void SceneHierarchyWidget::OnEvent(const EntitySelectedEvent& _event)
    {
        m_selectedEntity = _event.m_entity;
    }

}}