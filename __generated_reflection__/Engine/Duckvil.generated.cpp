#include "Engine/Duckvil.h"
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
_namespaces.push_back("system");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::system>(_data, "system");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::system, m_type), "m_type");
_property = record_property<Duckvil::ISystem*>(_data, _type, offsetof(Duckvil::system, m_pTrackKeeper), "m_pTrackKeeper");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::system, m_bIsHot), "m_bIsHot");
_property = record_property<Duckvil::system::UpdateCallback>(_data, _type, offsetof(Duckvil::system, m_fnUpdateCallback), "m_fnUpdateCallback");
_property = record_property<Duckvil::system::InitCallback>(_data, _type, offsetof(Duckvil::system, m_fnInitCallback), "m_fnInitCallback");
_namespaces.pop_back();
_namespaces.push_back("__data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::__data>(_data, "__data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<bool>(_data, _type, offsetof(Duckvil::__data, m_bRunning), "m_bRunning");
_property = record_property<Memory::ftable*>(_data, _type, offsetof(Duckvil::__data, m_pMemory), "m_pMemory");
_property = record_property<Memory::free_list_allocator*>(_data, _type, offsetof(Duckvil::__data, m_pHeap), "m_pHeap");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::__data, m_heap), "m_heap");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::__data, m_objectsHeap), "m_objectsHeap");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::__data, m_eventsHeap), "m_eventsHeap");
_property = record_property<size_t>(_data, _type, offsetof(Duckvil::__data, m_ullLastTimeUsed), "m_ullLastTimeUsed");
_property = record_property<Duckvil::logger_ftable>(_data, _type, offsetof(Duckvil::__data, _loggerFTable), "_loggerFTable");
_property = record_property<Duckvil::logger_data>(_data, _type, offsetof(Duckvil::__data, _loggerData), "_loggerData");
_property = record_property<RuntimeReflection::__ftable*>(_data, _type, offsetof(Duckvil::__data, m_pRuntimeReflection), "m_pRuntimeReflection");
_property = record_property<RuntimeReflection::__recorder_ftable*>(_data, _type, offsetof(Duckvil::__data, m_pRuntimeReflectionRecorder), "m_pRuntimeReflectionRecorder");
_property = record_property<RuntimeReflection::__data*>(_data, _type, offsetof(Duckvil::__data, m_pRuntimeReflectionData), "m_pRuntimeReflectionData");
_property = record_property<Memory::ThreadsafeVector<duckvil_recorderd_types>>(_data, _type, offsetof(Duckvil::__data, m_aRecordedTypes), "m_aRecordedTypes");
_property = record_property<Memory::Vector<PlugNPlay::__module_information>>(_data, _type, offsetof(Duckvil::__data, m_aLoadedModules), "m_aLoadedModules");
_property = record_property<HotReloader::RuntimeCompilerSystem*>(_data, _type, offsetof(Duckvil::__data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
_property = record_property<Duckvil::system::UpdateCallback>(_data, _type, offsetof(Duckvil::__data, m_fnRuntimeCompilerUpdate), "m_fnRuntimeCompilerUpdate");
_property = record_property<Duckvil::system::InitCallback>(_data, _type, offsetof(Duckvil::__data, m_fnRuntimeCompilerInit), "m_fnRuntimeCompilerInit");
_property = record_property<Duckvil::__logger_channel_data*>(_data, _type, offsetof(Duckvil::__data, m_pLoggerChannelData), "m_pLoggerChannelData");
_property = record_property<Duckvil::__logger_channel_ftable*>(_data, _type, offsetof(Duckvil::__data, m_pLoggerChannel), "m_pLoggerChannel");
_property = record_property<Thread::pool_ftable*>(_data, _type, offsetof(Duckvil::__data, m_pThread), "m_pThread");
_property = record_property<Thread::pool_data*>(_data, _type, offsetof(Duckvil::__data, m_pThreadData), "m_pThreadData");
_property = record_property<Duckvil::__time_ftable>(_data, _type, offsetof(Duckvil::__data, m_time), "m_time");
_property = record_property<Duckvil::__time_data>(_data, _type, offsetof(Duckvil::__data, m_timeData), "m_timeData");
_property = record_property<double>(_data, _type, offsetof(Duckvil::__data, m_dOneSecond), "m_dOneSecond");
_property = record_property<Event::Pool<Event::mode::immediate>>(_data, _type, offsetof(Duckvil::__data, m_eventPool), "m_eventPool");
_property = record_property<Event::Pool<Event::mode::buffered>>(_data, _type, offsetof(Duckvil::__data, m_windowEventPool), "m_windowEventPool");
_property = record_property<Event::Pool<Event::mode::buffered>>(_data, _type, offsetof(Duckvil::__data, m_bufferedEventPool), "m_bufferedEventPool");
_property = record_property<Memory::Vector<Duckvil::system>>(_data, _type, offsetof(Duckvil::__data, m_aEngineSystems), "m_aEngineSystems");
_property = record_property<Window::IWindow*>(_data, _type, offsetof(Duckvil::__data, m_pWindow), "m_pWindow");
_property = record_property<Editor::EditorFTable*>(_data, _type, offsetof(Duckvil::__data, m_pEditor), "m_pEditor");
_property = record_property<Editor::ImGuiEditorData*>(_data, _type, offsetof(Duckvil::__data, m_pEditorData), "m_pEditorData");
_property = record_property<Graphics::Renderer::renderer_ftable*>(_data, _type, offsetof(Duckvil::__data, m_pRenderer), "m_pRenderer");
_property = record_property<Graphics::Renderer::renderer_data>(_data, _type, offsetof(Duckvil::__data, m_pRendererData), "m_pRendererData");
_property = record_property<flecs::world>(_data, _type, offsetof(Duckvil::__data, m_ecs), "m_ecs");
_property = record_property<Duckvil::EntityFactory>(_data, _type, offsetof(Duckvil::__data, m_entityFactory), "m_entityFactory");
_property = record_property<flecs::query<Graphics::TransformComponent>>(_data, _type, offsetof(Duckvil::__data, m_rendererQuery), "m_rendererQuery");
_property = record_property<flecs::query<ScriptComponent>>(_data, _type, offsetof(Duckvil::__data, m_scriptsQuery), "m_scriptsQuery");
_property = record_property<ProjectManager::ftable>(_data, _type, offsetof(Duckvil::__data, m_projectManager), "m_projectManager");
_property = record_property<ProjectManager::data>(_data, _type, offsetof(Duckvil::__data, m_projectManagerData), "m_projectManagerData");
_property = record_property<Network::IServer*>(_data, _type, offsetof(Duckvil::__data, m_pServer), "m_pServer");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::__data, m_bIsServer), "m_bIsServer");
_property = record_property<Network::IClient*>(_data, _type, offsetof(Duckvil::__data, m_pClient), "m_pClient");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::__data, m_bIsClient), "m_bIsClient");
_property = record_property<uint16_t>(_data, _type, offsetof(Duckvil::__data, m_usPort), "m_usPort");
_property = record_property<char const*>(_data, _type, offsetof(Duckvil::__data, m_sIP), "m_sIP");
_property = record_property<ScriptEngine::Language*>(_data, _type, offsetof(Duckvil::__data, m_pCSharpLanguage), "m_pCSharpLanguage");
_property = record_property<DependencyInjection::IDependencyResolver*>(_data, _type, offsetof(Duckvil::__data, m_pDependencyInjector), "m_pDependencyInjector");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::__data, m_globalHeap), "m_globalHeap");
_constructor = record_constructor<Duckvil::__data>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("__ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::__ftable>(_data, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<bool(*)(Duckvil::__data*,Memory::ftable*,Memory::free_list_allocator*)>(_data, _type, offsetof(Duckvil::__ftable, init), "init");
_property = record_property<bool(*)(Duckvil::__data*,Duckvil::__ftable*)>(_data, _type, offsetof(Duckvil::__ftable, start), "start");
_property = record_property<bool(*)(Duckvil::__data*,Duckvil::__ftable*)>(_data, _type, offsetof(Duckvil::__ftable, stop), "stop");
_property = record_property<void(*)(Duckvil::__data*,Duckvil::__ftable*)>(_data, _type, offsetof(Duckvil::__ftable, update), "update");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/Duckvil.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
