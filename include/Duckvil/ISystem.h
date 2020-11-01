#pragma once

#include "Logger/Logger.h"

namespace Duckvil {

    struct ISystem
    {
        Logger::__ftable* m_pLogger;
        Logger::__data* m_pLoggerData;
    };

}