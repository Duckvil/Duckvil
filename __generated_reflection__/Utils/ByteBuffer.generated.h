#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<duckvil_byte_buffer_t*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("data", data); \
_pSerializer->SerializeProperty("size", size); \
_pSerializer->SerializeProperty("position", position); \
_pSerializer->SerializeProperty("capacity", capacity); \
}
