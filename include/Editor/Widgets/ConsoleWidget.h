#pragma once

#include "Editor/Widget.h"

#include "Editor/Widgets/ConsoleWidget.generated.h"

#include "Memory/FreeList.h"

namespace Duckvil { namespace Editor {

    class ConsoleWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:

    public:
        ConsoleWidget(const Memory::FreeList& _heap);
        ~ConsoleWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}