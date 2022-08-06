#include "Editor/Widgets/SceneHierarchyWidget.h"

#include "imgui/imgui.h"

#include "Utils/ECS.h"

#include "Editor/Events/EntitySelectedEvent.h"

#include "Engine/EntityFactory.h"

namespace Duckvil { namespace Editor {

    SceneHierarchyWidget::SceneHierarchyWidget(const Memory::FreeList& _heap, flecs::world* _pECS, const Event::Pool<Event::mode::immediate>* _pEditorEventPool) :
        m_pECS(_pECS),
        m_pEditorEventPool(_pEditorEventPool)
    {
        Utils::init_ecs();

        m_entityQuery = m_pECS->query<UUIDComponent>();
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

        static size_t _selectedEntity = -1;

        m_entityQuery.each(
            [this](flecs::entity _e, const UUIDComponent& _uuid)
            {
                if(ImGui::Selectable(std::to_string(_uuid.m_uuid.Hash()).c_str(), _selectedEntity == _uuid.m_uuid.Hash()))
                {
                    _selectedEntity = _uuid.m_uuid.Hash();

                    m_pEditorEventPool->Broadcast(EntitySelectedEvent{ EntityFactory::Clone(_e) });
                }
            }
        );

        ImGui::End();
    }

}}