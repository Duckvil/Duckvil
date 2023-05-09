#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_SceneHierarchyWidget_h_25_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_SceneHierarchyWidget_h_25_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<SceneHierarchyWidget*>(_pObject); }
#define Editor_Widgets_SceneHierarchyWidget_h_25_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pECS", m_pECS); \
_pSerializer->SerializeProperty("m_entityQuery", m_entityQuery); \
_pSerializer->SerializeProperty("m_pEntityFactory", m_pEntityFactory); \
_pSerializer->SerializeProperty("m_selectedEntity", m_selectedEntity); \
}
#define Editor_Widgets_SceneHierarchyWidget_h_25_GENERATED_BODY \
Editor_Widgets_SceneHierarchyWidget_h_25_REFLECTION_MODULE_DUMMY \
Editor_Widgets_SceneHierarchyWidget_h_25_REFLECTION_MODULE_CASTER \
Editor_Widgets_SceneHierarchyWidget_h_25_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_SceneHierarchyWidget_h

