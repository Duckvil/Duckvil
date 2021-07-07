#include "HotReloader/TrackKeeper.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(10)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::TrackKeeper>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "TrackKeeper");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
record_inheritance<ITrackKeeper>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::HotReloader::TrackKeeper, void*, RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::HotReloader::TrackKeeper>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::HotReloader::TrackKeeper, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::TrackKeeper::GetObject, "GetObject");
record_function<Duckvil::HotReloader::TrackKeeper, RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::TrackKeeper::GetTypeHandle, "GetTypeHandle");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "HotReloader/TrackKeeper.h", 10);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 10;
}
#endif
