#pragma once

#include "ECS/Entity.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Markers.h"

#include "Graphics/TransformComponent.h"

#include "Engine/ScriptComponent.generated.h"

namespace Duckvil {

    namespace Editor {

        class EntityInspectorWidget;

    }

    class NativeScriptBase
    {
        friend class Editor::EntityInspectorWidget;
    private:
        ECS::Entity m_entity;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        double m_dDelta;

    public:
        virtual ~NativeScriptBase()
        {

        }

        virtual void Update(double _dDelta)
        {

        }

        virtual bool Init()
        {
            return true;
        }

        void SetEntity(ECS::Entity _entity)
        {
            m_entity = _entity;
        }

        const ECS::Entity& GetEntity() const
        {
            return m_entity;
        }

		void SetDelta(double _dDelta)
        {
            m_dDelta = _dDelta;
        }
    };

    DUCKVIL_CLASS("Component")
    struct ScriptComponent
    {
        DUCKVIL_GENERATED_BODY

        Memory::fixed_vector_allocator* m_pScripts = nullptr;
        // void (Script::*m_fnUpdate)(double);
        // RuntimeReflection::__duckvil_resource_type_t m_type;
    };

}