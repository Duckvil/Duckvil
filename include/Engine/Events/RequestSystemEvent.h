#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil {

    struct RequestSystemEvent
    {
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        void* m_pRequestedSystem;
    };

}