#pragma once

#include <cstdint>

#include "ProjectManager/Project.h"

#include "Utils/Utils.h"

#include "Memory/FreeList.h"

#include "Utils/Macro.h"

#include "HotReloader/FileWatcher.h"

// #include "RuntimeCompiler/RuntimeCompiler.h"

#include "HotReloader/RuntimeCompilerSystem.h"

namespace Duckvil { namespace ProjectManager {

    enum class project_type : uint8_t
    {
        standalone,
        plugin,
        mod
    };

    struct data
    {
        Memory::FreeList m_heap;
        HotReloader::FileWatcher* m_pFileWatcher;
        // RuntimeCompiler::ICompiler* m_pCompiler;
        HotReloader::RuntimeCompilerSystem* m_pRCS;

        typedef void (HotReloader::RuntimeCompilerSystem::*CompileCallback)(const std::filesystem::path&, const std::string&, void (*_fnSwap)(Memory::Vector<HotReloader::RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&), bool, const RuntimeCompiler::Options&);
        RuntimeReflection::__function<data::CompileCallback>* m_fnCompile;

        Duckvil::RuntimeReflection::__duckvil_resource_type_t m_fileWatcherType;
        Duckvil::RuntimeReflection::__duckvil_resource_function_t m_watchHandle;
        Duckvil::RuntimeReflection::__duckvil_resource_function_t m_updateHandle;

        RuntimeReflection::__proxy_member_function<void>* m_fnUpdateProject;

        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;
        Event::Pool<Event::mode::immediate> m_projectManagerEventPool;

        project m_loadedProject;
        bool m_bLoaded;

        double m_dOneSecond;
    };

    struct ftable
    {
        bool (*m_fnInitProjectManager)(data* _pData, const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool);
        project (*m_fnLoadProject)(data* _pData, const Utils::string& _sFilename, const Utils::string& _sPath);
        bool (*m_fnCreateProject)(data* _pData, const Utils::string& _sName, const Utils::string& _sPath);
        void (*m_fnUpdate)(data* _pData, double _dDelta);
        void (*m_fnSetRuntimeCompiler)(data* _pData, HotReloader::RuntimeCompilerSystem* _pRCS);
    };

}}

DUCKVIL_EXPORT void duckvil_project_manager_init(Duckvil::ProjectManager::ftable* _pFTable);