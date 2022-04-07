#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_EngineInfoWidget_h_20_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<EngineInfoWidget*>(_pObject); }
#define Editor_Widgets_EngineInfoWidget_h_20_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_pTime", m_pTime); \
_pSerializer->SerializeProperty("m_uiFPS", m_uiFPS); \
_pSerializer->SerializeProperty("m_uiOldFPS", m_uiOldFPS); \
_pSerializer->SerializeProperty("m_dOneSecond", m_dOneSecond); \
}
#define Editor_Widgets_EngineInfoWidget_h_20_GENERATED_BODY \
Editor_Widgets_EngineInfoWidget_h_20_REFLECTION_MODULE_CASTER \
Editor_Widgets_EngineInfoWidget_h_20_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_EngineInfoWidget_h

