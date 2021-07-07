#include "Event/BufferedChannel.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Event;
_type = record_type<Duckvil::Event::BufferedChannel>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "BufferedChannel");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Event");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
record_inheritance<IChannel>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<bool(*)(BufferedChannel*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::BufferedChannel, m_fnAnyEvents), "m_fnAnyEvents");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::BufferedChannel, m_uiIndex), "m_uiIndex");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Event::BufferedChannel, m_uiEventsCount), "m_uiEventsCount");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Event/BufferedChannel.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
