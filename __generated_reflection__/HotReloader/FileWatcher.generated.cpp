#include "HotReloader/FileWatcher.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(5)
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
_namespaces.push_back("FileWatcher");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::FileWatcher>(_data, "FileWatcher");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 5);
_constructor = record_constructor<Duckvil::HotReloader::FileWatcher, FileWatcher::ActionCallback, void*>(_data, _type);
_destructor = record_destructor<Duckvil::HotReloader::FileWatcher>(_data, _type);
_function = record_function<Duckvil::HotReloader::FileWatcher, void, const std::filesystem::path&>(_data, _type, &Duckvil::HotReloader::FileWatcher::Watch, "Watch");
_function = record_function<Duckvil::HotReloader::FileWatcher, void>(_data, _type, &Duckvil::HotReloader::FileWatcher::Update, "Update");
_namespaces.pop_back();
_enum = record_enum<Duckvil::HotReloader::FileWatcher::FileStatus>(_data, _ntype, "FileStatus");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::HotReloader::FileWatcher::FileStatus::FileStatus_Created, "FileStatus_Created");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::HotReloader::FileWatcher::FileStatus::FileStatus_Modified, "FileStatus_Modified");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::HotReloader::FileWatcher::FileStatus::FileStatus_Removed, "FileStatus_Removed");
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/FileWatcher.h", 5);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 5;
}
#endif
