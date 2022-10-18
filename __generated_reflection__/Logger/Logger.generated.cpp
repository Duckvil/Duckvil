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
_enum = record_enum<Duckvil::LoggerChannelID>(_data, _ntype, "LoggerChannelID");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::LoggerChannelID::Default, "Default");
_namespaces.push_back("logger_channel_lookup");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::logger_channel_lookup>(_data, "logger_channel_lookup");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<__logger_channel_data*>(_data, _type, offsetof(Duckvil::logger_channel_lookup, m_pChannel), "m_pChannel");
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::logger_channel_lookup, m_ullTypeID), "m_ullTypeID");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::logger_channel_lookup, m_uiValue), "m_uiValue");
_namespaces.pop_back();
_namespaces.push_back("logger_channel_descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::logger_channel_descriptor>(_data, "logger_channel_descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<const char*>(_data, _type, offsetof(Duckvil::logger_channel_descriptor, m_sPathFile), "m_sPathFile");
_property = record_property<size_t>(_data, _type, offsetof(Duckvil::logger_channel_descriptor, m_ullLength), "m_ullLength");
_property = record_property<__logger_channel_flags>(_data, _type, offsetof(Duckvil::logger_channel_descriptor, m_flags), "m_flags");
_namespaces.pop_back();
_namespaces.push_back("logger_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::logger_data>(_data, "logger_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<Memory::Vector<logger_channel_lookup>>(_data, _type, offsetof(Duckvil::logger_data, m_aChannels), "m_aChannels");
_property = record_property<Memory::Vector<__logger_channel_data*>>(_data, _type, offsetof(Duckvil::logger_data, _loggers), "_loggers");
_namespaces.pop_back();
_namespaces.push_back("logger_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::logger_ftable>(_data, "logger_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<logger_data(*)(const Memory::FreeList&)>(_data, _type, offsetof(Duckvil::logger_ftable, m_fnInitLogger), "m_fnInitLogger");
_property = record_property<__logger_channel_data*(*)(const Memory::FreeList&, logger_data*, const logger_channel_descriptor&, std::size_t, uint32_t)>(_data, _type, offsetof(Duckvil::logger_ftable, m_fnAdd), "m_fnAdd");
_property = record_property<void(*)(__logger_channel_ftable*, logger_data*, std::size_t, uint32_t, uint32_t, const char*, std::size_t, const char*, std::size_t, __logger_channel_verbosity, va_list)>(_data, _type, offsetof(Duckvil::logger_ftable, m_fnLog), "m_fnLog");
_namespaces.pop_back();
_namespaces.push_back("logger_context");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::logger_context>(_data, "logger_context");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<logger_ftable>(_data, _type, offsetof(Duckvil::logger_context, m_logger), "m_logger");
_property = record_property<logger_data>(_data, _type, offsetof(Duckvil::logger_context, m_loggerData), "m_loggerData");
_property = record_property<__logger_channel_ftable*>(_data, _type, offsetof(Duckvil::logger_context, m_pLoggerChannel), "m_pLoggerChannel");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::logger_context, m_bInitialized), "m_bInitialized");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Logger/Logger.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
