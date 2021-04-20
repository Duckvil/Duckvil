#include "Memory/FreeList.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Memory;
_type = record_type<Duckvil::Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "FreeList");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_constructor = record_constructor<Duckvil::Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, ftable*, linear_allocator*, std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, ftable*, free_list_allocator*, std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, ftable*, free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, const FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, FreeList&&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Memory/FreeList.h", 8 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
