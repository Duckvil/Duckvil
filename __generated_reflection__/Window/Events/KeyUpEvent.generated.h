#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<KeyUpEvent*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_key", m_key); \
}