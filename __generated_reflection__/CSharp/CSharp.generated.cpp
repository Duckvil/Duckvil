#include "CSharp/CSharp.h"
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
_namespaces.push_back("CSharp");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("CSharp");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::CSharp;
_type = record_type<Duckvil::CSharp::CSharp>(_data, "CSharp");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "CSharp");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<ScriptEngine::Language>(_data, _type, __protection::__protection_public);
_function = record_function<Duckvil::CSharp::CSharp*>(_data, _type, &Duckvil::CSharp::CSharp::GetScriptEngine, "GetScriptEngine");
_constructor = record_constructor<Duckvil::CSharp::CSharp, Memory::FreeList const&, ECS::EntityFactory*, Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_data, _type);
record_meta(_data, _type, _constructor, 2, "Engine", true);
_destructor = record_destructor<Duckvil::CSharp::CSharp>(_data, _type);
_property = record_property<std::unordered_map<MonoType *, Duckvil::CSharp::CSharp::Function>>(_data, _type, offsetof(Duckvil::CSharp::CSharp, m_aComponentFuncs), "m_aComponentFuncs");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::CSharp::CSharp, m_pCoreAssembly), "m_pCoreAssembly");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::CSharp::CSharp, m_pAppAssembly), "m_pAppAssembly");
_function = record_function<Duckvil::CSharp::CSharp, bool>(_data, _type, &Duckvil::CSharp::CSharp::Setup, "Setup");
_function = record_function<Duckvil::CSharp::CSharp, bool>(_data, _type, &Duckvil::CSharp::CSharp::Init, "Init");
_function = record_function<Duckvil::CSharp::CSharp, void>(_data, _type, &Duckvil::CSharp::CSharp::Update, "Update");
_function = record_function<Duckvil::CSharp::CSharp, bool>(_data, _type, &Duckvil::CSharp::CSharp::Shutdown, "Shutdown");
_function = record_function<Duckvil::CSharp::CSharp, MonoObject*, void*>(_data, _type, &Duckvil::CSharp::CSharp::Create, "Create");
_function = record_function<Duckvil::CSharp::CSharp, Duckvil::CSharp::SharedScript*, RuntimeReflection::__duckvil_resource_type_t const&>(_data, _type, &Duckvil::CSharp::CSharp::Create, "Create");
_function = record_function<Duckvil::CSharp::CSharp, void, size_t*, Duckvil::CSharp::SharedScript*>(_data, _type, &Duckvil::CSharp::CSharp::Register, "Register");
_function = record_function<Duckvil::CSharp::CSharp::ScriptFieldType, void*>(_data, _type, &Duckvil::CSharp::CSharp::MonoTypeToScriptFieldType, "MonoTypeToScriptFieldType");
_function = record_function<Duckvil::CSharp::CSharp, ECS::EntityFactory*>(_data, _type, &Duckvil::CSharp::CSharp::GetECS, "GetECS");
_function = record_function<Duckvil::CSharp::CSharp, MonoDomain*>(_data, _type, &Duckvil::CSharp::CSharp::GetAppDomain, "GetAppDomain");
_function = record_function<Duckvil::CSharp::CSharp, MonoDomain*>(_data, _type, &Duckvil::CSharp::CSharp::GetRootDomain, "GetRootDomain");
_function = record_function<Duckvil::CSharp::CSharp, Duckvil::CSharp::SharedScript*, size_t>(_data, _type, &Duckvil::CSharp::CSharp::GetSharedScript, "GetSharedScript");
_function = record_function<Duckvil::CSharp::CSharp, void*, char const*, char const**, uint32_t>(_data, _type, &Duckvil::CSharp::CSharp::GetType, "GetType");
_function = record_function<Duckvil::CSharp::CSharp, void*, void*>(_data, _type, &Duckvil::CSharp::CSharp::CreateObject, "CreateObject");
_namespaces.pop_back();
_namespaces.push_back("Function");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::CSharp::CSharp::Function>(_data, "Function");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "CSharp");
record_namespace(_data, _type, "CSharp");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<Duckvil::CSharp::CSharp::Function::HasComponentCallback>(_data, _type, offsetof(Duckvil::CSharp::CSharp::Function, m_fnHasComponent), "m_fnHasComponent");
_property = record_property<Duckvil::CSharp::CSharp::Function::AddComponentCallback>(_data, _type, offsetof(Duckvil::CSharp::CSharp::Function, m_fnAddComponent), "m_fnAddComponent");
_namespaces.pop_back();
_enum = record_enum<Duckvil::CSharp::CSharp::ScriptFieldType>(_data, _ntype, "ScriptFieldType");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::None, "None");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::Int8, "Int8");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::UInt8, "UInt8");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::Int16, "Int16");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::UInt16, "UInt16");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::Int32, "Int32");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::UInt32, "UInt32");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::Int64, "Int64");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::UInt64, "UInt64");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::CSharp::CSharp::ScriptFieldType::SIZE, "SIZE");
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "CSharp/CSharp.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
