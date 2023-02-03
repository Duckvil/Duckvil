#include "HotReloader/RuntimeCompilerSystem.h"
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
{
_namespaces.push_back("Network");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
}
_namespaces.push_back("RuntimeCompilerSystem");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem>(_data, "RuntimeCompilerSystem");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
record_meta(_data, _type, Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem, true);
record_meta(_data, _type, Duckvil::ReflectionFlags_AutoInstantiate, false);
record_inheritance<ISystem>(_data, _type, __protection::__protection_public);
record_inheritance<Editor::Widget>(_data, _type, __protection::__protection_public);
_function = record_function<void, std::filesystem::path const&, FileWatcher::FileStatus, void*>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Action, "Action");
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerSystem, Memory::FreeList const&, Event::Pool<Event::mode::immediate>*, Event::Pool<Event::mode::immediate>*, FileWatcher::ActionCallback, void*>(_data, _type);
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerSystem, Memory::FreeList const&, Event::Pool<Event::mode::immediate>*, Event::Pool<Event::mode::immediate>*>(_data, _type);
_destructor = record_destructor<Duckvil::HotReloader::RuntimeCompilerSystem>(_data, _type);
_property = record_property<Memory::Vector<RuntimeCompilerSystem::hot_object>>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aHotObjects), "m_aHotObjects");
_property = record_property<Memory::Vector<RuntimeCompilerSystem::reflection_module>>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_aModules), "m_aModules");
_property = record_property<std::filesystem::path>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem, m_path), "m_path");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, bool>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Init, "Init");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, bool, std::vector<std::filesystem::path> const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Init, "Init");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, double>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Update, "Update");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, void*>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::InitEditor, "InitEditor");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::OnDraw, "OnDraw");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, std::string const&, RuntimeCompiler::Options const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Compile, "Compile");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, std::string const&, RuntimeCompiler::Options const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::CompileT, "CompileT");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, std::filesystem::path const&, std::filesystem::path const&, bool>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::GenerateReflection, "GenerateReflection");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, RuntimeCompilerSystem::hot_object*, RuntimeReflection::__duckvil_resource_type_t const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::Swap, "Swap");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, RuntimeCompilerSystem::hot_object const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::AddHotObject, "AddHotObject");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, Memory::FreeList const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetObjectsHeap, "SetObjectsHeap");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, Memory::Vector<PlugNPlay::__module_information>*>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetModules, "SetModules");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, Memory::ThreadsafeVector<duckvil_recorderd_types>*>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetReflectedTypes, "SetReflectedTypes");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, std::filesystem::path const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::SetCWD, "SetCWD");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, std::filesystem::path const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::GetCWD, "GetCWD");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, RuntimeCompiler::ICompiler*>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::GetCompiler, "GetCompiler");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerSystem, void, RuntimeReflection::TrackedObjectCreatedEvent const&>(_data, _type, &Duckvil::HotReloader::RuntimeCompilerSystem::OnEvent, "OnEvent");
_namespaces.pop_back();
_namespaces.push_back("user_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::user_data>(_data, "user_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_namespace(_data, _type, "RuntimeCompilerSystem");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
_property = record_property<Duckvil::HotReloader::RuntimeCompilerSystem*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::user_data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
_namespaces.pop_back();
_namespaces.push_back("reflection_module");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module>(_data, "reflection_module");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_namespace(_data, _type, "RuntimeCompilerSystem");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
_property = record_property<void*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_typeHandle), "m_typeHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_generateCustomFunctionHandle), "m_generateCustomFunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_processAST_FunctionHandle), "m_processAST_FunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::reflection_module, m_clearFunctionHandle), "m_clearFunctionHandle");
_namespaces.pop_back();
_namespaces.push_back("hot_object");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::HotReloader::RuntimeCompilerSystem::hot_object>(_data, "hot_object");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_namespace(_data, _type, "RuntimeCompilerSystem");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
_property = record_property<Duckvil::HotReloader::ITrackKeeper*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::hot_object, m_pObject), "m_pObject");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerSystem::hot_object, m_pSerializeFunction), "m_pSerializeFunction");
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerSystem::hot_object, Duckvil::HotReloader::ITrackKeeper*, void*>(_data, _type);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/RuntimeCompilerSystem.h", 11);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 11;
}
#endif
