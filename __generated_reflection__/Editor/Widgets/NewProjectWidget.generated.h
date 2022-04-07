#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_NewProjectWidget_h_29_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<NewProjectWidget*>(_pObject); }
#define Editor_Widgets_NewProjectWidget_h_29_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_sProjectName", m_sProjectName); \
_pSerializer->SerializeProperty("m_iSelectedType", m_iSelectedType); \
_pSerializer->SerializeProperty("m_pProjectManager", m_pProjectManager); \
_pSerializer->SerializeProperty("m_pProjectManagerData", m_pProjectManagerData); \
ISystem::Serialize(_pSerializer); \
}
#define Editor_Widgets_NewProjectWidget_h_29_GENERATED_BODY \
Editor_Widgets_NewProjectWidget_h_29_REFLECTION_MODULE_CASTER \
Editor_Widgets_NewProjectWidget_h_29_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_NewProjectWidget_h

