#include "Editor/Widgets/ConsoleWidget.h"

#include "imgui/imgui.h"

namespace Duckvil { namespace Editor {

    ConsoleWidget::ConsoleWidget(const Memory::FreeList& _heap)
    {

    }

    ConsoleWidget::~ConsoleWidget()
    {

    }

    void ConsoleWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void ConsoleWidget::OnDraw()
    {
        ImGui::Begin("Console");

        ImGui::End();
    }

}}