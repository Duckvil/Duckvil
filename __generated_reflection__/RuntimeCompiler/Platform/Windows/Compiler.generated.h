#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<WindowsCompiler*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_compilerOptions", m_compilerOptions); \
_pSerializer->SerializeProperty("m_processFTable", m_processFTable); \
_pSerializer->SerializeProperty("m_pProcessData", m_pProcessData); \
}
