#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(9)
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
_namespaces.push_back("ReflectedArgument");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::ReflectedArgument>(_data, "ReflectedArgument");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_destructor = record_destructor<Duckvil::RuntimeReflection::ReflectedArgument>(_data, _type);
_function = record_function<Duckvil::RuntimeReflection::ReflectedArgument, size_t>(_data, _type, &Duckvil::RuntimeReflection::ReflectedArgument::GetTypeID, "GetTypeID");
_namespaces.pop_back();
_namespaces.push_back("ReflectedProperty");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::ReflectedProperty>(_data, "ReflectedProperty");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_destructor = record_destructor<Duckvil::RuntimeReflection::ReflectedProperty>(_data, _type);
_function = record_function<Duckvil::RuntimeReflection::ReflectedProperty, const __duckvil_resource_property_t&>(_data, _type, &Duckvil::RuntimeReflection::ReflectedProperty::GetHandle, "GetHandle");
_function = record_function<Duckvil::RuntimeReflection::ReflectedProperty, size_t>(_data, _type, &Duckvil::RuntimeReflection::ReflectedProperty::GetTypeID, "GetTypeID");
_function = record_function<Duckvil::RuntimeReflection::ReflectedProperty, const char*>(_data, _type, &Duckvil::RuntimeReflection::ReflectedProperty::GetName, "GetName");
_function = record_function<Duckvil::RuntimeReflection::ReflectedProperty, size_t>(_data, _type, &Duckvil::RuntimeReflection::ReflectedProperty::GetAddress, "GetAddress");
_namespaces.pop_back();
_namespaces.push_back("IReflectedFunction");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::IReflectedFunction>(_data, "IReflectedFunction");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_destructor = record_destructor<Duckvil::RuntimeReflection::IReflectedFunction>(_data, _type);
_function = record_function<Duckvil::RuntimeReflection::IReflectedFunction, const __duckvil_resource_function_t&>(_data, _type, &Duckvil::RuntimeReflection::IReflectedFunction::GetHandle, "GetHandle");
_function = record_function<Duckvil::RuntimeReflection::IReflectedFunction, Memory::Vector<ReflectedArgument>, const Memory::FreeList&>(_data, _type, &Duckvil::RuntimeReflection::IReflectedFunction::GetArguments, "GetArguments");
_function = record_function<Duckvil::RuntimeReflection::IReflectedFunction, size_t>(_data, _type, &Duckvil::RuntimeReflection::IReflectedFunction::GetReturnTypeID, "GetReturnTypeID");
_function = record_function<Duckvil::RuntimeReflection::IReflectedFunction, const char*>(_data, _type, &Duckvil::RuntimeReflection::IReflectedFunction::GetName, "GetName");
_function = record_function<Duckvil::RuntimeReflection::IReflectedFunction, const void*>(_data, _type, &Duckvil::RuntimeReflection::IReflectedFunction::GetPointer, "GetPointer");
_function = record_function<Duckvil::RuntimeReflection::IReflectedFunction, size_t>(_data, _type, &Duckvil::RuntimeReflection::IReflectedFunction::GetArgumentsTypeID, "GetArgumentsTypeID");
_namespaces.pop_back();
_namespaces.push_back("ReflectedFunction");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("ReflectedFunction");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("ReflectedFunction");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("ReflectedFunction");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("ReflectedType");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::ReflectedType>(_data, "ReflectedType");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_constructor = record_constructor<Duckvil::RuntimeReflection::ReflectedType, const __duckvil_resource_type_t&>(_data, _type);
_destructor = record_destructor<Duckvil::RuntimeReflection::ReflectedType>(_data, _type);
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, const char*>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetName, "GetName");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, size_t>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetTypeID, "GetTypeID");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, const __duckvil_resource_type_t&>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetHandle, "GetHandle");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, Memory::Vector<ReflectedProperty>, const Memory::FreeList&>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetProperties, "GetProperties");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, Memory::Vector<IReflectedFunction>, const Memory::FreeList&>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetFunctions, "GetFunctions");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, IReflectedFunction, const RuntimeReflection::__duckvil_resource_function_t&>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetFunction, "GetFunction");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, Utils::string>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetFullName, "GetFullName");
_function = record_function<Duckvil::RuntimeReflection::ReflectedType, Memory::Vector<Utils::string>>(_data, _type, &Duckvil::RuntimeReflection::ReflectedType::GetNamespaces, "GetNamespaces");
_namespaces.pop_back();
_namespaces.push_back("Tag");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeReflection/ReflectedType.h", 9);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 9;
}
#endif
