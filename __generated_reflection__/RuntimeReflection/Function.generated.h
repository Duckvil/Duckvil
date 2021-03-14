#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__function*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_fnFunction", m_fnFunction); \
_pSerializer->SerializeProperty("m_fnFunction", m_fnFunction); \
_pSerializer->SerializeProperty("m_fnFunction", m_fnFunction); \
_pSerializer->SerializeProperty("m_fnFunction", m_fnFunction); \
_pSerializer->SerializeProperty("m_fnFunction", m_fnFunction); \
_pSerializer->SerializeProperty("m_fnFunction", m_fnFunction); \
}
