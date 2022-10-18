#include "Utils/CommandArgumentsParser.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(2)
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
{
_namespaces.push_back("Utils");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("CommandArgumentsParser");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Utils;
_type = record_type<Duckvil::Utils::CommandArgumentsParser>(_data, "CommandArgumentsParser");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Utils");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_constructor = record_constructor<Duckvil::Utils::CommandArgumentsParser, int, char*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::CommandArgumentsParser, int, const char*>(_data, _type);
_destructor = record_destructor<Duckvil::Utils::CommandArgumentsParser>(_data, _type);
_function = record_function<Duckvil::Utils::CommandArgumentsParser, bool, Descriptor*, std::size_t>(_data, _type, &Duckvil::Utils::CommandArgumentsParser::Parse, "Parse");
_namespaces.pop_back();
_namespaces.push_back("Descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Utils::CommandArgumentsParser::Descriptor>(_data, "Descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Utils");
record_namespace(_data, _type, "CommandArgumentsParser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_constructor = record_constructor<Duckvil::Utils::CommandArgumentsParser::Descriptor>(_data, _type);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Utils::CommandArgumentsParser::Descriptor, m_uiIndex), "m_uiIndex");
_property = record_property<const char*>(_data, _type, offsetof(Duckvil::Utils::CommandArgumentsParser::Descriptor, m_sOption), "m_sOption");
_property = record_property<const char*>(_data, _type, offsetof(Duckvil::Utils::CommandArgumentsParser::Descriptor, m_sResult), "m_sResult");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Utils::CommandArgumentsParser::Descriptor, m_bIsSet), "m_bIsSet");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Utils/CommandArgumentsParser.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
