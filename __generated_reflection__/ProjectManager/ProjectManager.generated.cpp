#include "ProjectManager/ProjectManager.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(6)
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
_namespaces.push_back("ProjectManager");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::ProjectManager::project_type>(_data, _ntype, "project_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ProjectManager::project_type::standalone, "standalone");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ProjectManager::project_type::plugin, "plugin");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ProjectManager::project_type::mod, "mod");
_namespaces.push_back("data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::ProjectManager;
_type = record_type<Duckvil::ProjectManager::data>(_data, "data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "ProjectManager");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
record_meta(_data, _type, Duckvil::DependencyInjection::INJECTABLE, true);
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_heap), "m_heap");
_property = record_property<HotReloader::RuntimeCompilerSystem*>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_pRCS), "m_pRCS");
_property = record_property<RuntimeReflection::__proxy_member_function<void>*>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_fnUpdateProject), "m_fnUpdateProject");
_property = record_property<Event::Pool<Event::mode::immediate>*>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_pEngineEventPool), "m_pEngineEventPool");
_property = record_property<Event::Pool<Event::mode::immediate>>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_projectManagerEventPool), "m_projectManagerEventPool");
_property = record_property<project>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_loadedProject), "m_loadedProject");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_bLoaded), "m_bLoaded");
_property = record_property<double>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_dOneSecond), "m_dOneSecond");
_property = record_property<HotReloader::RuntimeCompilerSystem*>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_pRuntimeCompilerSystem), "m_pRuntimeCompilerSystem");
_property = record_property<bool(Duckvil::ISystem::*)(std::vector<std::filesystem::path> const&,bool)>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_fnRuntimeCompilerInit), "m_fnRuntimeCompilerInit");
_property = record_property<Duckvil::ProjectManager::data::UpdateCallback>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_fnRuntimeCompilerUpdate), "m_fnRuntimeCompilerUpdate");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_objectsHeap), "m_objectsHeap");
_property = record_property<DependencyInjection::IDependencyResolver*>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_pDependencyResolver), "m_pDependencyResolver");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_bSimulating), "m_bSimulating");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::ProjectManager::data, m_bPaused), "m_bPaused");
_function = record_function<void*, void*>(_data, _type, &Duckvil::ProjectManager::data::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<Duckvil::ProjectManager::data, void, Duckvil::RuntimeSerializer::ISerializer*>(_data, _type, &Duckvil::ProjectManager::data::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.push_back("ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::ProjectManager::ftable>(_data, "ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "ProjectManager");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
record_meta(_data, _type, Duckvil::DependencyInjection::INJECTABLE, true);
_property = record_property<bool(*)(data*,Memory::FreeList const&,Event::Pool<Event::mode::immediate>*)>(_data, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnInitProjectManager), "m_fnInitProjectManager");
_property = record_property<project(*)(data*,Utils::string const&,Utils::string const&)>(_data, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnLoadProject), "m_fnLoadProject");
_property = record_property<bool(*)(data*,Utils::string const&,Utils::string const&)>(_data, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnCreateProject), "m_fnCreateProject");
_property = record_property<void(*)(data*,double)>(_data, _type, offsetof(Duckvil::ProjectManager::ftable, m_fnUpdate), "m_fnUpdate");
_function = record_function<void*, void*>(_data, _type, &Duckvil::ProjectManager::ftable::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<Duckvil::ProjectManager::ftable, void, Duckvil::RuntimeSerializer::ISerializer*>(_data, _type, &Duckvil::ProjectManager::ftable::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "ProjectManager/ProjectManager.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
