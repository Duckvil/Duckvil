#pragma once

#include <cstdint>

#include "HotReloader/ITrackKeeper.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace HotReloader {

    struct HotReloadedEvent
    {
        enum stage : uint8_t
        {
            stage_not_specified,
            stage_before_compile,
            stage_after_compile,
            stage_after_swap
            // And more...
        };

        HotReloadedEvent()
        {
            m_stage = stage::stage_not_specified;
            m_pObject = nullptr;
        }

        void* m_pObject;
        void* m_pOldObject;
        HotReloader::ITrackKeeper* m_pTrackKeeper;
        HotReloadedEvent::stage m_stage;
        RuntimeReflection::__duckvil_resource_type_t _typeHandle;
    };

}}