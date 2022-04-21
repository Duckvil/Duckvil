#include "Editor/Widgets/NewProjectWidget.h"

#include "imgui/imgui.h"

#include "Editor/Events/CloseWidgetEvent.h"

#include "ProjectManager/ProjectManager.h"

namespace Duckvil { namespace Editor {

    const char* NewProjectWidget::ms_saTypes[] =
    {
        "Standalone",
        "Plugin",
        "Mod"
    };

    const uint32_t NewProjectWidget::ms_uiTypesCount = sizeof(NewProjectWidget::ms_saTypes) / sizeof(NewProjectWidget::ms_saTypes[0]);

    NewProjectWidget::NewProjectWidget()
    {

    }

    NewProjectWidget::NewProjectWidget(const Memory::FreeList& _heap, const Event::Pool<Event::mode::immediate>* _pEditorEventPool, ProjectManager::ftable* _pProjectManager, ProjectManager::data* _pProjectManagerData) :
        m_heap(_heap),
        m_pEditorEventPool(_pEditorEventPool),
        m_pProjectManager(_pProjectManager),
        m_pProjectManagerData(_pProjectManagerData)
    {
        m_sProjectName = _heap.AllocateArray<char>(32);
        memset(m_sProjectName, 0, 32);
        m_iSelectedType = -1;
    }

    NewProjectWidget::~NewProjectWidget()
    {
        m_heap.Free(m_sProjectName);
    }

    void NewProjectWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    void NewProjectWidget::OnDraw()
    {
        ImGui::Begin("NewProject");

        ImGui::InputText("Project name", m_sProjectName, 32);
        ImGui::Combo("Type", &m_iSelectedType, ms_saTypes, ms_uiTypesCount);

        if(ImGui::Button("Create") && m_iSelectedType > -1 && strcmp(m_sProjectName, "") != 0)
        {
            Utils::string _projectPath(DUCKVIL_PROJECTS_PATH);

            if(m_pProjectManager->m_fnCreateProject(m_pProjectManagerData, m_sProjectName, _projectPath))
            {
                _projectPath = _projectPath / m_sProjectName;

                ProjectManager::project _loadedProject = m_pProjectManager->m_fnLoadProject(m_pProjectManagerData, Utils::string(m_sProjectName, strlen(m_sProjectName) + 1), _projectPath / "bin");
            }

            m_pEditorEventPool->Broadcast(CloseWidgetEvent(this));
        }

        ImGui::SameLine();

        if(ImGui::Button("Close"))
        {
            m_pEditorEventPool->Broadcast(CloseWidgetEvent(this));
        }

        ImGui::End();
    }

    bool NewProjectWidget::Init()
    {
        return true;
    }

    void NewProjectWidget::Update(double _dDelta)
    {
        // printf("aaaa");
    }

    // void NewProjectWidget::SetEditorEventPool(const Event::Pool<Event::mode::immediate>* _pEventPool)
    // {
    //     m_pEditorEventPool = _pEventPool;
    // }

}}