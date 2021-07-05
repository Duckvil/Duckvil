#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Editor {

    struct CloseWidgetEvent
    {
        RuntimeReflection::__duckvil_resource_type_t _typeHandle;
        void* m_pObject;
    };

}}