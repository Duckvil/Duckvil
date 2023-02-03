#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define CSharp_Entity_h_31_REFLECTION_MODULE_DUMMY 
#define CSharp_Entity_h_31_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<Script*>(_pObject); }
#define CSharp_Entity_h_31_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_pClass", m_pClass); \
_pSerializer->SerializeProperty("m_pInit", m_pInit); \
_pSerializer->SerializeProperty("m_pUpdate", m_pUpdate); \
_pSerializer->SerializeProperty("m_aFields", m_aFields); \
_pSerializer->SerializeProperty("m_aProperties", m_aProperties); \
}
#define CSharp_Entity_h_31_GENERATED_BODY \
CSharp_Entity_h_31_REFLECTION_MODULE_DUMMY \
CSharp_Entity_h_31_REFLECTION_MODULE_CASTER \
CSharp_Entity_h_31_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID CSharp_Entity_h

