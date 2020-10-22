#include "Memory\Internal\QueueAllocator.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(14)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
using namespace Duckvil::Memory;
_type = record_type<__queue_node>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__queue_node");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__queue_node, m_ullBlockSize), "m_ullBlockSize");
_property = record_property<uint8_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__queue_node, m_ucPadding), "m_ucPadding");
}