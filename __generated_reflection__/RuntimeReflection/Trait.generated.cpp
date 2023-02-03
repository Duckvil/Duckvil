#include "RuntimeReflection/Trait.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(13)
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
_namespaces.push_back("RuntimeReflection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::RuntimeReflection::property_traits>(_data, _ntype, "property_traits");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_pointer, "is_pointer");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_reference, "is_reference");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_array, "is_array");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_void, "is_void");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_integral, "is_integral");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_floating_point, "is_floating_point");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_enum, "is_enum");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_bool, "is_bool");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_const, "is_const");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::property_traits::is_static, "is_static");
_enum = record_enum<Duckvil::RuntimeReflection::function_traits>(_data, _ntype, "function_traits");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::function_traits::is_static, "is_static");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::function_traits::is_const, "is_const");
{
_namespaces.push_back("traits");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("function");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_const");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.pop_back();
}
{
_namespaces.push_back("property");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("is_bool");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeReflection/Trait.h", 13);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 13;
}
#endif
