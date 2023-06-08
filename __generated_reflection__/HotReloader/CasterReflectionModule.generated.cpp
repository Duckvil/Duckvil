#include "HotReloader/CasterReflectionModule.h"
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
_namespaces.push_back("CasterReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("CasterReflectionModule");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::CasterReflectionModule>(_data, "CasterReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_meta(_data, _type, ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_property = record_property<char const*>(_data, _type, offsetof(Duckvil::CasterReflectionModule, m_sReflectionModuleName), "m_sReflectionModuleName");
_constructor = record_constructor<Duckvil::CasterReflectionModule, Memory::FreeList const&, RuntimeReflection::__ftable const*, RuntimeReflection::__data*>(_data, _type);
_destructor = record_destructor<Duckvil::CasterReflectionModule>(_data, _type);
_property = record_property<std::filesystem::path>(_data, _type, offsetof(Duckvil::CasterReflectionModule, m_sFile), "m_sFile");
_property = record_property<std::vector<CasterReflectionModule::Context *>>(_data, _type, offsetof(Duckvil::CasterReflectionModule, m_aContexts), "m_aContexts");
_property = record_property<CasterReflectionModule::Context*>(_data, _type, offsetof(Duckvil::CasterReflectionModule, m_pCurrentContext), "m_pCurrentContext");
_function = record_function<Duckvil::CasterReflectionModule, void, Parser::__ast*>(_data, _type, &Duckvil::CasterReflectionModule::ProcessAST, "ProcessAST");
_function = record_function<Duckvil::CasterReflectionModule, void, std::ofstream&, std::ofstream&, RuntimeReflection::GeneratedVector&>(_data, _type, &Duckvil::CasterReflectionModule::GenerateCustom, "GenerateCustom");
_function = record_function<Duckvil::CasterReflectionModule, void>(_data, _type, &Duckvil::CasterReflectionModule::Clear, "Clear");
_namespaces.pop_back();
_namespaces.push_back("Context");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::CasterReflectionModule::Context>(_data, "Context");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "CasterReflectionModule");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::CasterReflectionModule::Context, m_uiGeneratedBodyLine), "m_uiGeneratedBodyLine");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::CasterReflectionModule::Context, m_bIsStruct), "m_bIsStruct");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::CasterReflectionModule::Context, m_sTypeName), "m_sTypeName");
_property = record_property<std::vector<CasterReflectionModule::Context *>>(_data, _type, offsetof(Duckvil::CasterReflectionModule::Context, m_aChildren), "m_aChildren");
_property = record_property<CasterReflectionModule::Context*>(_data, _type, offsetof(Duckvil::CasterReflectionModule::Context, m_pParent), "m_pParent");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/CasterReflectionModule.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
