#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_ViewportWidget_h_47_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_ViewportWidget_h_47_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ViewportWidget*>(_pObject); }
#define Editor_Widgets_ViewportWidget_h_47_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_pRenderer", m_pRenderer); \
_pSerializer->SerializeProperty("m_pRendererData", m_pRendererData); \
_pSerializer->SerializeProperty("m_oldSize", m_oldSize); \
_pSerializer->SerializeProperty("m_pWindowEventPool", m_pWindowEventPool); \
_pSerializer->SerializeProperty("m_pEditorEventPool", m_pEditorEventPool); \
_pSerializer->SerializeProperty("m_viewport", m_viewport); \
_pSerializer->SerializeProperty("m_aKeys", m_aKeys); \
_pSerializer->SerializeProperty("m_bSkip", m_bSkip); \
_pSerializer->SerializeProperty("m_pECS", m_pECS); \
_pSerializer->SerializeProperty("m_pEntityFactory", m_pEntityFactory); \
_pSerializer->SerializeProperty("m_owner", m_owner); \
_pSerializer->SerializeProperty("m_pServer", m_pServer); \
_pSerializer->SerializeProperty("m_pClient", m_pClient); \
_pSerializer->SerializeProperty("m_networkQuery", m_networkQuery); \
_pSerializer->SerializeProperty("m_selectQuery", m_selectQuery); \
_pSerializer->SerializeProperty("m_selectedEntity", m_selectedEntity); \
_pSerializer->SerializeProperty("m_viewportBounds", m_viewportBounds); \
_pSerializer->SerializeProperty("m_bLookingAround", m_bLookingAround); \
_pSerializer->SerializeProperty("m_dDelta", m_dDelta); \
ISystem::Serialize(_pSerializer); \
}
#define Editor_Widgets_ViewportWidget_h_47_GENERATED_BODY \
Editor_Widgets_ViewportWidget_h_47_REFLECTION_MODULE_DUMMY \
Editor_Widgets_ViewportWidget_h_47_REFLECTION_MODULE_CASTER \
Editor_Widgets_ViewportWidget_h_47_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_ViewportWidget_h

