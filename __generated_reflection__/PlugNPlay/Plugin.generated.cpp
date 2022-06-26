#include "PlugNPlay/Plugin.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
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
_enum = record_enum<Duckvil::PlugNPlay::__instantiation>(_data, _ntype, "__instantiation");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::PlugNPlay::__instantiation::SINGLE, "SINGLE");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::PlugNPlay::__instantiation::MANY, "MANY");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::PlugNPlay::__instantiation::NONE, "NONE");
_namespaces.push_back("__plugin_details");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::PlugNPlay;
_type = record_type<Duckvil::PlugNPlay::__plugin_details>(_data, "__plugin_details");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "PlugNPlay");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<const char*>(_data, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_sPluginName), "m_sPluginName");
_property = record_property<allocator_callback>(_data, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_pAllocator), "m_pAllocator");
_property = record_property<instance_callback>(_data, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_pInstance), "m_pInstance");
_property = record_property<free_callback>(_data, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_pDeallocator), "m_pDeallocator");
_property = record_property<__instantiation>(_data, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_instantiation), "m_instantiation");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "PlugNPlay/Plugin.h", 4);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
