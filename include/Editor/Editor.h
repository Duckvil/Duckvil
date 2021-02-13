#pragma once

#include "Utils/Macro.h"

#include "Memory/FreeListAllocator.h"

#include "Window/IWindow.h"

#include "Editor/Draw.h"

namespace Duckvil { namespace Editor {

    struct EditorFTable
    {
        void* (*m_fnInit)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Window::IWindow* _pWindow);
        void (*m_fnRender)(void* _pData, Window::IWindow* _pWindow);

        void (*m_fnAddDraw)(void* _pData, Draw _draw);
        void (*m_fnRemoveDraw)(void* _pData, void* _pObject);

        void (*m_fnPostInit)(void* _pData);
    };

}}

DUCKVIL_EXPORT Duckvil::Editor::EditorFTable* duckvil_editor_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);