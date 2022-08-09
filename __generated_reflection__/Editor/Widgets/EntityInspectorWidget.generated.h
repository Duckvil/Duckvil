#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_EntityInspectorWidget_h_17_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_EntityInspectorWidget_h_17_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<EntityInspectorWidget*>(_pObject); }
#define Editor_Widgets_EntityInspectorWidget_h_17_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_selectedEntity", m_selectedEntity); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_aComponentTypeHandles", m_aComponentTypeHandles); \
}
#define Editor_Widgets_EntityInspectorWidget_h_17_GENERATED_BODY \
Editor_Widgets_EntityInspectorWidget_h_17_REFLECTION_MODULE_DUMMY \
Editor_Widgets_EntityInspectorWidget_h_17_REFLECTION_MODULE_CASTER \
Editor_Widgets_EntityInspectorWidget_h_17_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_EntityInspectorWidget_h

