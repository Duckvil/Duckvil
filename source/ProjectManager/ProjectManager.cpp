#include "ProjectManager/ProjectManager.h"

#include "PlugNPlay/Module.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Meta.h"

#include "Logger/Logger.h"

#include "Serializer/Runtime/Serializer.h"

#include "ProjectManager/Events/OnLoadEvent.h"
#include "ProjectManager/Events/AddScriptEvent.h"

#include "Utils/AST.h"
#include "Utils/md5.h"
#include "Utils/TemplateEngine.h"

#include "json/single_include/nlohmann/json.hpp"

#include "Engine/EntityFactory.h"

#include "Engine/Events/RequestSystemEvent.h"

#undef GetObject
#undef max

namespace Duckvil { namespace ProjectManager {

    std::string load_file_as_string(const Utils::string& _sFile)
    {
        std::ifstream _f(_sFile);

        std::string _str = std::string((std::istreambuf_iterator<char>(_f)), std::istreambuf_iterator<char>());

        _f.close();

        return _str;
    }

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

    /*void action(const std::filesystem::path& _file, HotReloader::FileWatcher::FileStatus _status, void* _pUserData)
    {
        data* _data = static_cast<data*>(_pUserData);

        if(_status == HotReloader::FileWatcher::FileStatus::FileStatus_Modified && _file.extension() == ".cpp")
        {
            RuntimeCompiler::Options _options;

            _options.m_aIncludes.push_back((_data->m_loadedProject.m_sPath / "include").m_sText);
            _options.m_aIncludes.push_back((_data->m_loadedProject.m_sPath / "__generated_reflection__").m_sText);

            _data->m_fnCompile->Invoke(_data->m_pRCS, _data->m_loadedProject.m_sPath.m_sText, _file.string(), Utils::lambda([_data](Memory::Vector<HotReloader::RuntimeCompilerSystem::hot_object>* _pHotObjects, duckvil_recorderd_types& _newTypes)
            {
                bool _found = false;

                for(uint32_t i = 0; i < _pHotObjects->Size(); ++i)
                {
                    HotReloader::RuntimeCompilerSystem::hot_object& _hot = _pHotObjects->At(i);

                    for(size_t j = 0; j < _newTypes.m_ullCount; ++j)
                    {
                        const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _newTypes.m_aTypes[j];

                        if(_hot.m_pObject->GetTypeHandle().m_ID == _type.m_ID)
                        {
                            _found = true;

                            const auto& _rcsTypeHandle = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>();

                            RuntimeReflection::invoke_member<HotReloader::RuntimeCompilerSystem::hot_object*, const RuntimeReflection::__duckvil_resource_type_t&>(
                                _rcsTypeHandle,
                                RuntimeReflection::get_function_handle<HotReloader::RuntimeCompilerSystem::hot_object*, const RuntimeReflection::__duckvil_resource_type_t&>(_rcsTypeHandle, "Swap"),
                                _data->m_pRCS,
                                &_hot,
                                _type
                            );

                            const auto& _initFunctionHandle = RuntimeReflection::get_function_handle(_type, "Init");

                            RuntimeReflection::invoke_member(_type, _initFunctionHandle, DUCKVIL_TRACK_KEEPER_GET_OBJECT(_hot.m_pObject));
                        }
                    }
                }

                if(!_found)
                {
                    for(size_t j = 0; j < _newTypes.m_ullCount; ++j)
                    {
                        const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _newTypes.m_aTypes[j];

                        const auto& _initFunctionHandle = RuntimeReflection::get_function_handle(_type, "Init");

                        void* _newObject = RuntimeReflection::create(_data->m_heap, _type, true);

                        RuntimeReflection::invoke_member(_type, _initFunctionHandle, DUCKVIL_TRACK_KEEPER_GET_OBJECT(_newObject));

                        _data->m_projectManagerEventPool.Broadcast(AddScriptEvent{ static_cast<HotReloader::ITrackKeeper*>(_newObject) });
                    }
                }
            }), false, _options);
        }
        else if((_status == HotReloader::FileWatcher::FileStatus_Created || _status == HotReloader::FileWatcher::FileStatus_Modified) && _file.extension() == ".h")
        {
            PlugNPlay::__module _module;
            Process::ftable _process;
            Process::data _processData;

            PlugNPlay::module_init(&_module);

            PlugNPlay::__module_information _processModuleInfo("Process");

            _module.load(&_processModuleInfo);

            void (*_duckvilProcessInit)(Process::ftable* _pFTable);

            _module.get(_processModuleInfo, "duckvil_process_init", reinterpret_cast<void**>(&_duckvilProcessInit));

            _duckvilProcessInit(&_process);

            _process.m_fnInit(_data->m_heap.GetMemoryInterface(), _data->m_heap.GetAllocator(), &_processData);
            _process.m_fnSetup(&_processData);

            if(_status == HotReloader::FileWatcher::FileStatus_Modified)
            {
                _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _data->m_loadedProject.m_sPath + " -is_absolute -file " + _file + "\n_COMPLETION_TOKEN_\n").c_str());
            }
            else if(_status == HotReloader::FileWatcher::FileStatus_Created)
            {
                _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _data->m_loadedProject.m_sPath + "\n_COMPLETION_TOKEN_\n").c_str());
            }

            _process.m_fnWait(&_processData);
            _process.m_fnStop(&_processData);

            if(_process.m_fnTerminate(&_processData))
            {
                _process.m_fnCleanup(_data->m_heap.GetMemoryInterface(), _data->m_heap.GetAllocator(), &_processData);
            }

            nlohmann::json _j;

            std::ifstream _iJson(_data->m_loadedProject.m_sPath / "config.json");
            _iJson >> _j;
            _iJson.close();

            if(_status == HotReloader::FileWatcher::FileStatus_Created)
            {
                nlohmann::json _f;

                _f["path"] = std::filesystem::relative(_file, _data->m_loadedProject.m_sPath.m_sText).string();
                _f["hash"] = md5(load_file_as_string(_file));

                _j["files"].insert(_j["files"].end() - 1, _f);
            }
            else if(_status == HotReloader::FileWatcher::FileStatus_Modified)
            {
                auto _res = std::find_if(_j["files"].begin(), _j["files"].end(), [_file, _data](const nlohmann::json& _element)
                {
                    auto _pathIt = _element.find("path");

                    return _pathIt != _element.end() && _pathIt->get<std::string>() == std::filesystem::relative(_file, _data->m_loadedProject.m_sPath.m_sText).string();
                });

                (*_res)["hash"] = md5(load_file_as_string(_file));
            }

            std::ofstream _oJson(_data->m_loadedProject.m_sPath / "config.json");
            _oJson << std::setw(4) << _j << std::endl;
            _oJson.close();
        }
        else if(_status == HotReloader::FileWatcher::FileStatus_Created && _file.extension() == ".cpp")
        {
            RuntimeCompiler::Options _options;

            _options.m_aIncludes.push_back((_data->m_loadedProject.m_sPath / "include").m_sText);
            _options.m_aIncludes.push_back((_data->m_loadedProject.m_sPath / "__generated_reflection__").m_sText);

            _data->m_fnCompile->Invoke(_data->m_pRCS, _data->m_loadedProject.m_sPath.m_sText, _file.string(), Utils::lambda([&](Memory::Vector<HotReloader::RuntimeCompilerSystem::hot_object>* _pHotObjects, duckvil_recorderd_types& _newTypes)
            {
                for(size_t j = 0; j < _newTypes.m_ullCount; ++j)
                {
                    const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _newTypes.m_aTypes[j];

                    const auto& _initFunctionHandle = RuntimeReflection::get_function_handle(_type, "Init");

                    void* _newObject = RuntimeReflection::create(_data->m_heap, _type, true);

                    RuntimeReflection::invoke_member(_type, _initFunctionHandle, DUCKVIL_TRACK_KEEPER_GET_OBJECT(_newObject));

                    _data->m_projectManagerEventPool.Broadcast(AddScriptEvent{ static_cast<HotReloader::ITrackKeeper*>(_newObject) });
                }
            }), false, _options);
        }
    }*/

