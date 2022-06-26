#include "Serializer/Runtime/ISerializer.h"
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
_namespaces.push_back("RuntimeSerializer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("ISerializer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeSerializer;
_type = record_type<Duckvil::RuntimeSerializer::ISerializer>(_data, "ISerializer");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeSerializer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_destructor = record_destructor<Duckvil::RuntimeSerializer::ISerializer>(_data, _type);
_function = record_function<Duckvil::RuntimeSerializer::ISerializer, void>(_data, _type, &Duckvil::RuntimeSerializer::ISerializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::ISerializer, void, uint32_t>(_data, _type, &Duckvil::RuntimeSerializer::ISerializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::ISerializer, void, uint32_t, const char*>(_data, _type, &Duckvil::RuntimeSerializer::ISerializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::ISerializer, bool>(_data, _type, &Duckvil::RuntimeSerializer::ISerializer::IsLoading, "IsLoading");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Serializer/Runtime/ISerializer.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
