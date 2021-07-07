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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil;
_type = record_type<Duckvil::system>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "system");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::system, m_type), "m_type");
_property = record_property<DUCKVIL_TRACK_KEEPER_OBJECT(ISystem)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::system, m_pTrackKeeper), "m_pTrackKeeper");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::system, m_bIsHot), "m_bIsHot");
_property = record_property<system::UpdateCallback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::system, m_fnUpdateCallback), "m_fnUpdateCallback");
_property = record_property<system::InitCallback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::system, m_fnInitCallback), "m_fnInitCallback");
_type = record_type<Duckvil::__data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_bRunning), "m_bRunning");
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_debug_info*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pMemoryDebugger), "m_pMemoryDebugger");
#endif
_property = record_property<Memory::ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pMemory), "m_pMemory");
_property = record_property<Memory::free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pHeap), "m_pHeap");
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_heap), "m_heap");
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_objectsHeap), "m_objectsHeap");
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_eventsHeap), "m_eventsHeap");
_property = record_property<size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_ullLastTimeUsed), "m_ullLastTimeUsed");
_property = record_property<logger_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, _loggerFTable), "_loggerFTable");
_property = record_property<logger_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, _loggerData), "_loggerData");
_property = record_property<RuntimeReflection::__ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pRuntimeReflection), "m_pRuntimeReflection");
_property = record_property<RuntimeReflection::__recorder_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pRuntimeReflectionRecorder), "m_pRuntimeReflectionRecorder");
_property = record_property<RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pRuntimeReflectionData), "m_pRuntimeReflectionData");
_property = record_property<Memory::Vector<duckvil_recorderd_types>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_aRecordedTypes), "m_aRecordedTypes");
_property = record_property<Memory::Vector<PlugNPlay::__module_information>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_aLoadedModules), "m_aLoadedModules");
_property = record_property<HotReloader::RuntimeCompilerSystem*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
_property = record_property<system::UpdateCallback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_fnRuntimeCompilerUpdate), "m_fnRuntimeCompilerUpdate");
_property = record_property<system::InitCallback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_fnRuntimeCompilerInit), "m_fnRuntimeCompilerInit");
_property = record_property<__logger_channel_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pLoggerChannelData), "m_pLoggerChannelData");
_property = record_property<__logger_channel_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pLoggerChannel), "m_pLoggerChannel");
_property = record_property<Thread::pool_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pThread), "m_pThread");
_property = record_property<Thread::pool_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pThreadData), "m_pThreadData");
_property = record_property<__time_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_time), "m_time");
_property = record_property<__time_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_timeData), "m_timeData");
_property = record_property<double>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_dOneSecond), "m_dOneSecond");
_property = record_property<Event::Pool<Event::mode::immediate>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_eventPool), "m_eventPool");
_property = record_property<Event::Pool<Event::mode::buffered>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_windowEventPool), "m_windowEventPool");
_property = record_property<Memory::Vector<Duckvil::system>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_aEngineSystems), "m_aEngineSystems");
_property = record_property<Window::IWindow*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pWindow), "m_pWindow");
_property = record_property<Editor::EditorFTable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pEditor), "m_pEditor");
_property = record_property<Editor::ImGuiEditorData*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pEditorData), "m_pEditorData");
_property = record_property<Graphics::Renderer::renderer_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pRenderer), "m_pRenderer");
_property = record_property<Graphics::Renderer::renderer_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_pRendererData), "m_pRendererData");
_property = record_property<flecs::world>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_ecs), "m_ecs");
_property = record_property<flecs::query<Graphics::TransformComponent>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__data, m_rendererQuery), "m_rendererQuery");
_type = record_type<Duckvil::__ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<bool(*)(Duckvil::__data*, Memory::ftable*, Memory::free_list_allocator*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__ftable, init), "init");
_property = record_property<bool(*)(Duckvil::__data*, Duckvil::__ftable*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__ftable, start), "start");
_property = record_property<bool(*)(Duckvil::__data*, Duckvil::__ftable*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__ftable, stop), "stop");
_property = record_property<void(*)(Duckvil::__data*, Duckvil::__ftable*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::__ftable, update), "update");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Engine/Duckvil.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
