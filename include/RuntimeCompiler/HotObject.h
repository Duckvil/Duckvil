#pragma once

#include <cstddef>

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace RuntimeCompiler {

    struct HotObject
    {
        HotObject()
        {
            m_ullHotObjectID = 0;
        }

        std::size_t m_ullHotObjectID;
        RuntimeReflection::__duckvil_resource_type_t m_type;
    };

}}