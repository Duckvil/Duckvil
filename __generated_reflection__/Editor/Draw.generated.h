#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<Draw*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pTrackKeeper", m_pTrackKeeper); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
}
