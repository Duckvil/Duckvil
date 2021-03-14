#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pReflection", m_pReflection); \
_pSerializer->SerializeProperty("m_pReflectionData", m_pReflectionData); \
_pSerializer->SerializeProperty("m_pRecorder", m_pRecorder); \
_pSerializer->SerializeProperty("m_ullSize", m_ullSize); \
_pSerializer->SerializeProperty("m_ullTypeID", m_ullTypeID); \
_pSerializer->SerializeProperty("m_pData", m_pData); \
_pSerializer->SerializeProperty("m_traits", m_traits); \
_pSerializer->SerializeProperty("m_aTypes", m_aTypes); \
_pSerializer->SerializeProperty("m_pEvents", m_pEvents); \
}
