#include "DependencyInjection/IDependencyResolver.h"
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
_namespaces.push_back("DependencyInjection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("IDependencyResolver");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::DependencyInjection;
_type = record_type<Duckvil::DependencyInjection::IDependencyResolver>(_data, "IDependencyResolver");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "DependencyInjection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_destructor = record_destructor<Duckvil::DependencyInjection::IDependencyResolver>(_data, _type);
_function = record_function<Duckvil::DependencyInjection::IDependencyResolver, bool, Duckvil::DependencyInjection::IDependencyResolver::Dependency const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyResolver::Register, "Register");
_function = record_function<Duckvil::DependencyInjection::IDependencyResolver, bool, RuntimeReflection::__duckvil_resource_type_t const&, RuntimeReflection::__duckvil_resource_constructor_t const&>(_data, _type, &Duckvil::DependencyInjection::IDependencyResolver::Resolve, "Resolve");
_namespaces.pop_back();
_enum = record_enum<Duckvil::DependencyInjection::IDependencyResolver::Scope>(_data, _ntype, "Scope");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::DependencyInjection::IDependencyResolver::Scope::SINGLETON, "SINGLETON");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::DependencyInjection::IDependencyResolver::Scope::SCOPED, "SCOPED");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::DependencyInjection::IDependencyResolver::Scope::TRANSIENT, "TRANSIENT");
_namespaces.push_back("Dependency");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::DependencyInjection::IDependencyResolver::Dependency>(_data, "Dependency");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "DependencyInjection");
record_namespace(_data, _type, "IDependencyResolver");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<size_t>(_data, _type, offsetof(Duckvil::DependencyInjection::IDependencyResolver::Dependency, m_ullTypeID), "m_ullTypeID");
_property = record_property<Duckvil::DependencyInjection::IDependencyResolver::Scope>(_data, _type, offsetof(Duckvil::DependencyInjection::IDependencyResolver::Dependency, m_scope), "m_scope");
_property = record_property<void const*>(_data, _type, offsetof(Duckvil::DependencyInjection::IDependencyResolver::Dependency, m_pData), "m_pData");
_property = record_property<Duckvil::DependencyInjection::IDependencyResolver::Dependency::OnInjectCallback>(_data, _type, offsetof(Duckvil::DependencyInjection::IDependencyResolver::Dependency, m_fnOnInject), "m_fnOnInject");
_namespaces.pop_back();
_namespaces.push_back("DependencyNotFound");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::DependencyInjection::IDependencyResolver::DependencyNotFound>(_data, "DependencyNotFound");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "DependencyInjection");
record_namespace(_data, _type, "IDependencyResolver");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
record_inheritance<std::exception>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::DependencyInjection::IDependencyResolver::DependencyNotFound>(_data, _type);
_destructor = record_destructor<Duckvil::DependencyInjection::IDependencyResolver::DependencyNotFound>(_data, _type);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "DependencyInjection/IDependencyResolver.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
