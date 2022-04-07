#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_LoadProjectWidget_h_25_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<LoadProjectWidget*>(_pObject); }
#define Editor_Widgets_LoadProjectWidget_h_25_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_iSelectedProject", m_iSelectedProject); \
_pSerializer->SerializeProperty("m_aProjects", m_aProjects); \
}
#define Editor_Widgets_LoadProjectWidget_h_25_GENERATED_BODY \
Editor_Widgets_LoadProjectWidget_h_25_REFLECTION_MODULE_CASTER \
Editor_Widgets_LoadProjectWidget_h_25_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_LoadProjectWidget_h

