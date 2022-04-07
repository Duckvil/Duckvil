#include "Utils/FunctionArgumentsPusher.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil;
_type = record_type<Duckvil::FunctionArgumentsPusher>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "FunctionArgumentsPusher");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
record_inheritance<Xbyak::CodeGenerator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, _property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::FunctionArgumentsPusher, m_uiIndex), "m_uiIndex");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::FunctionArgumentsPusher, m_aFloats), "m_aFloats");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::FunctionArgumentsPusher, m_uiCount), "m_uiCount");
_constructor = record_constructor<Duckvil::FunctionArgumentsPusher, uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::FunctionArgumentsPusher>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_function = record_function<Duckvil::FunctionArgumentsPusher, explicit, const int&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::FunctionArgumentsPusher::voidPush, "voidPush");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, const size_t&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, const float&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, const double&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, const bool&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::FunctionArgumentsPusher::Call, "Call");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Utils/FunctionArgumentsPusher.h", 4);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
