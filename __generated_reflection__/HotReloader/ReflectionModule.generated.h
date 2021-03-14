#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<RuntimeCompilerReflectionModule*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pRuntimeReflection", m_pRuntimeReflection); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_bHasGeneratedBody", m_bHasGeneratedBody); \
_pSerializer->SerializeProperty("m_aVars", m_aVars); \
_pSerializer->SerializeProperty("m_aChildSerializers", m_aChildSerializers); \
}
