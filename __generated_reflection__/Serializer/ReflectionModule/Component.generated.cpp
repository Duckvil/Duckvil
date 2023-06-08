#include "Serializer/ReflectionModule/Component.h"
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
_namespaces.push_back("Serializer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("ReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Component");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("Component");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Serializer::ReflectionModule;
_type = record_type<Duckvil::Serializer::ReflectionModule::Component>(_data, "Component");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Serializer");
record_namespace(_data, _type, "ReflectionModule");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
record_meta(_data, _type, ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_property = record_property<char const*>(_data, _type, offsetof(Duckvil::Serializer::ReflectionModule::Component, m_sReflectionModuleName), "m_sReflectionModuleName");
_constructor = record_constructor<Duckvil::Serializer::ReflectionModule::Component, Memory::FreeList const&, RuntimeReflection::__ftable const*, RuntimeReflection::__data*>(_data, _type);
_destructor = record_destructor<Duckvil::Serializer::ReflectionModule::Component>(_data, _type);
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Serializer::ReflectionModule::Component, m_bIsValid), "m_bIsValid");
_property = record_property<std::filesystem::path>(_data, _type, offsetof(Duckvil::Serializer::ReflectionModule::Component, m_sFile), "m_sFile");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Serializer::ReflectionModule::Component, m_uiGeneratedBodyLine), "m_uiGeneratedBodyLine");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Serializer::ReflectionModule::Component, m_sTypeName), "m_sTypeName");
_function = record_function<Duckvil::Serializer::ReflectionModule::Component, void, Parser::__ast*>(_data, _type, &Duckvil::Serializer::ReflectionModule::Component::ProcessAST, "ProcessAST");
_function = record_function<Duckvil::Serializer::ReflectionModule::Component, void, std::ofstream&, std::ofstream&, RuntimeReflection::GeneratedVector&>(_data, _type, &Duckvil::Serializer::ReflectionModule::Component::GenerateCustom, "GenerateCustom");
_function = record_function<Duckvil::Serializer::ReflectionModule::Component, void>(_data, _type, &Duckvil::Serializer::ReflectionModule::Component::Clear, "Clear");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Serializer/ReflectionModule/Component.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
