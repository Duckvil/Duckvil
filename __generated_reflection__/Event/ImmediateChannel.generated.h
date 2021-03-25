#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<member_event*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("value", value); \
_pSerializer->SerializeProperty("value", value); \
_pSerializer->SerializeProperty("m_functionHandle", m_functionHandle); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_functionHandle", m_functionHandle); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_aRefelctedEvents", m_aRefelctedEvents); \
_pSerializer->SerializeProperty("m_aTrackedEvents", m_aTrackedEvents); \
_pSerializer->SerializeProperty("m_aCallbackEvents", m_aCallbackEvents); \
_pSerializer->SerializeProperty("m_aMemberEvents", m_aMemberEvents); \
_pSerializer->SerializeProperty("m_pReflectionData", m_pReflectionData); \
_pSerializer->SerializeProperty("m_pReflection", m_pReflection); \
_pSerializer->SerializeProperty("m_mutex", m_mutex); \
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_fn", m_fn); \
}
