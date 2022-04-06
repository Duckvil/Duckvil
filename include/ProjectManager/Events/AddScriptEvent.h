#pragma once

// #include "ProjectManager/Script.h"
#include "HotReloader/ITrackKeeper.h"

namespace Duckvil { namespace ProjectManager {

    struct AddScriptEvent
    {
        // Project::Script* m_pScript;
        HotReloader::ITrackKeeper* m_pTrackKeepedScript;
    };

}}