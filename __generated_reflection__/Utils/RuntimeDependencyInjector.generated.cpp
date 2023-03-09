#include "Utils/RuntimeDependencyInjector.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
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
_namespaces.push_back("RuntimeDependencyInjector");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::RuntimeDependencyInjector>(_data, "RuntimeDependencyInjector");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_inheritance<Xbyak::CodeGenerator>(_data, _type, __protection::__protection_public);
record_inheritance<DependencyInjection::IDependencyInjector>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeDependencyInjector, uint32_t>(_data, _type);
_destructor = record_destructor<Duckvil::RuntimeDependencyInjector>(_data, _type);
_function = record_function<Duckvil::RuntimeDependencyInjector, void, void const*>(_data, _type, &Duckvil::RuntimeDependencyInjector::Push, "Push");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, int const&>(_data, _type, &Duckvil::RuntimeDependencyInjector::Push, "Push");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, size_t const&>(_data, _type, &Duckvil::RuntimeDependencyInjector::Push, "Push");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, float const&>(_data, _type, &Duckvil::RuntimeDependencyInjector::Push, "Push");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, double const&>(_data, _type, &Duckvil::RuntimeDependencyInjector::Push, "Push");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, bool const&>(_data, _type, &Duckvil::RuntimeDependencyInjector::Push, "Push");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, void const*>(_data, _type, &Duckvil::RuntimeDependencyInjector::Call, "Call");
_function = record_function<Duckvil::RuntimeDependencyInjector, void, void const**>(_data, _type, &Duckvil::RuntimeDependencyInjector::Call, "Call");
_function = record_function<Duckvil::RuntimeDependencyInjector, void*>(_data, _type, &Duckvil::RuntimeDependencyInjector::Execute, "Execute");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Utils/RuntimeDependencyInjector.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
