#include "Serializer/Runtime/ISerializer.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeSerializer;
_type = record_type<Duckvil::RuntimeSerializer::ISerializer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ISerializer");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeSerializer");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_destructor = record_destructor<Duckvil::RuntimeSerializer::ISerializer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::RuntimeSerializer::ISerializer, void, uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::ISerializer::Clear, "Clear");
record_function<Duckvil::RuntimeSerializer::ISerializer, void, uint32_t, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::ISerializer::Clear, "Clear");
record_function<Duckvil::RuntimeSerializer::ISerializer, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::ISerializer::IsLoading, "IsLoading");
record_function<Duckvil::RuntimeSerializer::ISerializer, bool, const char*, Type&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::ISerializer::SerializeProperty, "SerializeProperty");
record_function<Duckvil::RuntimeSerializer::ISerializer, bool, const char*, Type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeSerializer::ISerializer::SerializeProperty, "SerializeProperty");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Serializer/Runtime/ISerializer.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
