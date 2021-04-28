#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<logger_context*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pChannel", m_pChannel); \
_pSerializer->SerializeProperty("m_ullTypeID", m_ullTypeID); \
_pSerializer->SerializeProperty("m_uiValue", m_uiValue); \
_pSerializer->SerializeProperty("m_sPathFile", m_sPathFile); \
_pSerializer->SerializeProperty("m_ullLength", m_ullLength); \
_pSerializer->SerializeProperty("m_flags", m_flags); \
_pSerializer->SerializeProperty("m_aChannels", m_aChannels); \
_pSerializer->SerializeProperty("_loggers", _loggers); \
_pSerializer->SerializeProperty("m_logger", m_logger); \
_pSerializer->SerializeProperty("m_loggerData", m_loggerData); \
_pSerializer->SerializeProperty("m_pLoggerChannel", m_pLoggerChannel); \
_pSerializer->SerializeProperty("m_bInitialized", m_bInitialized); \
}
