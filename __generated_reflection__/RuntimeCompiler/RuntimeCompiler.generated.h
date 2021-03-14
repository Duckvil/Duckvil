#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<Compiler*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pCompiler", m_pCompiler); \
_pSerializer->SerializeProperty("m_processFTable", m_processFTable); \
_pSerializer->SerializeProperty("m_processData", m_processData); \
}
