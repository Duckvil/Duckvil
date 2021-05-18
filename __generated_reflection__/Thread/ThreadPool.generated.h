#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<pool_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_callback", m_callback); \
_pSerializer->SerializeProperty("m_pData", m_pData); \
}
