#pragma once

#include "HotReloader/ITrackKeeper.h"

namespace Duckvil { namespace RuntimeReflection {

    struct TrackedObjectCreatedEvent
    {
        HotReloader::ITrackKeeper* m_pTrackKeeper;
    };

}}