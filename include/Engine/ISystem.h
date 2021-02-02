#pragma once

#include "Logger/Logger.h"

#include "HotReloader/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"

#include "Engine/ISystem.generated.h"

namespace Duckvil {

    DUCKVIL_CLASS(HotReloader::ReflectionFlags_Hot)
    struct ISystem
    {
        DUCKVIL_GENERATED_BODY

        Logger::__ftable* m_pLogger;
        Logger::__data* m_pLoggerData;
    };

}