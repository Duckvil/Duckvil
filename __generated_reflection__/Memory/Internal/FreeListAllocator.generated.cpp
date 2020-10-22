#include "Memory\Internal\FreeListAllocator.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(12)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
using namespace Duckvil::Memory;
_type = record_type<__free_list_header>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__free_list_header");
_property = record_property<uint8_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__free_list_header, m_ucPadding), "m_ucPadding");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__free_list_header, m_ullSize), "m_ullSize");
_type = record_type<__free_list_node>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__free_list_node");
_property = record_property<__free_list_node*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__free_list_node, m_pNext), "m_pNext");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__free_list_node, m_ullSize), "m_ullSize");
}