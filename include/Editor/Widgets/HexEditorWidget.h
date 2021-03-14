#pragma once

#include "Editor/Widget.h"

#include "imgui/ImGui.h"

#include "imgui_memory_editor/imgui_memory_editor.h"

#include "Memory/FreeList.h"

#include "Editor/Widgets/HexEditorWidgetEvent.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Editor/Widgets/HexEditorWidget.generated.h"

namespace Duckvil { namespace Editor {

    class HexEditorWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;
#ifdef DUCKVIL_MEMORY_DEBUGGER
        duckvil_memory_debug_info* m_pMemoryDebugInfo;
#endif

        float a1;
        float a2;

    public:
        HexEditorWidget(const Memory::FreeList& _heap);
        ~HexEditorWidget();

        MemoryEditor m_hexEditor;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        duckvil_memory_debug_info* m_pSelected;
#endif

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void OnEvent(const HexEditorWidgetInitEvent& _event);
    };

}}