#include "PlugNPlay\Module.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::PlugNPlay;
_type = record_type<Duckvil::PlugNPlay::__module_information>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__module_information");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "PlugNPlay");
_constructor = record_constructor<Duckvil::PlugNPlay::__module_information, const Utils::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<Utils::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__module_information, m_sName), "m_sName");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__module_information, m_pModule), "m_pModule");
_property = record_property<Utils::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::PlugNPlay::__module_information, m_sPath), "m_sPath");
_type = record_type<Duckvil::PlugNPlay::module>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "module");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "PlugNPlay");
}
