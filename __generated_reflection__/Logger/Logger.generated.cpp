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
_type = record_type<Duckvil::__logger_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__logger_data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_type = record_type<Duckvil::__logger_log_info>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__logger_log_info");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_constructor = record_constructor<Duckvil::__logger_log_info>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, m_uiLine), "m_uiLine");
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, m_sFile), "m_sFile");
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, m_sMessage), "m_sMessage");
_property = record_property<__logger_verbosity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, m_verbosity), "m_verbosity");
_property = record_property<__logger_log_flags>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, _flags), "_flags");
_property = record_property<std::time_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, m_time), "m_time");
_property = record_property<__logger_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_log_info, m_pOwner), "m_pOwner");
_type = record_type<Duckvil::__logger_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__logger_data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<Memory::Queue<__logger_log_info>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_logs), "m_logs");
_property = record_property<int64_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_llInitTime), "m_llInitTime");
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_buffer), "m_buffer");
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_sPathFile), "m_sPathFile");
_property = record_property<time_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_lastTime), "m_lastTime");
_property = record_property<__logger_flags>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_flags), "m_flags");
_property = record_property<Event::Pool<Event::mode::immediate>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_data, m_pLogEventPool), "m_pLogEventPool");
_type = record_type<Duckvil::__logger_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__logger_ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<__logger_data*(*)(Memory::IMemory*, Memory::__free_list_allocator*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_ftable, init), "init");
_property = record_property<void(*)(__logger_ftable*, __logger_data*, __logger_log_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_ftable, log), "log");
_property = record_property<void(*)(__logger_data*, const __logger_log_info&, char*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_ftable, format), "format");
_property = record_property<void(*)(__logger_ftable*, __logger_data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__logger_ftable, dispatch_logs), "dispatch_logs");
_type = record_type<Duckvil::logger_context>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "logger_context");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_constructor = record_constructor<Duckvil::logger_context>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::logger_context, __logger_ftable*, __logger_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<__logger_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_context, m_pLogger), "m_pLogger");
_property = record_property<__logger_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::logger_context, m_pLoggerData), "m_pLoggerData");
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
