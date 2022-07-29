#include "Editor/Widgets/ContentExplorerWidget.h"

#include "imgui/imgui.h"

#include "ProjectManager/Events/OnLoadEvent.h"

#include "Utils/TemplateEngine.h"

#include <fstream>
#include <map>
#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace Duckvil { namespace Editor {

#ifdef DUCKVIL_PLATFORM_WINDOWS
    bool execute_command(const char* _sProgram, const char* _sCurrentDirectory, const char* args)
    {
        STARTUPINFOA startupInfo;
        PROCESS_INFORMATION procInfo;

        memset(&startupInfo, 0, sizeof(startupInfo));
        memset(&procInfo, 0, sizeof(procInfo));

        std::string cmdLine(_sProgram);

        cmdLine.append(" ") += args;

        if(!CreateProcessA(0, const_cast<char*>(cmdLine.c_str()), 0, 0, FALSE, 0, 0, _sCurrentDirectory, &startupInfo, &procInfo))
        {
            return false;
        }

        WaitForSingleObject(procInfo.hProcess, INFINITE);
        DWORD dwExitCode;
        GetExitCodeProcess(procInfo.hProcess, &dwExitCode);
        CloseHandle(procInfo.hProcess);
        CloseHandle(procInfo.hThread);

        return dwExitCode == 0;
    }
