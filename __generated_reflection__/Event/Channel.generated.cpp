#include "Event\Channel.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Event;
_type = record_type<Duckvil::Event::IChannel>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "IChannel");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Event");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_constructor = record_constructor<Duckvil::Event::IChannel, RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::mutex>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel, m_mutex), "m_mutex");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel, m_ullTypeID), "m_ullTypeID");
_property = record_property<mode>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel, m_mode), "m_mode");
_property = record_property<RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel, m_pReflectionData), "m_pReflectionData");
_type = record_type<Duckvil::Event::IChannel::event>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "event");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Event");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel::event, m_functionHandle), "m_functionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel::event, m_typeHandle), "m_typeHandle");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::IChannel::event, m_pObject), "m_pObject");
_type = record_type<Duckvil::Event::Channel::Channel::BufferedChannel>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "BufferedChannel");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Event");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<IChannel>(_pData), __protection::__protection_public);
_constructor = record_constructor<Duckvil::Event::Channel::Channel::BufferedChannel, RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::Channel::Channel::BufferedChannel, m_uiIndex), "m_uiIndex");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::Channel::Channel::BufferedChannel, m_uiEventsCount), "m_uiEventsCount");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Event/Channel.h", 0 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
