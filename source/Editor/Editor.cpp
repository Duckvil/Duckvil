#include "Editor/Editor.h"

#include "Editor/ImGui/ImGuiEditor.h"

namespace Duckvil { namespace Editor {

    

}}

Duckvil::Editor::EditorFTable* duckvil_editor_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Editor::EditorFTable* _result = (Duckvil::Editor::EditorFTable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Editor::EditorFTable), alignof(Duckvil::Editor::EditorFTable));

    _result->m_fnInit = &Duckvil::Editor::ImGuiEditor::init;
    _result->m_fnRender = &Duckvil::Editor::ImGuiEditor::render;

    _result->m_fnAddDraw = &Duckvil::Editor::ImGuiEditor::add_draw;
    _result->m_fnRemoveDraw = &Duckvil::Editor::ImGuiEditor::remove_draw;

    _result->m_fnPostInit = &Duckvil::Editor::ImGuiEditor::post_init;

    return _result;
}