#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<SpecifiedContainer*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_fnCopy", m_fnCopy); \
_pSerializer->SerializeProperty("m_fnDestruct", m_fnDestruct); \
_pSerializer->SerializeProperty("m_pContainer", m_pContainer); \
}
