#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#undef GetObject

// If hot reloading is enabled, we want to use tracked object otherwise use straight object
#ifdef DUCKVIL_HOT_RELOADING
#define DUCKVIL_TRACK_KEEPER_GET_OBJECT(object) (static_cast<Duckvil::HotReloader::ITrackKeeper*>(object))->GetObject()
#define DUCKVIL_TRACK_KEEPER_OBJECT(object) Duckvil::HotReloader::ITrackKeeper*
#define DUCKVIL_TRACK_KEEPER_CAST(type, object) static_cast<Duckvil::HotReloader::ITrackKeeper*>(object)
#else
#define DUCKVIL_TRACK_KEEPER_GET_OBJECT(object) object
#define DUCKVIL_TRACK_KEEPER_OBJECT(object) object*
#define DUCKVIL_TRACK_KEEPER_CAST(type, object) static_cast<type*>(object)
#endif

namespace Duckvil { namespace HotReloader {

    struct ITrackKeeper
    {
        friend class RuntimeCompilerSystem;
    private:
        virtual void SetObject(void* _pObject) = 0;

    public:
        virtual void* GetObject() const = 0;
        virtual RuntimeReflection::__duckvil_resource_type_t GetTypeHandle() const = 0;
    };

}}