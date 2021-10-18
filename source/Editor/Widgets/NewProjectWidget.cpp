#include "Editor/Widgets/NewProjectWidget.h"

#include "imgui/imgui.h"

#include "Editor/Events/CloseWidgetEvent.h"

#include <fstream>

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <windows.h>
#endif
#include <map>

#include "ProjectManager/ProjectManager.h"

#include "Parser/Lexer.h"
#include "Parser/AST.h"

#include "Utils/AST.h"
#include "Utils/Utils.h"

#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"

#include "Engine/ReflectionFlags.h"

#include "ProjectManager/ProjectManager.h"

#undef max

namespace Duckvil { namespace Editor {

    const char* NewProjectWidget::ms_saTypes[] =
    {
        "Standalone",
        "Plugin",
        "Mod"
    };

    const uint32_t NewProjectWidget::ms_uiTypesCount = sizeof(NewProjectWidget::ms_saTypes) / sizeof(NewProjectWidget::ms_saTypes[0]);

    struct reflection_module
    {
        void* m_pObject;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        RuntimeReflection::__duckvil_resource_function_t m_generateCustomFunctionHandle;
        RuntimeReflection::__duckvil_resource_function_t m_processAST_FunctionHandle;
        RuntimeReflection::__duckvil_resource_function_t m_clearFunctionHandle;
    };

    struct user_data
    {
        const Memory::Vector<reflection_module>& m_aModules;

        explicit user_data(const Memory::Vector<reflection_module>& _aModules) :
            m_aModules(_aModules)
        {

        }
    };

    void generate_from_template(const std::string& _sTemplatePath, const std::string& _sOutputPath, const std::map<std::string, std::string>& _aParameters)
    {
        std::ifstream _tplFile(_sTemplatePath);
        std::ofstream _oFile(_sOutputPath);
        std::string _line;

        while(getline(_tplFile, _line))
        {
            for(const auto& _parameter : _aParameters)
            {
                _line = Utils::replace_all(_line, "{$" + _parameter.first + "}", _parameter.second);
            }

            _oFile << _line << "\n";
        }

        _oFile.close();
        _tplFile.close();
    }

    void generate_from_template(const std::map<std::string, std::string>& _aPaths, const std::map<std::string, std::string>& _aParameters)
    {
        for(const auto& _path : _aPaths)
        {
            generate_from_template(_path.first, _path.second, _aParameters);
        }
    }

#ifdef DUCKVIL_PLATFORM_WINDOWS
    bool RunMsBuild(const char* _sProgram, const char* _sCurrentDirectory, const char* args)
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

    void generate(std::ofstream& _file, void* _pUserData)
    {
        user_data* _userData = static_cast<user_data*>(_pUserData);

        _file << "#include \"Serializer/Runtime/ISerializer.h\"\n\n";
        _file << "#define DUCKVIL_GENERATED_BODY";

        for(auto& _module : _userData->m_aModules)
        {
            if(_module.m_pObject == nullptr)
            {
                continue;
            }

            Duckvil::RuntimeReflection::invoke_member<std::ofstream&>(RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData, _module.m_typeHandle, _module.m_generateCustomFunctionHandle, _module.m_pObject, _file);
        }
    }

    void generate_reflection(const Memory::FreeList& _heap, const PlugNPlay::__module& _module, const std::filesystem::path& _sPath)
    {
        Memory::Vector<reflection_module> _aModules;

        _heap.Allocate(_aModules, 1);

        Duckvil::PlugNPlay::__module_information _parser("Parser");
        PlugNPlay::__module_information _reflectionModule("RuntimeReflection");

        _module.load(&_parser);
        _module.load(&_reflectionModule);

        Parser::__lexer_ftable* (*_lexer_init)();
        RuntimeReflection::__generator_ftable* (*_runtime_reflection_generator)();
        Parser::__ast_ftable* (*_ast_init)();

        _module.get(_parser, "duckvil_lexer_init", reinterpret_cast<void**>(&_lexer_init));
        _module.get(_parser, "duckvil_ast_init", reinterpret_cast<void**>(&_ast_init));
        _module.get(_reflectionModule, "duckvil_runtime_reflection_generator_init", reinterpret_cast<void**>(&_runtime_reflection_generator));

        Parser::__ast_ftable* _ast = _ast_init();
        Parser::__lexer_ftable* _lexerFtable = _lexer_init();
        RuntimeReflection::__generator_ftable* _generatorFtable = _runtime_reflection_generator();

        std::filesystem::path _lastPath;
        uint32_t _index = 0;

        auto _types = RuntimeReflection::get_types(_heap);

        for(auto& _typeHandle : _types)
        {
            const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(_typeHandle, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule);

            if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() && (uint8_t)_variant.m_traits & (uint8_t)Duckvil::RuntimeReflection::property_traits::is_bool)
            {
                reflection_module _module = {};

                _module.m_pObject = Duckvil::RuntimeReflection::create<const Duckvil::Memory::FreeList&, Duckvil::RuntimeReflection::__ftable*, Duckvil::RuntimeReflection::__data*>(_heap, _typeHandle, false, _heap, RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData);
                _module.m_typeHandle = _typeHandle;
                _module.m_generateCustomFunctionHandle = Duckvil::RuntimeReflection::get_function_handle<std::ofstream&>(RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData, _typeHandle, "GenerateCustom");
                _module.m_clearFunctionHandle = Duckvil::RuntimeReflection::get_function_handle(RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData, _typeHandle, "Clear");
                _module.m_processAST_FunctionHandle = Duckvil::RuntimeReflection::get_function_handle<Duckvil::Parser::__ast*>(RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData, _typeHandle, "ProcessAST");

                if(_aModules.Full())
                {
                    _aModules.Resize(_aModules.Size() * 2);
                }

                _aModules.Allocate(_module);

                // Duckvil::RuntimeReflection::invoke_member<std::ofstream&>(_runtimeReflectionData, _typeHandle, _function, _testSystem);
            }
        }

