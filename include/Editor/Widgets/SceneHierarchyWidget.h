#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "flecs/flecs.h"

#include "Engine/UUIDComponent.h"

#include "Event/ImmediatePool.h"

#include "Editor/Widgets/SceneHierarchyWidget.generated.h"

namespace Duckvil { namespace Editor {

    class SceneHierarchyWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        flecs::world* m_pECS;

        flecs::query<UUIDComponent> m_entityQuery;

        const Event::Pool<Event::mode::immediate>* m_pEditorEventPool;

    public:
        SceneHierarchyWidget(const Memory::FreeList& _heap, flecs::world* _pECS, DUCKVIL_ARGUMENT("Editor") const Event::Pool<Event::mode::immediate>* _pEditorEventPool);
        ~SceneHierarchyWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}