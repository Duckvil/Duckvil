#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define TestSystem_TestSystem_h_53_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<Test*>(_pObject); }
#define TestSystem_TestSystem_h_53_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_iA", m_iA); \
ISystem::Serialize(_pSerializer); \
}
#define TestSystem_TestSystem_h_53_GENERATED_BODY \
TestSystem_TestSystem_h_53_REFLECTION_MODULE_CASTER \
TestSystem_TestSystem_h_53_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID TestSystem_TestSystem_h

