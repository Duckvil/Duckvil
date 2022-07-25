#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_NetworkDebugger_h_19_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_NetworkDebugger_h_19_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<NetworkDebugger*>(_pObject); }
#define Editor_Widgets_NetworkDebugger_h_19_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pClient", m_pClient); \
_pSerializer->SerializeProperty("m_pServer", m_pServer); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_owner", m_owner); \
ISystem::Serialize(_pSerializer); \
}
#define Editor_Widgets_NetworkDebugger_h_19_GENERATED_BODY \
Editor_Widgets_NetworkDebugger_h_19_REFLECTION_MODULE_DUMMY \
Editor_Widgets_NetworkDebugger_h_19_REFLECTION_MODULE_CASTER \
Editor_Widgets_NetworkDebugger_h_19_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_NetworkDebugger_h

