#pragma once

#include "HotReloader/Events/HotReloadEvent.h"

#include "HotReloader/ITrackKeeper.h"

#undef GetObject

namespace Duckvil { namespace HotReloader {

    class TrackKeeper : public ITrackKeeper
    {
    private:
        void* m_pObject;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;

    public:
        TrackKeeper(void* _pObject, RuntimeReflection::__duckvil_resource_type_t _typeHandle);
        ~TrackKeeper();

        void OnEvent(const HotReloadedEvent& _event);

        void* GetObject() const override;
        RuntimeReflection::__duckvil_resource_type_t GetTypehandle() const override;
    };

}}