        for(auto& _path : std::filesystem::recursive_directory_iterator(_sPath / "include"))
        {
            if(_path.path().extension() != ".h")
            {
                continue;
            }

            Parser::__lexer_data _data;

            _lexerFtable->load_file(&_data, _path.path().string().c_str());

            Parser::__ast _astData;

            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_EXPORT", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "slot", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_RESOURCE_DECLARE", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_GENERATED_BODY", &Utils::user_define_behavior });
            _astData.m_aUserDefines.push_back(Parser::user_define{ "DUCKVIL_RESOURCE", &Utils::user_define_resource_behavior });

#ifdef DUCKVIL_HOT_RELOADING
                _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_HOT_RELOADING", &Duckvil::Utils::user_define_behavior });
#endif

            _ast->ast_generate(&_astData, _lexerFtable, _data);

            RuntimeReflection::__generator_data _generatorData;
            std::filesystem::path _relativePath = std::filesystem::relative(_path.path(), _sPath / "include");
            std::filesystem::path _pluginDirectory = _relativePath;

            while(_pluginDirectory.has_parent_path())
            {
                _pluginDirectory = _pluginDirectory.parent_path();
            }

            if(_lastPath == "")
            {
                _lastPath = _pluginDirectory;
            }

            if(_lastPath != _pluginDirectory)
            {
                std::ofstream _file(_sPath / "__generated_reflection__" / _lastPath / "plugin_info.cpp");

                _file << "#include \"RuntimeReflection/Recorder.h\"\n";
                _file << "#include \"Logger/Logger.h\"\n";
                _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(" << _index << ")\n\n";
                _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_runtime_reflection_context(const duckvil_frontend_reflection_context& _runtimeReflectionContext)\n";
                _file << "{\n";
                _file << "Duckvil::RuntimeReflection::make_current(_runtimeReflectionContext);\n";
                _file << "}\n\n";
                _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_logger_context(const Duckvil::logger_context& _loggerContext)\n";
                _file << "{\n";
                _file << "Duckvil::logger_make_current(_loggerContext);\n";
                _file << "}";

                _file.close();

                _index = 0;
                _lastPath = _pluginDirectory;
            }

            if(_relativePath.string().size() < DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX)
            {
                strcpy(_generatorData.m_sInclude, _relativePath.string().c_str());
            }
            else
            {
                assert(false && "Path is too long!");
            }

            _generatorData.m_uiRecorderIndex = _index++;

            std::filesystem::path _generatePath = _sPath / "__generated_reflection__" / _relativePath;

            if(!std::filesystem::exists(_generatePath.parent_path()))
            {
                std::filesystem::create_directories(_generatePath.parent_path());
            }

            std::filesystem::path _source = _generatePath;
            std::filesystem::path _header = _generatePath;

            _source.replace_extension(".generated.cpp");
            _header.replace_extension(".generated.h");

            // strcpy(_generatorData.m_sGeneratedHeader, std::filesystem::relative(_header, std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__").string().c_str());

            user_data _userData(_aModules);

            _generatorFtable->generate(&_generatorData, _source.string().c_str(), _header.string().c_str(), _astData, &generate, &_userData);
        }

        std::ofstream _file;

        if(!_lastPath.has_extension())
        {
            _file.open(_sPath / "__generated_reflection__" / _lastPath / "plugin_info.cpp");
        }
        else
        {
            _file.open(_sPath / "__generated_reflection__" / "plugin_info.cpp");
        }

