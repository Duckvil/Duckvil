#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__duckvil_slot_array_base*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_handleIndices", m_handleIndices); \
_pSerializer->SerializeProperty("m_freeIndices", m_freeIndices); \
}
