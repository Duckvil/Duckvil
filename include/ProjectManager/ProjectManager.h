#pragma once

#include <cstdint>

#include "ProjectManager/Project.h"

#include "Utils/Utils.h"

#include "Memory/FreeList.h"

#include "utils/Macro.h"

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
    };

    struct ftable
    {
        bool (*m_fnInitProjectManager)(data* _pData, const Memory::FreeList& _heap);
        project (*m_fnLoadProject)(data* _pData, const Utils::string& _sFilename, const Utils::string& _sPath);
    };

}}

DUCKVIL_EXPORT void duckvil_project_manager_init(Duckvil::ProjectManager::ftable* _pFTable);