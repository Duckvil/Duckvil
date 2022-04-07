#include "ProjectManager/ProjectManager.h"
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
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::ProjectManager;
_type = record_type<Duckvil::ProjectManager::data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "ProjectManager");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_heap), "m_heap");
_property = record_property<HotReloader::FileWatcher*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_pFileWatcher), "m_pFileWatcher");
_property = record_property<HotReloader::RuntimeCompilerSystem*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_pRCS), "m_pRCS");
_property = record_property<RuntimeReflection::__function<data::CompileCallback>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_fnCompile), "m_fnCompile");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_fileWatcherType), "m_fileWatcherType");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_watchHandle), "m_watchHandle");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_updateHandle), "m_updateHandle");
_property = record_property<RuntimeReflection::__proxy_member_function<void>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_fnUpdateProject), "m_fnUpdateProject");
_property = record_property<Event::Pool<Event::mode::immediate>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_pEngineEventPool), "m_pEngineEventPool");
_property = record_property<Event::Pool<Event::mode::immediate>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_projectManagerEventPool), "m_projectManagerEventPool");
_property = record_property<project>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_loadedProject), "m_loadedProject");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_bLoaded), "m_bLoaded");
_property = record_property<double>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::data, m_dOneSecond), "m_dOneSecond");
_type = record_type<Duckvil::ProjectManager::ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "ProjectManager");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<bool(*)(data*, const Memory::FreeList&, Event::Pool<Event::mode::immediate>*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnInitProjectManager), "m_fnInitProjectManager");
_property = record_property<project(*)(data*, const Utils::string&, const Utils::string&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnLoadProject), "m_fnLoadProject");
_property = record_property<bool(*)(data*, const Utils::string&, const Utils::string&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnCreateProject), "m_fnCreateProject");
_property = record_property<void(*)(data*, double)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnUpdate), "m_fnUpdate");
_property = record_property<void(*)(data*, HotReloader::RuntimeCompilerSystem*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnSetRuntimeCompiler), "m_fnSetRuntimeCompiler");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "ProjectManager/ProjectManager.h", 4);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
