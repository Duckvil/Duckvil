#pragma once

#include <cstdint>

#include "ProjectManager/Project.h"

#include "Utils/Utils.h"

#include "Memory/FreeList.h"

#include "Utils/Macro.h"

#include "HotReloader/FileWatcher.h"

// #include "RuntimeCompiler/RuntimeCompiler.h"

#include "HotReloader/RuntimeCompilerSystem.h"

#include "Engine/ISystem.h"

#include "DependencyInjection/ReflectionFlags.h"

#include "ProjectManager/ProjectManager.generated.h"

namespace Duckvil { namespace ProjectManager {

    enum class project_type : uint8_t
    {
        standalone,
        plugin,
        mod
    };

    DUCKVIL_CLASS(Duckvil::DependencyInjection::INJECTABLE)
    struct data
    {
        DUCKVIL_GENERATED_BODY

        Memory::FreeList m_heap;
        HotReloader::RuntimeCompilerSystem* m_pRCS;

        RuntimeReflection::__proxy_member_function<void>* m_fnUpdateProject;

        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;
        Event::Pool<Event::mode::immediate> m_projectManagerEventPool;

        project m_loadedProject;
        bool m_bLoaded;

        double m_dOneSecond;

        typedef void (ISystem::*UpdateCallback)(double _dDelta);
        typedef bool (ISystem::*InitCallback)(const std::vector<std::filesystem::path>&);

        HotReloader::RuntimeCompilerSystem* m_pRuntimeCompilerSystem;
        data::InitCallback m_fnRuntimeCompilerInit;
        data::UpdateCallback m_fnRuntimeCompilerUpdate;
        Memory::FreeList m_objectsHeap;
    };

    DUCKVIL_CLASS(Duckvil::DependencyInjection::INJECTABLE)
    struct ftable
    {
        DUCKVIL_GENERATED_BODY

        bool (*m_fnInitProjectManager)(data* _pData, const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool);
        project (*m_fnLoadProject)(data* _pData, const Utils::string& _sFilename, const Utils::string& _sPath);
        bool (*m_fnCreateProject)(data* _pData, const Utils::string& _sName, const Utils::string& _sPath);
        void (*m_fnUpdate)(data* _pData, double _dDelta);
    };

}}

DUCKVIL_EXPORT void duckvil_project_manager_init(Duckvil::ProjectManager::ftable* _pFTable);