#pragma once

#include <cstddef>

namespace Duckvil { namespace RuntimeReflection {

    struct PrepareObjectEvent
    {
        void* m_pObject = nullptr;
        std::size_t m_ullTypeID = static_cast<std::size_t>(-1);
    };

}}