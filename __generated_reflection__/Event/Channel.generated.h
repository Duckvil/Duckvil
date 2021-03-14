#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<Channel*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_ullMessageTypeID", m_ullMessageTypeID); \
_pSerializer->SerializeProperty("m_mode", m_mode); \
_pSerializer->SerializeProperty("m_pChannel", m_pChannel); \
}
