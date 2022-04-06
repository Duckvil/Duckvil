#pragma once

#include "Utils/Utils.h"

#include "ProjectManager/Events/OnLoadEvent.h"

namespace Duckvil { namespace ProjectManager {

    struct LoadProjectEvent
    {
        Utils::string m_sProjectName;
        project* m_pLoadedProject;
    };

}}