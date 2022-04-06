#include "Editor/Widgets/ContentExplorerWidget.h"

#include "imgui/imgui.h"

#include "ProjectManager/Events/OnLoadEvent.h"

namespace Duckvil { namespace Editor {

    ContentExplorerWidget::ContentExplorerWidget()
    {
        m_bCreateDirectoryPopup = false;
        m_bLoaded = false;
    }

    ContentExplorerWidget::ContentExplorerWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool) :
        m_pEngineEventPool(_pEngineEventPool)
    {
        m_bLoaded = false;
        m_bCreateDirectoryPopup = false;

        _pEngineEventPool->AddA<ProjectManager::OnLoadEvent>([&](const ProjectManager::OnLoadEvent& _event)
        {
            m_sPath = _event.m_project.m_sPath;
            m_bLoaded = true;
        });
    }

    ContentExplorerWidget::~ContentExplorerWidget()
    {

    }

    void ContentExplorerWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    void ContentExplorerWidget::OnDraw()
    {
        ImGui::Begin("ContentExplorer");

        if(m_bLoaded)
        {
            ImGui::Text("Current directory: ");
            const auto& _splitted = Utils::split(m_sPath.m_sText, '/');
            Utils::string _res(1);

            for(uint32_t i = 0; i < _splitted.size(); ++i)
            {
                const auto& _item = _splitted[i];
                if(i < _splitted.size() - 1)
                {
                    ImGui::Text("%s/", _item.c_str());
                    ImGui::SameLine(0, 1);
                }
                else
                {
                    ImGui::Text("%s", _item.c_str());
                }

                _res = _res + Utils::string(_item) + "/";

                if(ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    m_sPath = _res;
                }
            }

            bool _isAnyItemHovered = false;

            for(const auto& _it: std::filesystem::directory_iterator(m_sPath.m_sText))
            {
                ImGui::Text("%s", _it.path().filename().string().c_str());

                if(_it.is_directory())
                {
                    if(ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                    {
                        m_sPath = Utils::replace_all(_it.path().string(), "\\", "/");
                    }

                    if(ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                    {
                        m_sSelectedPath = _it.path();

                        ImGui::OpenPopup("##content_browser_file_context_popup");
                    }
                }
                else
                {
                    if(ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                    {
                        ImGui::OpenPopup("##content_browser_file_context_popup");
                    }
                }

                if(ImGui::IsItemHovered())
                {
                    _isAnyItemHovered = true;
                }
            }

            if(ImGui::BeginPopup("##content_browser_file_context_popup"))
            {
                bool _close = false;

                if(ImGui::Button("Delete"))
                {
                    // printf("%s\n", m_sSelectedPath.m_sText);
                    std::filesystem::remove(m_sSelectedPath.m_sText);

                    m_sSelectedPath = "";

                    ImGui::CloseCurrentPopup();
                }

                if(ImGui::Button("Rename"))
                {
                    ImGui::OpenPopup("##content_browser_file_context_rename_popup");
                }

                if(ImGui::BeginPopup("##content_browser_file_context_rename_popup"))
                {
                    static char _newName[32] = { 0 };

                    ImGui::InputText("New name", _newName, 32);

                    if(ImGui::Button("OK"))
                    {
                        std::filesystem::rename(m_sSelectedPath.m_sText, (Utils::string::parentPath(m_sSelectedPath) + "/" + _newName).m_sText);

                        _close = true;

                        ImGui::CloseCurrentPopup();
                    }

                    if(ImGui::Button("Close"))
                    {
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }

                if(_close)
                {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if(!_isAnyItemHovered && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
            {
                // m_sPath = Utils::string::parentPath(m_sPath);

                ImGui::OpenPopup("##content_browser_popup");
            }

            if(ImGui::BeginPopup("##content_browser_popup"))
            {
                bool _close = false;
                static bool _createClassModalOpen = false;

                if(ImGui::Button("Create class"))
                {
                    _createClassModalOpen = true;

                    ImGui::OpenPopup("##content_browser_create_class_popup");
                }

                if(ImGui::Button("Create directory"))
                {
                    ImGui::OpenPopup("##content_browser_create_directory_popup");
                }

                if(ImGui::BeginPopup("##content_browser_create_directory_popup"))
                {
                    static char _name[32] = { 0 };

                    ImGui::InputText("Name", _name, 32);

                    if(ImGui::Button("Create"))
                    {
                        std::filesystem::create_directory((m_sPath + "/" + _name).m_sText);

                        memset(_name, 0, 32);

                        ImGui::CloseCurrentPopup();

                        _close = true;
                    }

                    ImGui::SameLine();

                    if(ImGui::Button("Close"))
                    {
                        memset(_name, 0, 32);

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }

                if(ImGui::BeginPopupModal("##content_browser_create_class_popup", &_createClassModalOpen))
                {
                    static char _className[32] = { 0 };
                    static const char* _currentItem = nullptr;
                    static const char* _systems[] = { "Widget", "System" };

                    ImGui::InputText("Class name", _className, 32);

                    // if(ImGui::BeginCombo("Systems", _currentItem))
                    // {
                    //     for(const auto& _item : _systems)
                    //     {
                    //         bool _isSelected = _currentItem == _item;

                    //         if(ImGui::Selectable(_item, _isSelected))
                    //         {
                    //             _currentItem = _item;
                    //         }

                    //         if(_isSelected)
                    //         {
                    //             ImGui::SetItemDefaultFocus();
                    //         }
                    //     }

                    //     ImGui::EndCombo();
                    // }

                    if(ImGui::TreeNode("Selection State: Multiple Selection"))
                    {
                        static bool selection[] = { false, false, false, false, false };

                        for(int n = 0; n < 5; n++)
                        {
                            char buf[32];
                            sprintf(buf, "Object %d", n);

                            if(ImGui::Selectable(buf, selection[n]))
                            {
                                if(!ImGui::GetIO().KeyCtrl)
                                {
                                    memset(selection, 0, sizeof(selection));
                                }

                                selection[n] ^= 1;
                            }
                        }
                        ImGui::TreePop();
                    }

                    if(ImGui::Button("Create"))
                    {

                    }

                    ImGui::EndPopup();
                }

                if(_close)
                {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }

}}