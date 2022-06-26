#include "Serializer/Runtime/Serializer.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
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
_namespaces.push_back("RuntimeSerializer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Serializer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeSerializer;
_type = record_type<Duckvil::RuntimeSerializer::Serializer>(_data, "Serializer");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeSerializer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
record_inheritance<ISerializer>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeSerializer::Serializer>(_data, _type);
_destructor = record_destructor<Duckvil::RuntimeSerializer::Serializer>(_data, _type);
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, void*, RuntimeReflection::__function<voidHotReloader::HotObject::*RuntimeSerializer::ISerializer*>*>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::Serialize, "Serialize");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, void*, void*>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::Serialize, "Serialize");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, uint32_t>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, uint32_t, const char*>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, bool>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::IsLoading, "IsLoading");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, bool>(_data, _type, &Duckvil::RuntimeSerializer::Serializer::SetLoading, "SetLoading");
_namespaces.pop_back();
_namespaces.push_back("cmp_str");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeSerializer::Serializer::cmp_str>(_data, "cmp_str");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeSerializer");
record_namespace(_data, _type, "Serializer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Serializer/Runtime/Serializer.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
