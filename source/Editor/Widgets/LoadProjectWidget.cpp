#include "Editor/Widgets/LoadProjectWidget.h"

#include "imgui/imgui.h"

#include <filesystem>

#include "ProjectManager/ProjectManager.h"

#include "Editor/Events/CloseWidgetEvent.h"

namespace Duckvil { namespace Editor {

    LoadProjectWidget::LoadProjectWidget()
    {

    }

    LoadProjectWidget::LoadProjectWidget(const Memory::FreeList& _heap) :
        m_heap(_heap)
    {
        m_iSelectedProject = -1;

        _heap.Allocate(m_aProjects, 1);

        for(const auto& _dir : std::filesystem::directory_iterator("F:/Projects/Duckvil"))
        {
            if(m_aProjects.Full())
            {
                m_aProjects.Resize(m_aProjects.Size() * 2);
            }

            m_aProjects.Allocate(_dir.path().filename().string());
        }

        std::vector<int> a;

        a.push_back(1);
    }

    LoadProjectWidget::~LoadProjectWidget()
    {

    }

    void LoadProjectWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void LoadProjectWidget::OnDraw()
    {
        ImGui::Begin("LoadProject");

        ImGui::Combo("Projects", &m_iSelectedProject, &m_aProjects[0][0], m_aProjects.Size());

        if(ImGui::Button("Load") && m_iSelectedProject != -1)
        {
            PlugNPlay::__module _module;

            PlugNPlay::module_init(&_module);

            PlugNPlay::__module_information _projectManagerModule("ProjectManager");
            ProjectManager::ftable _projectManager;
            ProjectManager::data _projectManagerData;
            void (*_initProjectManager)(ProjectManager::ftable*);

            _module.load(&_projectManagerModule);
            _module.get(_projectManagerModule, "duckvil_project_manager_init", (void**)&_initProjectManager);

            _initProjectManager(&_projectManager);

            _projectManager.m_fnInitProjectManager(&_projectManagerData, m_heap);

            ProjectManager::project _loadedProject = _projectManager.m_fnLoadProject(&_projectManagerData, m_aProjects[m_iSelectedProject], "F:/Projects/Duckvil/" + m_aProjects[m_iSelectedProject] + "/bin");

            m_pEditorEventPool->Broadcast(CloseWidgetEvent(this));
        }

        ImGui::End();
    }

    void LoadProjectWidget::SetEditorEventPool(const Event::Pool<Event::mode::immediate>* _pEventPool)
    {
        m_pEditorEventPool = _pEventPool;
    }

}}