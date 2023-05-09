#include "Engine/ScriptComponent.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(11)
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
_namespaces.push_back("Editor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
}
_namespaces.push_back("NativeScriptBase");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::NativeScriptBase>(_data, "NativeScriptBase");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
_destructor = record_destructor<Duckvil::NativeScriptBase>(_data, _type);
_function = record_function<Duckvil::NativeScriptBase, void, double>(_data, _type, &Duckvil::NativeScriptBase::Update, "Update");
_function = record_function<Duckvil::NativeScriptBase, bool>(_data, _type, &Duckvil::NativeScriptBase::Init, "Init");
_function = record_function<Duckvil::NativeScriptBase, void, ECS::Entity>(_data, _type, &Duckvil::NativeScriptBase::SetEntity, "SetEntity");
_function = record_function<Duckvil::NativeScriptBase, ECS::Entity const&>(_data, _type, &Duckvil::NativeScriptBase::GetEntity, "GetEntity");
_function = record_function<Duckvil::NativeScriptBase, void, double>(_data, _type, &Duckvil::NativeScriptBase::SetDelta, "SetDelta");
_namespaces.pop_back();
_namespaces.push_back("ScriptComponent");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::ScriptComponent>(_data, "ScriptComponent");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
record_meta(_data, _type, "Component", true);
_property = record_property<Memory::fixed_vector_allocator*>(_data, _type, offsetof(Duckvil::ScriptComponent, m_pScripts), "m_pScripts");
_function = record_function<void*, void*>(_data, _type, &Duckvil::ScriptComponent::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<Duckvil::ScriptComponent, void, Duckvil::RuntimeSerializer::ISerializer*>(_data, _type, &Duckvil::ScriptComponent::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<void, nlohmann::json&, const ECS::Entity&>(_data, _type, &Duckvil::ScriptComponent::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<void, const nlohmann::json&, ECS::Entity&>(_data, _type, &Duckvil::ScriptComponent::Deserialize, "Deserialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<bool, const ECS::Entity&>(_data, _type, &Duckvil::ScriptComponent::Has, "Has");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<uint8_t*, const ECS::Entity&>(_data, _type, &Duckvil::ScriptComponent::Get, "Get");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/ScriptComponent.h", 11);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 11;
}
#endif
