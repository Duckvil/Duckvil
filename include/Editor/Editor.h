#pragma once

#include "Utils/Macro.h"

#include "Memory/FreeListAllocator.h"
#include "Memory/FreeList.h"

#include "Window/IWindow.h"

#include "Editor/Draw.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Event/ImmediatePool.h"

namespace Duckvil { namespace Editor {

    struct EditorFTable
    {
        void* (*m_fnInit)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Window::IWindow* _pWindow);
        void (*m_fnRender)(void* _pData, Window::IWindow* _pWindow);

        void (*m_fnAddDraw)(void* _pData, Draw _draw);
        void (*m_fnRemoveDraw)(void* _pData, void* _pObject);

        void (*m_fnPostInit)(const Memory::FreeList& _heap, void* _pData);
    };

    struct ImGuiEditorData
    {
        Memory::Vector<Editor::Draw> m_aDraws;
        void* _ctx;
        Event::Pool<Event::mode::immediate> m_pEditorEvents;
    };

}}

DUCKVIL_EXPORT Duckvil::Editor::EditorFTable* duckvil_editor_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);