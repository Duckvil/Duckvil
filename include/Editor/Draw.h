#pragma once

#include "Editor/Widget.h"

#include "HotReloader/ITrackKeeper.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Editor {

    struct HotDraw
    {
        void (Widget::*m_fnDraw)();
        void (Widget::*m_fnInit)(void*);
        DUCKVIL_TRACK_KEEPER_OBJECT(Widget) m_pTrackKeeper;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    };

    struct Draw
    {
        void (Widget::*m_fnDraw)();
        void (Widget::*m_fnInit)(void*);
        void* m_pObject;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    };

}}