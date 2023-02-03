#include "Event/ImmediateChannel.h"
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
_namespaces.push_back("Event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("helper");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("helper");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("is_stateless");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("reflected_event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Event;
_type = record_type<Duckvil::Event::reflected_event>(_data, "reflected_event");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Event");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::Event::reflected_event, m_functionHandle), "m_functionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::Event::reflected_event, m_typeHandle), "m_typeHandle");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::Event::reflected_event, m_pObject), "m_pObject");
_namespaces.pop_back();
_namespaces.push_back("tracked_event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Event::tracked_event>(_data, "tracked_event");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Event");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::Event::tracked_event, m_functionHandle), "m_functionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::Event::tracked_event, m_typeHandle), "m_typeHandle");
_property = record_property<HotReloader::ITrackKeeper*>(_data, _type, offsetof(Duckvil::Event::tracked_event, m_pObject), "m_pObject");
_namespaces.pop_back();
_namespaces.push_back("Channel");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("imember_const_event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Event::Channel::imember_const_event>(_data, "imember_const_event");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Event");
record_namespace(_data, _type, "Channel");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_function = record_function<Duckvil::Event::Channel::imember_const_event, void, Message const&>(_data, _type, &Duckvil::Event::Channel::imember_const_event::Invoke, "Invoke");
_namespaces.pop_back();
_namespaces.push_back("imember_event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Event::Channel::imember_event>(_data, "imember_event");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Event");
record_namespace(_data, _type, "Channel");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_function = record_function<Duckvil::Event::Channel::imember_event, void, Message&>(_data, _type, &Duckvil::Event::Channel::imember_event::Invoke, "Invoke");
_namespaces.pop_back();
_namespaces.push_back("member_event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("member_const_event");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Event/ImmediateChannel.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
