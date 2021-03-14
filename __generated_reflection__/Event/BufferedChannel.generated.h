#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<Channel*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_uiIndex", m_uiIndex); \
_pSerializer->SerializeProperty("m_uiEventsCount", m_uiEventsCount); \
_pSerializer->SerializeProperty("m_aMessages", m_aMessages); \
_pSerializer->SerializeProperty("m_pReflectionData", m_pReflectionData); \
}
