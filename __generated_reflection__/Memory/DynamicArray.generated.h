#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__duckvil_dynamic_array*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_ullSize", m_ullSize); \
_pSerializer->SerializeProperty("m_ullCapacity", m_ullCapacity); \
}
