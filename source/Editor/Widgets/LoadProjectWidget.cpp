#include "Editor/Widgets/LoadProjectWidget.h"

#include "imgui/imgui.h"

#include <filesystem>

// #include "ProjectManager/ProjectManager.h"
#include "ProjectManager/Events/LoadProjectEvent.h"

#include "Editor/Events/CloseWidgetEvent.h"

namespace Duckvil { namespace Editor {

    LoadProjectWidget::LoadProjectWidget()
    {

    }

    LoadProjectWidget::LoadProjectWidget(const Memory::FreeList& _heap, const Event::Pool<Event::mode::immediate>* _pEventPool, const Event::Pool<Event::mode::immediate>* _pEngineEventPool) :
        m_heap(_heap),
        m_pEditorEventPool(_pEventPool),
        m_pEngineEventPool(_pEngineEventPool)
    {
        m_iSelectedProject = -1;

        _heap.Allocate(m_aProjects, 1);

        for(const auto& _dir : std::filesystem::directory_iterator(DUCKVIL_PROJECTS_PATH))
        {
            if(m_aProjects.Full())
            {
                m_aProjects.Resize(m_aProjects.Size() * 2);
            }

            m_aProjects.Allocate(_dir.path().filename().string());
        }
    }

    LoadProjectWidget::~LoadProjectWidget()
    {
        m_heap.Free(m_aProjects);
    }

    void LoadProjectWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    bool items_getter(void* _pData, int _iIndex, const char** _ppOut)
    {
        Memory::Vector<Utils::string>* _items = static_cast<Memory::Vector<Utils::string>*>(_pData);

        *_ppOut = _items->At(_iIndex).m_sText;

        return true;
    }

    void LoadProjectWidget::OnDraw()
    {
        ImGui::Begin("LoadProject");

        ImGui::Combo("Projects", &m_iSelectedProject, &items_getter, &m_aProjects, m_aProjects.Size());

        if(ImGui::Button("Load") && m_iSelectedProject != -1)
        {
            // PlugNPlay::__module _module;

            // PlugNPlay::module_init(&_module);

            // PlugNPlay::__module_information _projectManagerModule("ProjectManager");
            // ProjectManager::ftable _projectManager;
            // ProjectManager::data _projectManagerData;
            // void (*_initProjectManager)(ProjectManager::ftable*);

            // _module.load(&_projectManagerModule);
            // _module.get(_projectManagerModule, "duckvil_project_manager_init", (void**)&_initProjectManager);

            // _initProjectManager(&_projectManager);

            // _projectManager.m_fnInitProjectManager(&_projectManagerData, m_heap);

            // ProjectManager::project _loadedProject = _projectManager.m_fnLoadProject(&_projectManagerData, m_aProjects[m_iSelectedProject], "F:/Projects/Duckvil/" + m_aProjects[m_iSelectedProject] + "/bin");

            auto _event = ProjectManager::LoadProjectEvent{ m_aProjects[m_iSelectedProject] };

            m_pEngineEventPool->Broadcast(_event);

            m_pEditorEventPool->Broadcast(CloseWidgetEvent(this));
        }

        ImGui::SameLine();

        if(ImGui::Button("Close"))
        {
            m_pEditorEventPool->Broadcast(CloseWidgetEvent(this));
        }

        ImGui::End();
    }

}}