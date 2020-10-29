#include "PlugNPlay\Plugin.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::PlugNPlay;
_type = record_type<Duckvil::PlugNPlay::__plugin_details>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__plugin_details");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "PlugNPlay");
_property = record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_sPluginName), "m_sPluginName");
_property = record_property<__instantiation>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_instantiation), "m_instantiation");
_property = record_property<allocator_callback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_pAllocator), "m_pAllocator");
_property = record_property<instance_callback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_pInstance), "m_pInstance");
_property = record_property<free_callback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__plugin_details, m_pDeallocator), "m_pDeallocator");
}
