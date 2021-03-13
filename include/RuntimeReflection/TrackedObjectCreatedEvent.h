#pragma once

#include "HotReloader/TrackKeeper.h"

namespace Duckvil { namespace RuntimeReflection {

    struct TrackedObjectCreatedEvent
    {
        HotReloader::TrackKeeper* m_pTrackKeeper;
    };

}}