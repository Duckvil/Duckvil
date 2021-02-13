#pragma once

#include "Window/IWindow.h"

#include "Memory/FreeListAllocator.h"
#include "Memory/Vector.h"

#include "Editor/Draw.h"

namespace Duckvil { namespace Editor { namespace ImGuiEditor {

    void* init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Window::IWindow* _pWindow);
    void render(void* _pData, Window::IWindow* _pWindow);

    void add_draw(void* _pData, Editor::Draw _draw);
    void remove_draw(void* _pData, void* _pObject);

    void post_init(void* _pData);

    struct ImGuiEditorData
    {
        Memory::Vector<Editor::Draw> m_aDraws;
        void* _ctx;
    };

}}}