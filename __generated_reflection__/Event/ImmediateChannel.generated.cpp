#include "Event/ImmediateChannel.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Event;
_type = record_type<Duckvil::Event::reflected_event>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "reflected_event");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Event");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::reflected_event, m_functionHandle), "m_functionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::reflected_event, m_typeHandle), "m_typeHandle");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::reflected_event, m_pObject), "m_pObject");
_type = record_type<Duckvil::Event::Channel::imember_event>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "imember_event");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Event");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
record_function<Duckvil::Event::Channel::imember_event, void, const Message&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Event::Channel::imember_event::Invoke, "Invoke");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Event/ImmediateChannel.h", 4 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
