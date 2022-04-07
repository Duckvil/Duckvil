#include "HotReloader/ReflectionModule.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
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
_type = record_type<Duckvil::HotReloader::RuntimeCompilerReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "RuntimeCompilerReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_type = record_type<Duckvil::HotReloader::RuntimeCompilerReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "RuntimeCompilerReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_property = record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_sReflectionModuleName), "m_sReflectionModuleName");
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerReflectionModule, const Memory::FreeList&, RuntimeReflection::__ftable*, RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::HotReloader::RuntimeCompilerReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pRuntimeReflectionData), "m_pRuntimeReflectionData");
_property = record_property<RuntimeReflection::__ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pRuntimeReflection), "m_pRuntimeReflection");
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_heap), "m_heap");
_property = record_property<std::filesystem::path>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_sFile), "m_sFile");
_property = record_property<std::vector<RuntimeCompilerReflectionModule::Context*>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_aContexts), "m_aContexts");
_property = record_property<RuntimeCompilerReflectionModule::Context*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pCurrentContext), "m_pCurrentContext");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void, Parser::__ast*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::ProcessAST, "ProcessAST");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void, std::ofstream&, std::vector<std::pair<uint32_t,std::vector<std::string>>>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::GenerateCustom, "GenerateCustom");
_function = record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::Clear, "Clear");
_type = record_type<Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Context");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "HotReloader");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_uiGeneratedBodyLine), "m_uiGeneratedBodyLine");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_aVars), "m_aVars");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_aChildSerializers), "m_aChildSerializers");
_property = record_property<std::vector<RuntimeCompilerReflectionModule::Context*>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_aChildren), "m_aChildren");
_property = record_property<RuntimeCompilerReflectionModule::Context*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule::Context, m_pParent), "m_pParent");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "HotReloader/ReflectionModule.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
