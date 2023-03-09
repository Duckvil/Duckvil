#include "DependencyInjection/IDependencyInjector.h"
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
_namespaces.push_back("DependencyInjection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("IDependencyInjector");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::DependencyInjection;
_type = record_type<Duckvil::DependencyInjection::IDependencyInjector>(_data, "IDependencyInjector");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "DependencyInjection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_destructor = record_destructor<Duckvil::DependencyInjection::IDependencyInjector>(_data, _type);
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, void const*>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Push, "Push");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, int const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Push, "Push");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, size_t const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Push, "Push");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, float const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Push, "Push");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, double const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Push, "Push");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, bool const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Push, "Push");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, void const*>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Call, "Call");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void, void const**>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Call, "Call");
_function = record_function<Duckvil::DependencyInjection::IDependencyInjector, void*>(_data, _type, &Duckvil::DependencyInjection::IDependencyInjector::Execute, "Execute");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "DependencyInjection/IDependencyInjector.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
