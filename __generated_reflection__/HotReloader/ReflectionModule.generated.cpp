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
_constructor = record_constructor<Duckvil::HotReloader::RuntimeCompilerReflectionModule, const Memory::FreeList&, RuntimeReflection::__ftable*, RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::HotReloader::RuntimeCompilerReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<RuntimeReflection::__ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_pRuntimeReflection), "m_pRuntimeReflection");
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_heap), "m_heap");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_bHasGeneratedBody), "m_bHasGeneratedBody");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_aVars), "m_aVars");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::HotReloader::RuntimeCompilerReflectionModule, m_aChildSerializers), "m_aChildSerializers");
record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void, Parser::__ast*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::ProcessAST, "ProcessAST");
record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void, std::ofstream&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::GenerateCustom, "GenerateCustom");
record_function<Duckvil::HotReloader::RuntimeCompilerReflectionModule, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::HotReloader::RuntimeCompilerReflectionModule::Clear, "Clear");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "HotReloader/ReflectionModule.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
