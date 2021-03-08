#pragma once

#include "Editor/Widget.h"

#include "HotReloader/ITrackKeeper.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Editor {

    struct Draw
    {
        void (Widget::*m_fnDraw)();
        void (Widget::*m_fnInit)(void*);
        HotReloader::ITrackKeeper* m_pObject;
    };

}}