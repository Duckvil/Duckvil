#include "DependencyInjection/DependencyInjection.h"
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
{
_namespaces.push_back("DependencyInjection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("DependencyResolver");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::DependencyInjection;
_type = record_type<Duckvil::DependencyInjection::DependencyResolver>(_data, "DependencyResolver");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "DependencyInjection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<IDependencyResolver>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::DependencyInjection::DependencyResolver, Memory::FreeList const&, Event::Pool<Event::mode::immediate>*>(_data, _type);
_destructor = record_destructor<Duckvil::DependencyInjection::DependencyResolver>(_data, _type);
_function = record_function<Duckvil::DependencyInjection::DependencyResolver, bool, Duckvil::DependencyInjection::IDependencyResolver::Dependency const&>(_data, _type, &Duckvil::DependencyInjection::DependencyResolver::Register, "Register");
_function = record_function<Duckvil::DependencyInjection::DependencyResolver, bool, RuntimeReflection::__duckvil_resource_type_t const&, RuntimeReflection::__duckvil_resource_constructor_t const&>(_data, _type, &Duckvil::DependencyInjection::DependencyResolver::Resolve, "Resolve");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "DependencyInjection/DependencyInjection.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
