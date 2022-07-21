#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#include "HotReloader/ITrackKeeper.h"

namespace Duckvil { namespace HotReloader {

    struct SwapEvent
    {
        void* m_pOldObject;
        HotReloader::ITrackKeeper* m_pTrackKeeper;
        Utils::string m_sModuleName;
    };

}}