    bool init_project_manager(data* _pData, const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool)
    {
        _pData->m_heap = _heap;
        _pData->m_pEngineEventPool = _pEngineEventPool;

        _pData->m_bLoaded = false;

        _pData->m_dOneSecond = 0;

        _pData->m_projectManagerEventPool = Event::Pool<Event::mode::immediate>(_heap);

        _pData->m_heap.Allocate(_pData->m_objectsHeap, 1024 * 4);

        return true;
    }

    PlugNPlay::__module_information load_module(const PlugNPlay::__module& _module, const Utils::string& _sFilename, const Utils::string& _sPath, uint32_t* _pRecordersCount)
    {
        PlugNPlay::__module_information _projectManager(_sFilename, _sPath);

        _module.load(&_projectManager);

        RuntimeReflection::GetRecordersCountFunction get_recorder_count = nullptr;
        void (*make_current_runtime_reflection_context)(const duckvil_frontend_reflection_context&);
        void (*make_current_logger_context)(const logger_context&);

        _module.get(_projectManager, "duckvil_get_runtime_reflection_recorder_count", reinterpret_cast<void**>(&get_recorder_count));
        _module.get(_projectManager, "duckvil_plugin_make_current_runtime_reflection_context", reinterpret_cast<void**>(&make_current_runtime_reflection_context));
        _module.get(_projectManager, "duckvil_plugin_make_current_logger_context", reinterpret_cast<void**>(&make_current_logger_context));

        make_current_runtime_reflection_context(RuntimeReflection::get_current());
        make_current_logger_context(logger_get_current());

        *_pRecordersCount = get_recorder_count();

        return _projectManager;
    }

