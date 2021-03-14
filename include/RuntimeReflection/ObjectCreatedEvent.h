#pragma once

#include <cstddef>

namespace Duckvil { namespace RuntimeReflection {

    struct ObjectCreatedEvent
    {
        const void* m_pObject;
        std::size_t m_ullTypeID;
    };

}}