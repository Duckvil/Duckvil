#pragma once

#include "Utils/Macro.h"

#include "Memory/FreeListAllocator.h"
#include "Memory/FreeList.h"

#include "Window/IWindow.h"

#include "Editor/Draw.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Event/ImmediatePool.h"

#include "HotReloader/Events/HotReloadEvent.h"

#include "HotReloader/RuntimeCompilerSystem.h"

namespace Duckvil { namespace Editor {

    struct ImGuiEditorData
    {
        Memory::Vector<Editor::Draw> m_aDraws;
        void* _ctx;
        Event::Pool<Event::mode::immediate> m_pEditorEvents;
        Memory::FreeList m_heap;
    };

    struct EditorFTable
    {
        void* (*m_fnInit)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Window::IWindow* _pWindow);
        void (*m_fnRender)(void* _pData, Window::IWindow* _pWindow);

        void (*m_fnAddDraw)(void* _pData, Draw _draw);
        // void (*m_fnRemoveDraw)(void* _pData, void* _pObject);

        void (*m_fnPostInit)(const Memory::FreeList& _heap, void* _pData, EditorFTable* _pEditor, Event::Pool<Event::mode::immediate>* _pEventPool, HotReloader::RuntimeCompilerSystem* _pHotReloader);
        void (*m_fnHotReloadInit)(ImGuiEditorData* _pData, const HotReloader::HotReloadedEvent& _event);
    };

}}

DUCKVIL_EXPORT Duckvil::Editor::EditorFTable* duckvil_editor_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);