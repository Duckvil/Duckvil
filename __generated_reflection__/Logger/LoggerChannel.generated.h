#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<LoggerChannel*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_uiLine", m_uiLine); \
_pSerializer->SerializeProperty("m_sFile", m_sFile); \
_pSerializer->SerializeProperty("m_sMessage", m_sMessage); \
_pSerializer->SerializeProperty("m_verbosity", m_verbosity); \
_pSerializer->SerializeProperty("_flags", _flags); \
_pSerializer->SerializeProperty("m_time", m_time); \
_pSerializer->SerializeProperty("m_pOwner", m_pOwner); \
_pSerializer->SerializeProperty("m_logs", m_logs); \
_pSerializer->SerializeProperty("m_llInitTime", m_llInitTime); \
_pSerializer->SerializeProperty("m_buffer", m_buffer); \
_pSerializer->SerializeProperty("m_sPathFile", m_sPathFile); \
_pSerializer->SerializeProperty("m_lastTime", m_lastTime); \
_pSerializer->SerializeProperty("m_flags", m_flags); \
_pSerializer->SerializeProperty("m_pLogEventPool", m_pLogEventPool); \
_pSerializer->SerializeProperty("m_pLogger", m_pLogger); \
_pSerializer->SerializeProperty("m_pLoggerData", m_pLoggerData); \
}
