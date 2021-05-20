#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Editor/Widgets/ViewportWidget.generated.h"

namespace Duckvil { namespace Editor {

    class ViewportWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        void* m_pTextureID;

    public:
        ViewportWidget(const Memory::FreeList& _heap);
        ~ViewportWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void SetViewportTexture(void* _pTexture);
    };

}}