#endif

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
            m_sProjectPath = m_sPath;
            m_bLoaded = true;
            m_sProjectName = std::filesystem::path(_event.m_project.m_sPath.m_sText).filename();
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
                        std::filesystem::path _parentPath = std::filesystem::path((m_sPath / _name).m_sText).parent_path();

                        if(std::filesystem::exists(_parentPath / "CmakeLists.txt"))
                        {
                            std::vector<std::string> _lines;

                            {
                                std::ifstream _cmakeFile(_parentPath / "CMakeLists.txt");
                                std::string _line;

                                while(getline(_cmakeFile, _line))
                                {
                                    if(_line == "# END SUBDIRECTORIES")
                                    {
                                        _lines.push_back("add_subdirectory(" + std::string(_name) + ")");
                                    }

                                    _lines.push_back(_line);
                                }

                                _cmakeFile.close();
                            }

                            {
                                std::ofstream _cmakeFile(_parentPath / "CMakeLists.txt", std::ofstream::trunc);

                                for(const std::string& _line : _lines)
                                {
                                    _cmakeFile << _line << "\n";
                                }

                                _cmakeFile.close();
                            }
                        }
                        else
                        {
                            Utils::string _cwd = DUCKVIL_CWD;

                            TemplateEngine::generate(
                                {
                                    { _cwd / "resource/template/project-subdirectory-cmake.tpl.txt", _parentPath / "CMakeLists.txt" }
                                },
                                {
                                    { "subdirectory", _name },
                                    { "projectName", m_sProjectName }
                                }
                            );
                        }

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
                    static char _fileName[32] = { 0 };
                    static bool _selection[] = { false, false };
                    static const char* _systems[] =
                    {
                        "Duckvil::Editor::Widget",
                        "Duckvil::Project::Script"
                    };

                    ImGui::InputText("Class name", _className, 32);
                    ImGui::InputText("File name", _fileName, 32);

                    if(ImGui::TreeNode("Inheritance"))
                    {
                        for(int n = 0; n < sizeof(_selection) / sizeof(_selection[0]); n++)
                        {
                            char _buf[32];

                            snprintf(_buf, sizeof(_buf), "%s", _systems[n]);

                            if(ImGui::Selectable(_buf, _selection[n], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
                            {
                                if(!ImGui::GetIO().KeyCtrl)
                                {
                                    memset(_selection, 0, sizeof(_selection));
                                }

                                _selection[n] ^= 1;
                            }
                        }

                        ImGui::TreePop();
                    }

                    if(ImGui::Button("Create"))
                    {
                        std::string _inheritance;
                        Utils::string _cwd = DUCKVIL_CWD;
                        Utils::string _iFileName(strcmp(_fileName, "") != 0 ? _fileName : _className);

                        std::string _rModulePath = std::filesystem::relative(m_sPath.m_sText, m_sProjectPath.m_sText).string();
                        std::string::iterator _rModulePathIt = _rModulePath.begin();

                        while(*_rModulePathIt != '\\')
                        {
                            _rModulePathIt++;
                        }

                        _rModulePathIt++;

                        _rModulePath = std::string(_rModulePathIt, _rModulePath.end());

                        _rModulePath = Utils::replace_all(_rModulePath, "\\", "/");

                        std::filesystem::create_directories((m_sProjectPath / "source" / _rModulePath).m_sText);
                        std::filesystem::create_directories((m_sProjectPath / "include" / _rModulePath).m_sText);

                        bool _anySelection = false;

                        for(int n = 0; n < sizeof(_selection) / sizeof(_selection[0]); n++)
                        {
                            if(_selection[n])
                            {
                                _anySelection = true;

                                break;
                            }
                        }

                        if(_anySelection)
                        {
                            _inheritance = " : ";

                            for(int n = 0; n < sizeof(_selection) / sizeof(_selection[0]); n++)
                            {
                                if(_selection[n])
                                {
                                    _inheritance += "public ";
                                    _inheritance += _systems[n];
                                }

                                if(n < sizeof(_selection) / sizeof(_selection[0]) - 1)
                                {
                                    _inheritance += ", ";
                                }
                            }
                        }

                        TemplateEngine::generate(
                            {
                                { _cwd / "resource/template/project/new-project-script.tpl.h", m_sProjectPath / "include" / _rModulePath / _iFileName + ".h" },
                                { _cwd / "resource/template/project/new-project-script.tpl.cpp", m_sProjectPath / "source" / _rModulePath / _iFileName + ".cpp" },
                            },
                            {
                                { "projectName", m_sProjectName },
                                { "scriptName", _iFileName },
                                { "inheritance", _inheritance },
                                { "headerFile", _rModulePath + "/" + _iFileName }
                            }
                        );

                        if(std::filesystem::exists((m_sProjectPath / "source" / _rModulePath / "CMakeLists.txt").m_sText))
                        {
                            std::vector<std::string> _lines;

                            {
                                std::ifstream _cmakeFile((m_sProjectPath / "source" / _rModulePath / "CMakeLists.txt").m_sText);
                                std::string _line;

                                while(getline(_cmakeFile, _line))
                                {
                                    if(_line == "# END FILES")
                                    {
                                        _lines.push_back(("\"" + _iFileName + ".cpp\"").m_sText);
                                        _lines.push_back(("\"${CMAKE_SOURCE_DIR}/__generated_reflection__/" + _rModulePath + "/" + _iFileName + ".generated.cpp\"").m_sText);
                                    }

                                    _lines.push_back(_line);
                                }

                                _cmakeFile.close();
                            }

                            {
                                std::ofstream _cmakeFile((m_sProjectPath / "source" / _rModulePath / "CMakeLists.txt").m_sText, std::ofstream::trunc);

                                for(const std::string& _line : _lines)
                                {
                                    _cmakeFile << _line << "\n";
                                }

                                _cmakeFile.close();
                            }
                        }
                        else
                        {
                            Utils::string _cwd = DUCKVIL_CWD;

                            TemplateEngine::generate(
                                {
                                    { _cwd / "resource/template/project-subdirectory-cmake.tpl.txt", (m_sProjectPath / "source" / _rModulePath / "CMakeLists.txt").m_sText }
                                },
                                {
                                    { "file", _iFileName + ".cpp\"\n\"${CMAKE_SOURCE_DIR}/__generated_reflection__/" + _rModulePath + "/" + _iFileName + ".generated.cpp" },
                                    { "projectName", m_sProjectName }
                                }
                            );
                        }

                        execute_command("CMake", (m_sProjectPath / "build").m_sText, "..");

                        _close = true;
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