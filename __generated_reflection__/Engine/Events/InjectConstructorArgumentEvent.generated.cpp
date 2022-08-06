#include "Engine/Events/InjectConstructorArgumentEvent.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
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
_namespaces.push_back("InjectConstructorArgumentEvent");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::InjectConstructorArgumentEvent>(_data, "InjectConstructorArgumentEvent");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<bool>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent, m_bSuccess), "m_bSuccess");
_property = record_property<FunctionArgumentsPusher*>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent, m_pFAP), "m_pFAP");
_property = record_property<InjectConstructorArgumentEvent::Info>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent, m_info), "m_info");
_property = record_property<RuntimeReflection::__argument_t>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent, m_argument), "m_argument");
_constructor = record_constructor<Duckvil::InjectConstructorArgumentEvent, FunctionArgumentsPusher*, const InjectConstructorArgumentEvent::Info&, const RuntimeReflection::__argument_t&>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("Info");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::InjectConstructorArgumentEvent::Info>(_data, "Info");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "InjectConstructorArgumentEvent");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent::Info, m_typeHandle), "m_typeHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_constructor_t>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent::Info, m_constructorHandle), "m_constructorHandle");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::InjectConstructorArgumentEvent::Info, m_uiArgumentIndex), "m_uiArgumentIndex");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/Events/InjectConstructorArgumentEvent.h", 4);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
