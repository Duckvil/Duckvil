#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<logger_context*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_channel", m_channel); \
_pSerializer->SerializeProperty("m_ullTypeID", m_ullTypeID); \
_pSerializer->SerializeProperty("m_uiValue", m_uiValue); \
_pSerializer->SerializeProperty("m_aChannels", m_aChannels); \
_pSerializer->SerializeProperty("m_logger", m_logger); \
_pSerializer->SerializeProperty("m_loggerData", m_loggerData); \
_pSerializer->SerializeProperty("m_bInitialized", m_bInitialized); \
}
