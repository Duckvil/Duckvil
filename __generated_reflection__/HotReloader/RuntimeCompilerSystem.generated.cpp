#include "HotReloader/RuntimeCompilerSystem.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(7)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "RuntimeCompilerSystem");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem, true);
record_inheritance<ISystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerSystem, const Memory::FreeList&, Event::Pool<Event::mode::immediate>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<Memory::Vector<duckvil_recorderd_types>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aRecordedTypes), "m_aRecordedTypes");
_property = record_property<Memory::Vector<ITrackKeeper*>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aHotObjects), "m_aHotObjects");
_property = record_property<Memory::Vector<RuntimeCompilerSystem::reflection_module>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aModules), "m_aModules");
record_function<Duckvil::HotReloader::RuntimeCompilerSystem, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Init, "Init");
record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Update, "Update");
record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Compile, "Compile");
record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, ITrackKeeper*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::AddHotObject, "AddHotObject");
record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const Memory::FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetObjectsHeap, "SetObjectsHeap");
record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, const RuntimeReflection::TrackedObjectCreatedEvent&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::OnEvent, "OnEvent");
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::user_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "user_data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
_property = record_property<RuntimeCompilerSystem*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::user_data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "reflection_module");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_typeHandle), "m_typeHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_generateCustomFunctionHandle), "m_generateCustomFunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_processAST_FunctionHandle), "m_processAST_FunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_clearFunctionHandle), "m_clearFunctionHandle");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "HotReloader/RuntimeCompilerSystem.h", 7 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 7;
}
#endif
