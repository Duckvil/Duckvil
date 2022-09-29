#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_EntityInspectorWidget_h_29_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_EntityInspectorWidget_h_29_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<EntityInspectorWidget*>(_pObject); }
#define Editor_Widgets_EntityInspectorWidget_h_29_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_selectedEntity", m_selectedEntity); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_aFunctions", m_aFunctions); \
_pSerializer->SerializeProperty("m_aScripts", m_aScripts); \
_pSerializer->SerializeProperty("m_bPopupOpened", m_bPopupOpened); \
_pSerializer->SerializeProperty("m_pEntityFactory", m_pEntityFactory); \
_pSerializer->SerializeProperty("m_aComponents", m_aComponents); \
_pSerializer->SerializeProperty("m_iCurrentComponentItem", m_iCurrentComponentItem); \
_pSerializer->SerializeProperty("m_aScriptNames", m_aScriptNames); \
_pSerializer->SerializeProperty("m_iCurrentScriptItem", m_iCurrentScriptItem); \
_pSerializer->SerializeProperty("m_ppAddress", m_ppAddress); \
}
#define Editor_Widgets_EntityInspectorWidget_h_29_GENERATED_BODY \
Editor_Widgets_EntityInspectorWidget_h_29_REFLECTION_MODULE_DUMMY \
Editor_Widgets_EntityInspectorWidget_h_29_REFLECTION_MODULE_CASTER \
Editor_Widgets_EntityInspectorWidget_h_29_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_EntityInspectorWidget_h

