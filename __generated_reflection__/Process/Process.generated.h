#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_bComplete", m_bComplete); \
_pSerializer->SerializeProperty("m_bRunning", m_bRunning); \
_pSerializer->SerializeProperty("m_pImplementationData", m_pImplementationData); \
}
