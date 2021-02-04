#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#undef GetObject

namespace Duckvil { namespace HotReloader {

    struct ITrackKeeper
    {
        friend class RuntimeCompilerSystem;
    private:
        virtual void SetObject(void* _pObject) = 0;

    public:
        virtual void* GetObject() const = 0;
        virtual RuntimeReflection::__duckvil_resource_type_t GetTypehandle() const = 0;
    };

}}