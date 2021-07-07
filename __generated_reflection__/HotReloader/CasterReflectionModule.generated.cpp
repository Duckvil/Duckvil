#include "HotReloader/CasterReflectionModule.h"
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
_type = record_type<Duckvil::CasterReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "CasterReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_type = record_type<Duckvil::CasterReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "CasterReflectionModule");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_constructor = record_constructor<Duckvil::CasterReflectionModule, const Memory::FreeList&, RuntimeReflection::__ftable*, RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::CasterReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::CasterReflectionModule, m_bHasGeneratedBody), "m_bHasGeneratedBody");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::CasterReflectionModule, m_bIsStruct), "m_bIsStruct");
record_function<Duckvil::CasterReflectionModule, void, Parser::__ast*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::CasterReflectionModule::ProcessAST, "ProcessAST");
record_function<Duckvil::CasterReflectionModule, void, std::ofstream&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::CasterReflectionModule::GenerateCustom, "GenerateCustom");
record_function<Duckvil::CasterReflectionModule, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::CasterReflectionModule::Clear, "Clear");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "HotReloader/CasterReflectionModule.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
