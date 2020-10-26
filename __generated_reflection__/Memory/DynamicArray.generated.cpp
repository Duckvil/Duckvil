#include "Memory/DynamicArray.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
_type = record_type<__duckvil_dynamic_array>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__duckvil_dynamic_array");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__duckvil_dynamic_array, m_ullSize), "m_ullSize");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__duckvil_dynamic_array, m_ullCapacity), "m_ullCapacity");
}