    void register_module(data* _pData, const PlugNPlay::__module& _module, project* _pProject, const Utils::string& _sFilename, const Utils::string& _sPath)
    {
        uint32_t _recordersCount;

        const PlugNPlay::__module_information& _projectManager = load_module(_module, _sFilename, _sPath, &_recordersCount);

        for(uint32_t j = 0; j < _recordersCount; ++j)
        {
            RuntimeReflection::RecordFunction record = nullptr;

            _module.get(_projectManager, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if(record == nullptr)
            {
                // TODO: Should return false?

                continue;
            }

            duckvil_runtime_reflection_recorder_stuff _stuff =
            {
                ._pMemoryInterface = _pData->m_heap.GetMemoryInterface(),
                ._pAllocator = _pData->m_heap.GetAllocator(),
                ._pFunctions = RuntimeReflection::get_current().m_pRecorder,
                ._pData = RuntimeReflection::get_current().m_pReflectionData
            };

            duckvil_recorderd_types _types = record(_stuff);

            if(_pProject->m_aTypes.Full())
            {
                _pProject->m_aTypes.Resize(_pProject->m_aTypes.Size() * 2);
            }

            _pProject->m_aTypes.Allocate(_types);
        }
    }

    project load_project(data* _pData, const Utils::string& _sFilename, const Utils::string& _sPath)
    {
        Utils::string _projectCWD = Utils::string::parentPath(_sPath);

        {
            nlohmann::json _j;

            std::ifstream _iJson(_projectCWD / "config.json");
            _iJson >> _j;
            _iJson.close();
            bool _wasDifferent = false;

            for(const auto& _p : std::filesystem::recursive_directory_iterator((_projectCWD / "source").m_sText))
            {
                if(_p.path().extension() != ".cpp")
                {
                    continue;
                }

                auto _res = std::find_if(_j["files"].begin(), _j["files"].end(), [_projectCWD, _p](const nlohmann::json& _element)
                {
                    auto _pathIt = _element.find("path");
                    const auto& _path = std::filesystem::relative(_p.path(), _projectCWD.m_sText).string();

                    return _pathIt != _element.end() && _pathIt->get<std::string>() == Utils::replace_all(_path, "\\", "/");
                });

                std::string _md5 = md5(load_file_as_string(_p.path()));

                if(_res != _j["files"].end() && (*_res)["hash"].get<std::string>() != _md5)
                {
                    (*_res)["hash"] = _md5;

                    _wasDifferent = true;
                }
            }

            for(const auto& _p : std::filesystem::recursive_directory_iterator((_projectCWD / "include").m_sText))
            {
                if(_p.path().extension() != ".h")
                {
                    continue;
                }

                auto _res = std::find_if(_j["files"].begin(), _j["files"].end(), [_projectCWD, _p](const nlohmann::json& _element)
                {
                    auto _pathIt = _element.find("path");
                    const auto& _path = std::filesystem::relative(_p.path(), _projectCWD.m_sText).string();

                    return _pathIt != _element.end() && _pathIt->get<std::string>() == Utils::replace_all(_path, "\\", "/");
                });

                std::string _md5 = md5(load_file_as_string(_p.path()));

                if(_res != _j["files"].end() && (*_res)["hash"].get<std::string>() != _md5)
                {
                    (*_res)["hash"] = _md5;

                    _wasDifferent = true;
                }
            }

            if(_wasDifferent)
            {
#ifdef DUCKVIL_PLATFORM_WINDOWS
                if(!execute_command("CMake", (_projectCWD / "build").m_sText, ".."))
                {
                    // continue;
                }

                if(!execute_command(DUCKVIL_MSBUILD_PATH, (_projectCWD / "build").m_sText, (_projectCWD / "build" / (_sFilename + ".sln") + " /t:Build").m_sText))
                {
                    // continue;
                }
#endif
            }

            std::ofstream _oJson(_projectCWD / "config.json");
            _oJson << std::setw(4) << _j << std::endl;
            _oJson.close();
        }

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        uint32_t _recordersCount = 0;

    // TODO: Temporary fix
        static project _project = { .m_module = load_module(_module, _sFilename, _sPath, &_recordersCount) };

        _project.m_sPath = Utils::string::parentPath(_sPath);

        // _pData->m_heap.Allocate(_project.m_aTypes, 1);
        _project.m_aTypes = Memory::ThreadsafeVector<duckvil_recorderd_types>(_pData->m_heap);

        duckvil_runtime_reflection_recorder_stuff _stuff =
        {
            ._pMemoryInterface = _pData->m_heap.GetMemoryInterface(),
            ._pAllocator = _pData->m_heap.GetAllocator(),
            ._pFunctions = RuntimeReflection::get_current().m_pRecorder,
            ._pData = RuntimeReflection::get_current().m_pReflectionData
        };

        for(uint32_t j = 0; j < _recordersCount; ++j)
        {
            RuntimeReflection::RecordFunction record = nullptr;

            _module.get(_project.m_module, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if(record == nullptr)
            {
                // TODO: Should return false?

                continue;
            }

            duckvil_recorderd_types _types = record(_stuff);

            if(_project.m_aTypes.Full())
            {
                _project.m_aTypes.Resize(_project.m_aTypes.Size() * 2);
            }

            _project.m_aTypes.Allocate(_types);
        }

        for(const auto& _file : std::filesystem::directory_iterator(_sPath.m_sText))
        {
            if(!_file.is_directory() && _file.path().extension() == ".dll" && _file.path().stem().string() != _sFilename)
            {
                // printf("aaa\n");

                register_module(_pData, _module, &_project, _file.path().stem().string(), _sPath);
            }
        }

        {
            auto _runtimeCompilerType = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>();

            RuntimeReflection::ReflectedType _type(RuntimeReflection::ReflectedType::Tag<HotReloader::RuntimeCompilerSystem>{});

            HotReloader::RuntimeCompilerSystem::user_data* _actionData =
                _pData->m_heap.Allocate<HotReloader::RuntimeCompilerSystem::user_data>();

            _pData->m_pRuntimeCompilerSystem = (HotReloader::RuntimeCompilerSystem*)_type.Create<
                const Memory::FreeList&,
                Event::Pool<Event::mode::immediate>*,
                Event::Pool<Event::mode::immediate>*,
                HotReloader::FileWatcher::ActionCallback,
                void*
            >(
                (const Memory::FreeList&)_pData->m_heap,
                (const Memory::FreeList&)_pData->m_heap,
                _pData->m_pEngineEventPool,
                static_cast<Event::Pool<Event::mode::immediate>*>(RuntimeReflection::get_current().m_pReflectionData->m_pEvents),
                HotReloader::RuntimeCompilerSystem::Action,
                static_cast<void*>(_actionData)
            );

            _actionData->m_pRuntimeCompiler = _pData->m_pRuntimeCompilerSystem;

            const auto& _setCWDHandle = RuntimeReflection::get_function_handle<const std::filesystem::path&>({ _runtimeCompilerType.m_ID }, "SetCWD");

            RuntimeReflection::invoke_member<const std::filesystem::path&>({ _runtimeCompilerType.m_ID }, _setCWDHandle, _pData->m_pRuntimeCompilerSystem, std::filesystem::path(_projectCWD.m_sText));

            const auto& _getCompilerHandle = RuntimeReflection::get_function_handle({ _runtimeCompilerType.m_ID }, "GetCompiler");

            auto _c = RuntimeReflection::invoke_member_result<RuntimeCompiler::ICompiler*>({ _runtimeCompilerType.m_ID }, _getCompilerHandle, _pData->m_pRuntimeCompilerSystem);

            _c->AddInclude((std::filesystem::path(DUCKVIL_CWD) / "include").string());
            _c->AddInclude((std::filesystem::path(DUCKVIL_CWD) / "__generated_reflection__").string());

            (static_cast<Event::Pool<Event::mode::immediate>*>(RuntimeReflection::get_current().m_pReflectionData->m_pEvents))->Add<RuntimeReflection::TrackedObjectCreatedEvent>(_pData->m_pRuntimeCompilerSystem);

            _pData->m_fnRuntimeCompilerUpdate = _type.GetFunctionCallbackM<ISystem, double>("Update")->m_fnFunction;
            _pData->m_fnRuntimeCompilerInit = _type.GetFunctionCallbackMR<bool, ISystem, const std::vector<std::filesystem::path>&>("Init")->m_fnFunction;

            _type.InvokeM<const Memory::FreeList&>("SetObjectsHeap", _pData->m_pRuntimeCompilerSystem, _pData->m_objectsHeap);
            // _type.Invoke<Memory::Vector<PlugNPlay::__module_information>*>("SetModules", _pData->m_pRuntimeCompilerSystem, &_pData->m_aLoadedModules);
            _type.InvokeM<Memory::ThreadsafeVector<duckvil_recorderd_types>*>("SetReflectedTypes", _pData->m_pRuntimeCompilerSystem, &_project.m_aTypes);

            if(!(_pData->m_pRuntimeCompilerSystem->*_pData->m_fnRuntimeCompilerInit)(
                {
                    std::filesystem::path(_projectCWD.m_sText) / "source",
                    std::filesystem::path(_projectCWD.m_sText) / "include"
                }
            ))
            {
                throw std::exception();
            }
        }

        for(const auto& _type : _project.m_aTypes)
        {
            for(uint32_t i = 0; i < _type.m_ullCount; ++i)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _typeID = _type.m_aTypes[i];
                const auto& _variant = RuntimeReflection::get_meta(_typeID, ProjectManager::project_type::standalone);

                if(_variant.m_ullTypeID == typeid(bool).hash_code() && *(bool*)_variant.m_pData)
                {
                    RequestSystemEvent _rEvent;

                    _rEvent.m_typeHandle = RuntimeReflection::get_type("EntityFactory", { "Duckvil" });

                    _pData->m_pEngineEventPool->Broadcast(_rEvent);

                    _project.m_uiTypeHandleID = _typeID;
                    _project.m_pObject = RuntimeReflection::create<const Memory::FreeList&, const Memory::ThreadsafeVector<duckvil_recorderd_types>*, const Duckvil::PlugNPlay::__module_information&, EntityFactory*>(_pData->m_heap, _typeID, false, _pData->m_heap, &_project.m_aTypes, _project.m_module, static_cast<EntityFactory*>(_rEvent.m_pRequestedSystem));

                    _project.m_uiInitFunctionHandleID = RuntimeReflection::get_function_handle<Event::Pool<Event::mode::immediate>*>(_typeID, "Init");
                    _project.m_uiUpdateFunctionHandleID = RuntimeReflection::get_function_handle(_typeID, "Update");

                    bool _result = RuntimeReflection::invoke_member_result<bool, Event::Pool<Event::mode::immediate>*>(_typeID, _project.m_uiInitFunctionHandleID, _project.m_pObject, &_pData->m_projectManagerEventPool);

                    _pData->m_fnUpdateProject = (RuntimeReflection::__proxy_member_function<void>*)RuntimeReflection::get_function(_typeID, _project.m_uiUpdateFunctionHandleID).m_pFunction;
                }
            }
        }

        _pData->m_loadedProject = _project;
        _pData->m_bLoaded = true;

        _pData->m_pEngineEventPool->Broadcast(OnLoadEvent{ _project });

        return _project;
    }

    bool create_project(data* _pData, const Utils::string& _sName, const Utils::string& _sPath)
    {
        const Utils::string& _projectPath = _sPath / _sName;

        std::filesystem::create_directory(_projectPath.m_sText);
        std::filesystem::create_directory((_projectPath / "source").m_sText);
        std::filesystem::create_directory((_projectPath / "include").m_sText);
        std::filesystem::create_directory((_projectPath / "build").m_sText);
        std::filesystem::create_directory((_projectPath / "__generated_reflection__").m_sText);

        Utils::string _cwd = DUCKVIL_CWD;

        TemplateEngine::generate(
            {
                { _cwd / "resource/template/new-project.tpl.h", _projectPath / "include/Project.h" },
                { _cwd / "resource/template/new-project.tpl.cpp", _projectPath / "source/Project.cpp" },
                { _cwd / "resource/template/project-main-cmake.tpl.txt", _projectPath / "CMakeLists.txt" },
                { _cwd / "resource/template/project-source-cmake.tpl.txt", _projectPath / "source/CMakeLists.txt" }
            },
            {
                { "projectName", _sName },
                { "duckvilWorkingDirectory", _cwd },
                { "projectPath", _projectPath }
            }
        );

        TemplateEngine::generate(
            {
                { _cwd / "resource/template/project-config.tpl.json", _projectPath / "config.json" }
            },
            {
                { "project_cpp_md5", md5(load_file_as_string(_projectPath / "source/Project.cpp")) },
                { "project_h_md5", md5(load_file_as_string(_projectPath / "include/Project.h")) }
            }
        );

        PlugNPlay::__module _module;
        Process::ftable _process;
        Process::data _processData;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _processModuleInfo("Process");

        _module.load(&_processModuleInfo);

        void (*_duckvilProcessInit)(Process::ftable* _pFTable);

        _module.get(_processModuleInfo, "duckvil_process_init", reinterpret_cast<void**>(&_duckvilProcessInit));

        _duckvilProcessInit(&_process);

        _process.m_fnInit(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), &_processData);
        _process.m_fnSetup(&_processData);

        _process.m_fnWrite(&_processData, std::string((std::filesystem::path(DUCKVIL_OUTPUT) / "ReflectionGenerator.exe -CWD ").string() + _projectPath + "\n_COMPLETION_TOKEN_\n").c_str());
        _process.m_fnWait(&_processData);
        _process.m_fnStop(&_processData);

        if(_process.m_fnTerminate(&_processData))
        {
            _process.m_fnCleanup(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), &_processData);
        }

#ifdef DUCKVIL_PLATFORM_WINDOWS
        if(!execute_command("CMake", (_projectPath + "/build").m_sText, ("-DDUCKVIL_PATH=\"" + std::string(DUCKVIL_CWD) + "\" ..").c_str()))
        {
            return false;
        }

