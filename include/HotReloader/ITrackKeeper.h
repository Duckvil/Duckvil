#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#undef GetObject

namespace Duckvil { namespace HotReloader {

    struct ITrackKeeper
    {
        virtual void* GetObject() const = 0;
        virtual RuntimeReflection::__duckvil_resource_type_t GetTypehandle() const = 0;

        void* operator->()
        {
            return GetObject();
        }
    };

}}