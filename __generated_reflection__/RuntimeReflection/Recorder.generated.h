#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__recorder_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_aTypes", m_aTypes); \
_pSerializer->SerializeProperty("m_ullCount", m_ullCount); \
_pSerializer->SerializeProperty("m_sFile", m_sFile); \
_pSerializer->SerializeProperty("m_uiRecorderID", m_uiRecorderID); \
_pSerializer->SerializeProperty("m_ullKeyTypeID", m_ullKeyTypeID); \
_pSerializer->SerializeProperty("m_ullKeyTypeSize", m_ullKeyTypeSize); \
_pSerializer->SerializeProperty("m_ucKeyTypeAlignment", m_ucKeyTypeAlignment); \
_pSerializer->SerializeProperty("m_keyTraits", m_keyTraits); \
_pSerializer->SerializeProperty("m_pKeyData", m_pKeyData); \
_pSerializer->SerializeProperty("m_ullValueTypeID", m_ullValueTypeID); \
_pSerializer->SerializeProperty("m_ullValueTypeSize", m_ullValueTypeSize); \
_pSerializer->SerializeProperty("m_ucValueTypeAlignment", m_ucValueTypeAlignment); \
_pSerializer->SerializeProperty("m_valueTraits", m_valueTraits); \
_pSerializer->SerializeProperty("m_pValueData", m_pValueData); \
}
