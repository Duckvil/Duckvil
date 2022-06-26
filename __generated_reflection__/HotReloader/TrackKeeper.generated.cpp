#include "HotReloader/TrackKeeper.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(11)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("HotReloader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("TrackKeeper");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::TrackKeeper>(_data, "TrackKeeper");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
record_inheritance<ITrackKeeper>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::HotReloader::TrackKeeper, void*, RuntimeReflection::__duckvil_resource_type_t>(_data, _type);
_destructor = record_destructor<Duckvil::HotReloader::TrackKeeper>(_data, _type);
_function = record_function<Duckvil::HotReloader::TrackKeeper, void*>(_data, _type, &Duckvil::HotReloader::TrackKeeper::GetObject, "GetObject");
_function = record_function<Duckvil::HotReloader::TrackKeeper, RuntimeReflection::__duckvil_resource_type_t>(_data, _type, &Duckvil::HotReloader::TrackKeeper::GetTypeHandle, "GetTypeHandle");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/TrackKeeper.h", 11);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 11;
}
#endif
