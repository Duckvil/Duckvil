#include "Engine/ReflectionModules/NamedArgumentsReflectionModule.h"
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
_namespaces.push_back("NamedArgumentsReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("NamedArgumentsReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::NamedArgumentsReflectionModule>(_data, "NamedArgumentsReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
record_meta(_data, _type, ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_property = record_property<char const*>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_sReflectionModuleName), "m_sReflectionModuleName");
_constructor = record_constructor<Duckvil::NamedArgumentsReflectionModule, Memory::FreeList const&, RuntimeReflection::__ftable const*, RuntimeReflection::__data*>(_data, _type);
_destructor = record_destructor<Duckvil::NamedArgumentsReflectionModule>(_data, _type);
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_heap), "m_heap");
_property = record_property<RuntimeReflection::__ftable const*>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_pReflection), "m_pReflection");
_property = record_property<RuntimeReflection::__data*>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_pRuntimeReflectionData), "m_pRuntimeReflectionData");
_property = record_property<std::filesystem::path>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_sFile), "m_sFile");
_property = record_property<std::vector<NamedArgumentsReflectionModule::Context *>>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_aContexts), "m_aContexts");
_property = record_property<NamedArgumentsReflectionModule::Context*>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule, m_pCurrentContext), "m_pCurrentContext");
_function = record_function<Duckvil::NamedArgumentsReflectionModule, void, Parser::__ast*>(_data, _type, &Duckvil::NamedArgumentsReflectionModule::ProcessAST, "ProcessAST");
_function = record_function<Duckvil::NamedArgumentsReflectionModule, void, std::ofstream&, std::ofstream&, RuntimeReflection::GeneratedVector&>(_data, _type, &Duckvil::NamedArgumentsReflectionModule::GenerateCustom, "GenerateCustom");
_function = record_function<Duckvil::NamedArgumentsReflectionModule, void>(_data, _type, &Duckvil::NamedArgumentsReflectionModule::Clear, "Clear");
_namespaces.pop_back();
_namespaces.push_back("argument");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::NamedArgumentsReflectionModule::argument>(_data, "argument");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "NamedArgumentsReflectionModule");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::argument, m_sName), "m_sName");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::argument, m_sType), "m_sType");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::argument, m_sValue), "m_sValue");
_namespaces.pop_back();
_namespaces.push_back("function");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::NamedArgumentsReflectionModule::function>(_data, "function");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "NamedArgumentsReflectionModule");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::function, m_sName), "m_sName");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::function, m_sReturnType), "m_sReturnType");
_property = record_property<std::vector<NamedArgumentsReflectionModule::argument>>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::function, m_aArguments), "m_aArguments");
_namespaces.pop_back();
_namespaces.push_back("Context");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::NamedArgumentsReflectionModule::Context>(_data, "Context");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "NamedArgumentsReflectionModule");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::Context, m_uiGeneratedBodyLine), "m_uiGeneratedBodyLine");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::Context, m_bIsStruct), "m_bIsStruct");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::Context, m_sTypeName), "m_sTypeName");
_property = record_property<std::vector<NamedArgumentsReflectionModule::Context *>>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::Context, m_aChildren), "m_aChildren");
_property = record_property<NamedArgumentsReflectionModule::Context*>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::Context, m_pParent), "m_pParent");
_property = record_property<std::vector<NamedArgumentsReflectionModule::function>>(_data, _type, offsetof(Duckvil::NamedArgumentsReflectionModule::Context, m_aFunctions), "m_aFunctions");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/ReflectionModules/NamedArgumentsReflectionModule.h", 9);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 9;
}
#endif
