#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Event/ImmediatePool.h"

#include "Editor/Events/EntitySelectedEvent.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Editor/Widgets/EntityInspectorWidget.generated.h"

namespace Duckvil { namespace Editor {

    class EntityInspectorWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        struct Function
        {
            const void* m_pFunction;
            const char* m_sPropertyName;
            RuntimeReflection::__duckvil_resource_function_t m_hasFuncHandle;
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
            RuntimeReflection::__duckvil_resource_function_t m_getFuncHandle;
            size_t m_ullPropAddress;
        };

        struct Component
        {
            const char* m_sTypeName;
            Memory::Vector<EntityInspectorWidget::Function> m_aFunctions;
        };

        Entity m_selectedEntity;

        Memory::FreeList m_heap;

        // Memory::Vector<RuntimeReflection::__duckvil_resource_type_t> m_aComponentTypeHandles;
        Memory::Vector<EntityInspectorWidget::Component> m_aFunctions;

    public:
        EntityInspectorWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Editor") Event::Pool<Event::mode::immediate>* _pEditorEventPool);
        ~EntityInspectorWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void OnEvent(const EntitySelectedEvent& _event);
    };

}}