        if(!execute_command(DUCKVIL_MSBUILD_PATH, (_projectPath + "/build").m_sText, (_projectPath + "/build/" + _sName + ".sln /t:Build").m_sText))
        {
            return false;
        }
#endif

        return true;
    }

    void update(data* _pData, double _dDelta)
    {
        if(_pData->m_bLoaded)
        {
            if(_pData->m_dOneSecond >= 1)
            {
                (_pData->m_pRuntimeCompilerSystem->*_pData->m_fnRuntimeCompilerUpdate)(_dDelta);
            }

            _pData->m_fnUpdateProject->Invoke(_pData->m_loadedProject.m_pObject);
        }

        if(_pData->m_dOneSecond >= 1)
        {
            _pData->m_dOneSecond = 0;
        }

        _pData->m_dOneSecond += _dDelta;
    }

}}

void duckvil_project_manager_init(Duckvil::ProjectManager::ftable* _pFTable)
{
    _pFTable->m_fnInitProjectManager = &Duckvil::ProjectManager::init_project_manager;
    _pFTable->m_fnLoadProject = &Duckvil::ProjectManager::load_project;
    _pFTable->m_fnCreateProject = &Duckvil::ProjectManager::create_project;
    _pFTable->m_fnUpdate = &Duckvil::ProjectManager::update;
}