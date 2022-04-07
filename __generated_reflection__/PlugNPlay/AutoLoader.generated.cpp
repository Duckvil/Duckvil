#include "PlugNPlay/AutoLoader.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::PlugNPlay;
_type = record_type<Duckvil::PlugNPlay::AutoLoader>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "AutoLoader");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "PlugNPlay");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_constructor = record_constructor<Duckvil::PlugNPlay::AutoLoader, const std::filesystem::path&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::PlugNPlay::AutoLoader>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_function = record_function<Duckvil::PlugNPlay::AutoLoader, void, Memory::ftable*, Memory::free_list_allocator*, __module_information**, uint32_t*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::PlugNPlay::AutoLoader::LoadAll, "LoadAll");
_function = record_function<Duckvil::PlugNPlay::AutoLoader, void, Memory::ftable*, Memory::free_list_allocator*, Memory::Vector<__module_information>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::PlugNPlay::AutoLoader::LoadAll, "LoadAll");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "PlugNPlay/AutoLoader.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
