#include "HotReloader/ReflectionModule.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(10)
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
_namespaces.push_back("HotReloader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("RuntimeCompilerReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("RuntimeCompilerReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::RuntimeCompilerReflectionModule>(_data, "RuntimeCompilerReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
record_meta(_data, _type, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_property = record_property<char const*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_sReflectionModuleName), "m_sReflectionModuleName");
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerReflectionModule, Memory::FreeList const&, RuntimeReflection::__ftable const*, RuntimeReflection::__data*>(_data, _type);
_destructor = record_destructor<Duckvil::HotReloader::RuntimeCompilerReflectionModule>(_data, _type);
_property = record_property<RuntimeReflection::__data*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pRuntimeReflectionData), "m_pRuntimeReflectionData");
_property = record_property<RuntimeReflection::__ftable const*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pRuntimeReflection), "m_pRuntimeReflection");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_heap), "m_heap");
_property = record_property<std::filesystem::path>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_sFile), "m_sFile");
_property = record_property<std::vector<RuntimeCompilerReflectionModule::Context *>>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_aContexts), "m_aContexts");
_property = record_property<RuntimeCompilerReflectionModule::Context*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pCurrentContext), "m_pCurrentContext");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void, Parser::__ast*>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::ProcessAST, "ProcessAST");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void, std::ofstream&, std::ofstream&, RuntimeReflection::GeneratedVector&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::GenerateCustom, "GenerateCustom");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::Clear, "Clear");
_namespaces.pop_back();
_namespaces.push_back("Context");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context>(_data, "Context");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_namespace(_data, _type, "RuntimeCompilerReflectionModule");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_uiGeneratedBodyLine), "m_uiGeneratedBodyLine");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_aVars), "m_aVars");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_aChildSerializers), "m_aChildSerializers");
_property = record_property<std::vector<RuntimeCompilerReflectionModule::Context *>>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_aChildren), "m_aChildren");
_property = record_property<RuntimeCompilerReflectionModule::Context*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_pParent), "m_pParent");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/ReflectionModule.h", 10);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 10;
}
#endif
