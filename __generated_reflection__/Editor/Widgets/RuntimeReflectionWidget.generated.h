#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_RuntimeReflectionWidget_h_20_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_RuntimeReflectionWidget_h_20_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<RuntimeReflectionWidget*>(_pObject); }
#define Editor_Widgets_RuntimeReflectionWidget_h_20_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_aTypes", m_aTypes); \
_pSerializer->SerializeProperty("m_uiCurrentItem", m_uiCurrentItem); \
_pSerializer->SerializeProperty("m_uiCurrentFunctionItem", m_uiCurrentFunctionItem); \
_pSerializer->SerializeProperty("m_uiCurrentPropertyItem", m_uiCurrentPropertyItem); \
}
#define Editor_Widgets_RuntimeReflectionWidget_h_20_GENERATED_BODY \
Editor_Widgets_RuntimeReflectionWidget_h_20_REFLECTION_MODULE_DUMMY \
Editor_Widgets_RuntimeReflectionWidget_h_20_REFLECTION_MODULE_CASTER \
Editor_Widgets_RuntimeReflectionWidget_h_20_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_RuntimeReflectionWidget_h

