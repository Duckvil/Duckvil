#include "HotReloader/FileWatcher.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::FileWatcher>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "FileWatcher");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_constructor = record_constructor<Duckvil::HotReloader::FileWatcher, FileWatcher::ActionCallback, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::HotReloader::FileWatcher>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::HotReloader::FileWatcher, void, const std::filesystem::path&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::FileWatcher::Watch, "Watch");
record_function<Duckvil::HotReloader::FileWatcher, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::FileWatcher::Update, "Update");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "HotReloader/FileWatcher.h", 4);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
