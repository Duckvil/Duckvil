#include "RuntimeCompiler\RuntimeCompilerSystem.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::hot_object>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "hot_object");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<void**>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::hot_object, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::hot_object, m_typeHandle), "m_typeHandle");
_type = record_type<Duckvil::RuntimeCompiler::RuntimeCompilerSystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "RuntimeCompilerSystem");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, ReflectionFlags::ReflectionFlags_EngineSystem, true);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<ISystem>(_pData), __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, const Memory::FreeList&, RuntimeReflection::__data*, RuntimeReflection::__recorder_ftable*, RuntimeReflection::__ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_pReflectionData), "m_pReflectionData");
_property = record_property<duckvil_recorderd_types*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_aRecordedTypes), "m_aRecordedTypes");
_property = record_property<size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_ullRecordedTypesCount), "m_ullRecordedTypesCount");
_property = record_property<Memory::Vector<hot_object>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_aHotObjects), "m_aHotObjects");
_property = record_property<Memory::Vector<RuntimeCompilerSystem::reflection_module>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_aModules), "m_aModules");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::Init, "Init");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::Update, "Update");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::Compile, "Compile");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, void, void**, RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::AddHotObject, "AddHotObject");
_type = record_type<Duckvil::RuntimeCompiler::RuntimeCompilerSystem::user_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "user_data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<RuntimeCompilerSystem*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::user_data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
_type = record_type<Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "reflection_module");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_typeHandle), "m_typeHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_generateCustomFunctionHandle), "m_generateCustomFunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_processAST_FunctionHandle), "m_processAST_FunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_clearFunctionHandle), "m_clearFunctionHandle");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "RuntimeCompiler/RuntimeCompilerSystem.h", 4 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
