#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Event/ImmediatePool.h"

#include "Editor/Events/EntitySelectedEvent.h"

#include "Editor/Widgets/EntityInspectorWidget.generated.h"

namespace Duckvil { namespace Editor {

    class EntityInspectorWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Entity m_selectedEntity;

        Memory::FreeList m_heap;

        Memory::Vector<RuntimeReflection::__duckvil_resource_type_t> m_aComponentTypeHandles;

    public:
        EntityInspectorWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Editor") Event::Pool<Event::mode::immediate>* _pEditorEventPool);
        ~EntityInspectorWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void OnEvent(const EntitySelectedEvent& _event);
    };

}}