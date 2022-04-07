#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_ViewportWidget_h_33_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ViewportWidget*>(_pObject); }
#define Editor_Widgets_ViewportWidget_h_33_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_pRenderer", m_pRenderer); \
_pSerializer->SerializeProperty("m_pRendererData", m_pRendererData); \
_pSerializer->SerializeProperty("m_oldSize", m_oldSize); \
_pSerializer->SerializeProperty("m_pWindowEventPool", m_pWindowEventPool); \
_pSerializer->SerializeProperty("m_viewport", m_viewport); \
_pSerializer->SerializeProperty("m_bWrapCamera", m_bWrapCamera); \
_pSerializer->SerializeProperty("m_aKeys", m_aKeys); \
_pSerializer->SerializeProperty("m_bIsWindowFocused", m_bIsWindowFocused); \
_pSerializer->SerializeProperty("m_setMousePosition", m_setMousePosition); \
_pSerializer->SerializeProperty("m_bSkip", m_bSkip); \
_pSerializer->SerializeProperty("m_pECS", m_pECS); \
ISystem::Serialize(_pSerializer); \
}
#define Editor_Widgets_ViewportWidget_h_33_GENERATED_BODY \
Editor_Widgets_ViewportWidget_h_33_REFLECTION_MODULE_CASTER \
Editor_Widgets_ViewportWidget_h_33_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_ViewportWidget_h

