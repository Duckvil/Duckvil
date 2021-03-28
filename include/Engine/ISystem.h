#pragma once

#include "Logger/Logger.h"

#include "HotReloader/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"

#include "Engine/ISystem.generated.h"

namespace Duckvil {

    // Allow to retrieve system events
    struct ISystem
    {
        DUCKVIL_GENERATED_BODY

        // __logger_ftable* m_pLogger;
        // __logger_data* m_pLoggerData;
    };

}