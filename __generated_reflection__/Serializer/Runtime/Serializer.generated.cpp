#include "Serializer/Runtime/Serializer.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeSerializer;
_type = record_type<Duckvil::RuntimeSerializer::Serializer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Serializer");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeSerializer");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
record_inheritance<ISerializer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeSerializer::Serializer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::RuntimeSerializer::Serializer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, void*, RuntimeReflection::__function<voidHotReloader::HotObject::*RuntimeSerializer::ISerializer*>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::Serialize, "Serialize");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, void*, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::Serialize, "Serialize");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, uint32_t, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::Clear, "Clear");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::IsLoading, "IsLoading");
_function = record_function<Duckvil::RuntimeSerializer::Serializer, void, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::Serializer::SetLoading, "SetLoading");
_type = record_type<Duckvil::RuntimeSerializer::Serializer::cmp_str>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "cmp_str");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeSerializer");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Serializer/Runtime/Serializer.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
