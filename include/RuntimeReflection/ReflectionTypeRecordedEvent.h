#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    struct ReflectionTypeRecordedEvent
    {
        DUCKVIL_RESOURCE(type_t) m_typeHandle;
    };

}}