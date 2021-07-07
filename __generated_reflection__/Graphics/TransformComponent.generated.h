#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<TransformComponent*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_position", m_position); \
_pSerializer->SerializeProperty("m_rotation", m_rotation); \
_pSerializer->SerializeProperty("m_scale", m_scale); \
}
