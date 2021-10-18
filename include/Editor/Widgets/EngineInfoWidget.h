#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Engine/Time.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "HotReloader/ReflectionFlags.h"

#include "Editor/Widgets/EngineInfoWidget.generated.h"

namespace Duckvil { namespace Editor {

    class EngineInfoWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;

        __time_data* m_pTime;

    public:
        EngineInfoWidget();
        EngineInfoWidget(const Memory::FreeList& _heap);
        ~EngineInfoWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}