        _file << "#include \"RuntimeReflection/Recorder.h\"\n";
        _file << "#include \"Logger/Logger.h\"\n";
        _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(" << _index << ")\n\n";
        _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_runtime_reflection_context(const duckvil_frontend_reflection_context& _runtimeReflectionContext)\n";
        _file << "{\n";
        _file << "Duckvil::RuntimeReflection::make_current(_runtimeReflectionContext);\n";
        _file << "}\n\n";
        _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_logger_context(const Duckvil::logger_context& _loggerContext)\n";
        _file << "{\n";
        _file << "Duckvil::logger_make_current(_loggerContext);\n";
        _file << "}";

        _file.close();
    }

    NewProjectWidget::NewProjectWidget()
    {

    }

    NewProjectWidget::NewProjectWidget(const Memory::FreeList& _heap) :
        m_heap(_heap)
    {
        m_sProjectName = _heap.Allocate<char>(32);
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
            std::string _projectPath("F:/Projects/Duckvil/");

            _projectPath.append(m_sProjectName);

            std::filesystem::create_directory(_projectPath);
            std::filesystem::create_directory(_projectPath + "/source");
            std::filesystem::create_directory(_projectPath + "/include");
            std::filesystem::create_directory(_projectPath + "/build");
            std::filesystem::create_directory(_projectPath + "/__generated_reflection__");

            {
                std::ofstream _file(_projectPath + "/config.json");

                _file.close();
            }

            {
                std::ofstream _file(_projectPath + "/CMakeLists.txt");

                _file << "cmake_minimum_required(VERSION 3.16)\n";
                _file << "project(" << m_sProjectName << ")\n";
                _file << "add_definitions(-DDUCKVIL_OUTPUT=\"" << _projectPath << "/bin" << "\" -DDUCKVIL_PLATFORM_WINDOWS)\n";
                _file << "add_subdirectory(source)";

                _file.close();
            }

            {
                std::ofstream _file(_projectPath + "/source/CMakeLists.txt");

                _file << "cmake_minimum_required(VERSION 3.16)\n";
                _file << "FILE(GLOB GENERATED_SRC_FILES ${CMAKE_SOURCE_DIR}/__generated_reflection__/*.cpp)\n";
                _file << "add_library(" << m_sProjectName << " SHARED Project.cpp ${GENERATED_SRC_FILES})\n";
                _file << "target_include_directories(" << m_sProjectName << " PRIVATE ${CMAKE_SOURCE_DIR}/include F:/Projects/C++/Duckvil/include F:/Projects/C++/Duckvil/__generated_reflection__ F:/Projects/C++/Duckvil/external)\n";
                _file << "target_link_libraries(" << m_sProjectName << " F:/Projects/C++/Duckvil/bin/Window.lib)\n";
                _file << "set_target_properties(" << m_sProjectName << " PROPERTIES\n"
                "CXX_STANDARD 20\n"
                "LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin\n"
                "ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin\n"
                "RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin\n"
                "LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin\n"
                "ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin\n"
                "RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin\n"
                "LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin\n"
                "ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin\n"
                "RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin\n"
                "DEBUG_POSTFIX \"\"\n"
                "PREFIX \"\"\n)";

                _file.close();
            }

            generate_from_template(
                {
                    { "F:/Projects/C++/Duckvil/resource/template/new-project.tpl.h", _projectPath + "/include/Project.h" },
                    { "F:/Projects/C++/Duckvil/resource/template/new-project.tpl.cpp", _projectPath + "/source/Project.cpp" }
                },
                {
                    { "projectName", m_sProjectName }
                }
            );

            PlugNPlay::__module _module;

            PlugNPlay::module_init(&_module);

            generate_reflection(m_heap, _module, _projectPath);

#ifdef DUCKVIL_PLATFORM_WINDOWS
            RunMsBuild(
                "CMake",
                (_projectPath + "/build").c_str(),
                ".."
            );

            RunMsBuild(
                "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin/MSBuild.exe",
                (_projectPath + "/build").c_str(),
                (_projectPath + "/build/" + m_sProjectName + ".sln /t:Build").c_str()
            );
#endif

            PlugNPlay::__module_information _projectManagerModule("ProjectManager");
            ProjectManager::ftable _projectManager;
            ProjectManager::data _projectManagerData;
            void (*_initProjectManager)(ProjectManager::ftable*);

            _module.load(&_projectManagerModule);
            _module.get(_projectManagerModule, "duckvil_project_manager_init", reinterpret_cast<void**>(&_initProjectManager));

            _initProjectManager(&_projectManager);

            _projectManager.m_fnInitProjectManager(&_projectManagerData, m_heap);

            ProjectManager::project _loadedProject = _projectManager.m_fnLoadProject(&_projectManagerData, Utils::string(m_sProjectName, strlen(m_sProjectName) + 1), _projectPath + "/bin");

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
        printf("aaaa");
    }

    void NewProjectWidget::SetEditorEventPool(const Event::Pool<Event::mode::immediate>* _pEventPool)
    {
        m_pEditorEventPool = _pEventPool;
    }

}}