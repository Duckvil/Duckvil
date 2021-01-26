#pragma once

#include <cstdint>

namespace Duckvil { namespace HotReloader {

    struct HotReloadedEvent
    {
        enum stage : uint8_t
        {
            stage_not_specified,
            stage_before_compile,
            stage_after_compile
            // And more...
        };

        HotReloadedEvent()
        {
            m_stage = stage::stage_not_specified;
        }

        HotReloadedEvent::stage m_stage;
    };

}}