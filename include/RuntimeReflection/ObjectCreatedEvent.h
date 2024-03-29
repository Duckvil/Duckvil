#pragma once

#include <cstddef>

namespace Duckvil { namespace RuntimeReflection {

    struct ObjectCreatedEvent
    {
        void* m_pObject = nullptr;
        std::size_t m_ullTypeID = -1;
    };

}}