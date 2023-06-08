#include "Engine/Time.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(12)
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
_namespaces.push_back("__time_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::__time_data>(_data, "__time_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 12);
_property = record_property<Duckvil::__time_data::long_long>(_data, _type, offsetof(Duckvil::__time_data, m_llPreviousTime), "m_llPreviousTime");
_property = record_property<double>(_data, _type, offsetof(Duckvil::__time_data, m_dDelta), "m_dDelta");
_namespaces.pop_back();
_namespaces.push_back("__time_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::__time_ftable>(_data, "__time_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 12);
_property = record_property<bool(*)(__time_data*)>(_data, _type, offsetof(Duckvil::__time_ftable, init), "init");
_property = record_property<void(*)(__time_data*)>(_data, _type, offsetof(Duckvil::__time_ftable, update), "update");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/Time.h", 12);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 12;
}
#endif
