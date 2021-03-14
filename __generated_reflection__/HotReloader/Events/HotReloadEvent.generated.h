#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<HotReloadedEvent*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_pOldObject", m_pOldObject); \
_pSerializer->SerializeProperty("m_pTrackKeeper", m_pTrackKeeper); \
_pSerializer->SerializeProperty("m_stage", m_stage); \
_pSerializer->SerializeProperty("_typeHandle", _typeHandle); \
}
