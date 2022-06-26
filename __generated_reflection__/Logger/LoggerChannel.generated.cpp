#include "Logger/LoggerChannel.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::__logger_channel_verbosity>(_data, _ntype, "__logger_channel_verbosity");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_verbosity::__verbosity_info, "__verbosity_info");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_verbosity::__verbosity_warning, "__verbosity_warning");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_verbosity::__verbosity_error, "__verbosity_error");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_verbosity::__verbosity_fatal, "__verbosity_fatal");
_enum = record_enum<Duckvil::__logger_channel_log_flags>(_data, _ntype, "__logger_channel_log_flags");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_log_flags::__flags_immediate_log, "__flags_immediate_log");
_enum = record_enum<Duckvil::__logger_channel_flags>(_data, _ntype, "__logger_channel_flags");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_flags::__logger_flags_console_output, "__logger_flags_console_output");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::__logger_channel_flags::__logger_flags_editor_console_output, "__logger_flags_editor_console_output");
_namespaces.push_back("__logger_channel_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::__logger_channel_data>(_data, "__logger_channel_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_namespaces.pop_back();
_namespaces.push_back("__logger_channel_log_info");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::__logger_channel_log_info>(_data, "__logger_channel_log_info");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_constructor = record_constructor<Duckvil::__logger_channel_log_info>(_data, _type);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_uiLine), "m_uiLine");
_property = record_property<char>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_sFile), "m_sFile");
_property = record_property<char>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_sMessage), "m_sMessage");
_property = record_property<__logger_channel_verbosity>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_verbosity), "m_verbosity");
_property = record_property<__logger_channel_log_flags>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_flags), "m_flags");
_property = record_property<std::time_t>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_time), "m_time");
_property = record_property<__logger_channel_data*>(_data, _type, offsetof(Duckvil::__logger_channel_log_info, m_pOwner), "m_pOwner");
_namespaces.pop_back();
_namespaces.push_back("__logger_channel_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::__logger_channel_data>(_data, "__logger_channel_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<char>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_buffer), "m_buffer");
_property = record_property<char>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_sPathFile), "m_sPathFile");
_property = record_property<Memory::ThreadsafeQueue<__logger_channel_log_info>>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_logs), "m_logs");
_property = record_property<Event::Pool<Event::mode::immediate>*>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_pLogEventPool), "m_pLogEventPool");
_property = record_property<int64_t>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_llInitTime), "m_llInitTime");
_property = record_property<time_t>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_lastTime), "m_lastTime");
_property = record_property<__logger_channel_flags>(_data, _type, offsetof(Duckvil::__logger_channel_data, m_flags), "m_flags");
_namespaces.pop_back();
_namespaces.push_back("__logger_channel_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::__logger_channel_ftable>(_data, "__logger_channel_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<__logger_channel_data*(*)(const Memory::FreeList&)>(_data, _type, offsetof(Duckvil::__logger_channel_ftable, init), "init");
_property = record_property<void(*)(__logger_channel_ftable*, __logger_channel_data*, const __logger_channel_log_info&)>(_data, _type, offsetof(Duckvil::__logger_channel_ftable, log), "log");
_property = record_property<void(*)(__logger_channel_data*, const __logger_channel_log_info&, char*)>(_data, _type, offsetof(Duckvil::__logger_channel_ftable, format), "format");
_property = record_property<void(*)(__logger_channel_ftable*, __logger_channel_data*)>(_data, _type, offsetof(Duckvil::__logger_channel_ftable, dispatch_logs), "dispatch_logs");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Logger/LoggerChannel.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
