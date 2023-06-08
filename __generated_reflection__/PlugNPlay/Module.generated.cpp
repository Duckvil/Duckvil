#include "PlugNPlay/Module.h"
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
{
_namespaces.push_back("PlugNPlay");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("__module_information");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::PlugNPlay;
_type = record_type<Duckvil::PlugNPlay::__module_information>(_data, "__module_information");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "PlugNPlay");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_constructor = record_constructor<Duckvil::PlugNPlay::__module_information>(_data, _type);
_constructor = record_constructor<Duckvil::PlugNPlay::__module_information, Utils::string const&>(_data, _type);
_constructor = record_constructor<Duckvil::PlugNPlay::__module_information, Utils::string const&, Utils::string const&>(_data, _type);
_destructor = record_destructor<Duckvil::PlugNPlay::__module_information>(_data, _type);
_property = record_property<Utils::string>(_data, _type, offsetof(Duckvil::PlugNPlay::__module_information, m_sName), "m_sName");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::PlugNPlay::__module_information, m_pModule), "m_pModule");
_property = record_property<Utils::string>(_data, _type, offsetof(Duckvil::PlugNPlay::__module_information, m_sPath), "m_sPath");
_namespaces.pop_back();
_namespaces.push_back("__module");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::PlugNPlay::__module>(_data, "__module");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "PlugNPlay");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<bool(*)(__module_information*)>(_data, _type, offsetof(Duckvil::PlugNPlay::__module, load), "load");
_property = record_property<bool(*)(__module_information const&,char const*,void**)>(_data, _type, offsetof(Duckvil::PlugNPlay::__module, get), "get");
_property = record_property<bool(*)(__module_information*)>(_data, _type, offsetof(Duckvil::PlugNPlay::__module, free), "free");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "PlugNPlay/Module.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
