#include "Memory\SlotArray.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(20)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
_type = record_type<__duckvil_slot_array_base>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__duckvil_slot_array_base");
record_property<DUCKVIL_DYNAMIC_ARRAY(uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__duckvil_slot_array_base, m_handleIndices), "m_handleIndices");
record_property<DUCKVIL_DYNAMIC_ARRAY(uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__duckvil_slot_array_base, m_freeIndices), "m_freeIndices");
}
