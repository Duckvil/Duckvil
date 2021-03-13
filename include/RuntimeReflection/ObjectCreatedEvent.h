#pragma once

namespace Duckvil { namespace RuntimeReflection {

    struct ObjectCreatedEvent
    {
        const void* m_pObject;
        size_t m_ullTypeID;
    };

}}