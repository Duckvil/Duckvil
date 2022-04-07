#include "HotReloader/RuntimeCompilerSystem.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(9)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "RuntimeCompilerSystem");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem, true);
record_inheritance<ISystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
record_inheritance<Editor::Widget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerSystem, const Memory::FreeList&, Event::Pool<Event::mode::immediate>*, Event::Pool<Event::mode::immediate>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::HotReloader::RuntimeCompilerSystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<Memory::Vector<RuntimeCompilerSystem::hot_object>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aHotObjects), "m_aHotObjects");
_property = record_property<Memory::Vector<RuntimeCompilerSystem::reflection_module>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aModules), "m_aModules");
_property = record_property<std::filesystem::path>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_path), "m_path");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Init, "Init");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, bool, const std::filesystem::path&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Init, "Init");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, double>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Update, "Update");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::InitEditor, "InitEditor");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::OnDraw, "OnDraw");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const std::filesystem::path&, const std::string&, void(*)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&), bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Compile, "Compile");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const std::string&, bool, const RuntimeCompiler::Options&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Compile, "Compile");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, RuntimeCompilerSystem::hot_object*, const RuntimeReflection::__duckvil_resource_type_t&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Swap, "Swap");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const RuntimeCompilerSystem::hot_object&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::AddHotObject, "AddHotObject");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const Memory::FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetObjectsHeap, "SetObjectsHeap");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, Memory::Vector<PlugNPlay::__module_information>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetModules, "SetModules");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, Memory::ThreadsafeVector<duckvil_recorderd_types>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetReflectedTypes, "SetReflectedTypes");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const RuntimeReflection::TrackedObjectCreatedEvent&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::OnEvent, "OnEvent");
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::user_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "user_data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<RuntimeCompilerSystem*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::user_data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
_property = record_property<std::filesystem::path>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::user_data, m_file), "m_file");
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "reflection_module");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_typeHandle), "m_typeHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_generateCustomFunctionHandle), "m_generateCustomFunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_processAST_FunctionHandle), "m_processAST_FunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_clearFunctionHandle), "m_clearFunctionHandle");
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::hot_object>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "hot_object");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<ITrackKeeper*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::hot_object, m_pObject), "m_pObject");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::hot_object, m_pSerializeFunction), "m_pSerializeFunction");
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerSystem::hot_object, ITrackKeeper*, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "HotReloader/RuntimeCompilerSystem.h", 9);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 9;
}
#endif
