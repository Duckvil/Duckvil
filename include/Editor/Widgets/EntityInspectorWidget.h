#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Event/ImmediatePool.h"

#include "Editor/Events/EntitySelectedEvent.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Engine/EntityFactory.h"
#include "Engine/ReflectionFlags.h"

#include "HotReloader/Events/SwapEvent.h"

#undef GetObject

#include "ProjectManager/Events/OnLoadEvent.h"

#include "Editor/Widgets/EntityInspectorWidget.generated.h"

namespace Duckvil { namespace Editor {

    DUCKVIL_CLASS(ReflectionFlags_AutoEventsAdding)
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
            Utils::string m_sTypeName;
            Memory::Vector<EntityInspectorWidget::Function> m_aFunctions;
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        };

        struct Script
        {
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
            Utils::string m_sTypeName;
        };

        Entity m_selectedEntity;

        Memory::FreeList m_heap;

        // Memory::Vector<RuntimeReflection::__duckvil_resource_type_t> m_aComponentTypeHandles;
        Memory::Vector<EntityInspectorWidget::Component> m_aFunctions;
        Memory::Vector<EntityInspectorWidget::Script> m_aScripts;

        bool m_bPopupOpened;

        EntityFactory* m_pEntityFactory;

        std::vector<const char*> m_aComponents;
        int m_iCurrentComponentItem;

        std::vector<const char*> m_aScriptNames;
        int m_iCurrentScriptItem;

        const char** m_ppLabel;
        uint8_t** m_ppAddress;
        const void** m_pFunction;
        void (*m_fnGeneratedFunction)();

    public:
        EntityInspectorWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Editor") Event::Pool<Event::mode::immediate>* _pEditorEventPool, EntityFactory* _pEntityFactory, DUCKVIL_ARGUMENT("Engine") Event::Pool<Event::mode::immediate>* _pEngineEventPool);
        ~EntityInspectorWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void OnEvent(const EntitySelectedEvent& _event);
        void OnEvent(const HotReloader::SwapEvent& _event);
        void OnEvent(const ProjectManager::OnLoadEvent& _event);
    };

}}