#include "{$scriptName}.h"

#include "imgui/imgui.h"

namespace {$projectName} {

	{$scriptName}::{$scriptName}()
	{

	}

	{$scriptName}::~{$scriptName}()
	{

	}

	bool {$scriptName}::Init()
    {
        return true;
    }

    void {$scriptName}::Update(double _dDelta)
    {

    }

    void {$scriptName}::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void {$scriptName}::OnDraw()
    {
        ImGui::Begin("XXX\n");

        ImGui::Text("xx");

        ImGui::End();
    }

}