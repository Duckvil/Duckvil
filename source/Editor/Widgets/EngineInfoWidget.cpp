#include "Editor/Widgets/EngineInfoWidget.h"

#include "imgui/imgui.h"

namespace Duckvil { namespace Editor {

    EngineInfoWidget::EngineInfoWidget()
    {

    }

    EngineInfoWidget::EngineInfoWidget(const Memory::FreeList& _heap) :
        m_heap(_heap)
    {

    }

    EngineInfoWidget::~EngineInfoWidget()
    {

    }

    void EngineInfoWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);

        m_pTime = *(__time_data**)RuntimeReflection::get_meta_value_ptr(RuntimeReflection::get_type("__data", { "Duckvil" }), "Time");
    }

    void EngineInfoWidget::OnDraw()
    {
        ImGui::Begin("EngineInfoWidget");

        ImGui::Text("Delta: %f", m_pTime->m_dDelta * 1000.f);
        ImGui::Text("Memory used: %i of: %i", m_heap.GetAllocator()->m_ullUsed, m_heap.GetAllocator()->m_ullCapacity);
        // ImGui::Text("AAAAA");

        ImGui::End();
    }

}}