#include "PlugNPlay\AutoLoader.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::PlugNPlay;
_type = record_type<AutoLoader>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "AutoLoader");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "PlugNPlay");
_constructor = record_constructor<AutoLoader, const std::filesystem::path&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<AutoLoader, void, Memory::IMemory*, Memory::__free_list_allocator*, __module_information**, uint32_t*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &AutoLoader::LoadAll, "LoadAll");
}
