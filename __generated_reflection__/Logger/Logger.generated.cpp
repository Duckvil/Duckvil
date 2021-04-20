#include "Logger/Logger.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil;
_type = record_type<Duckvil::logger_channel_lookup>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "logger_channel_lookup");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<LoggerChannel>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_channel_lookup, m_channel), "m_channel");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_channel_lookup, m_ullTypeID), "m_ullTypeID");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_channel_lookup, m_uiValue), "m_uiValue");
_type = record_type<Duckvil::logger_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "logger_data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<Memory::Vector<logger_channel_lookup>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_data, m_aChannels), "m_aChannels");
_type = record_type<Duckvil::logger_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "logger_ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<logger_data(*)(const Memory::FreeList&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_ftable, m_fnInitLogger), "m_fnInitLogger");
_property = record_property<void(*)(logger_data*, const LoggerChannel&, std::size_t, uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_ftable, m_fnAdd), "m_fnAdd");
_property = record_property<void(*)(logger_data*, std::size_t, uint32_t, uint32_t, const char*, std::size_t, const char*, std::size_t, __logger_channel_verbosity, const va_list&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_ftable, m_fnLog), "m_fnLog");
_type = record_type<Duckvil::logger_context>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "logger_context");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<logger_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_context, m_logger), "m_logger");
_property = record_property<logger_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_context, m_loggerData), "m_loggerData");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_context, m_bInitialized), "m_bInitialized");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Logger/Logger.h", 0 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
