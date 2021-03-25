#pragma once

#include "HotReloader/ITrackKeeper.h"

#undef GetObject

namespace Duckvil { namespace HotReloader {

    class TrackKeeper : public ITrackKeeper
    {
    private:
        void* m_pObject;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;

        void SetObject(void* _pObject) override;

    public:
        TrackKeeper(void* _pObject, RuntimeReflection::__duckvil_resource_type_t _typeHandle);
        ~TrackKeeper();

        void* GetObject() const override;
        RuntimeReflection::__duckvil_resource_type_t GetTypeHandle() const override;
    };

}}