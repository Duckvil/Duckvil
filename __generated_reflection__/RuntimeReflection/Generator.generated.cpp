#include "RuntimeReflection/Generator.h"
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
_namespaces.push_back("RuntimeReflection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("__generator_namespace");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__generator_namespace>(_data, "__generator_namespace");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<char[32]>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_namespace, m_sName), "m_sName");
_property = record_property<std::vector<Parser::__ast_meta>>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_namespace, m_aMeta), "m_aMeta");
_namespaces.pop_back();
_namespaces.push_back("__generator_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__generator_data>(_data, "__generator_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<std::deque<__generator_namespace>>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_data, m_aNamespaces), "m_aNamespaces");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_data, m_bWasNamespaces), "m_bWasNamespaces");
_property = record_property<char[256]>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_data, m_sInclude), "m_sInclude");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_data, m_uiRecorderIndex), "m_uiRecorderIndex");
_namespaces.pop_back();
_namespaces.push_back("__generator_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__generator_ftable>(_data, "__generator_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<__generator_data*(*)(Memory::ftable*,Memory::free_list_allocator*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_ftable, init), "init");
_property = record_property<void(*)(__generator_data*,char const[256],char const[256],Parser::__ast const&,void(*)(std::ofstream&,std::ofstream&,void*,Duckvil::RuntimeReflection::__ftable const*),void*,Duckvil::RuntimeReflection::__ftable const*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__generator_ftable, generate), "generate");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeReflection/Generator.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
