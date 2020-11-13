#include "Memory\SlotArray.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(19)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
_type = record_type<__duckvil_slot_array_base>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__duckvil_slot_array_base");
_property = record_property<DUCKVIL_DYNAMIC_ARRAY(uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__duckvil_slot_array_base, m_handleIndices), "m_handleIndices");
_property = record_property<DUCKVIL_DYNAMIC_ARRAY(uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__duckvil_slot_array_base, m_freeIndices), "m_freeIndices");
}
