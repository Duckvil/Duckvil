#include "Memory/MemoryDebugger.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(21)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Memory;
_type = record_type<Duckvil::Memory::allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 21);
#ifdef DUCKVIL_MEMORY_DEBUGGER
_type = record_type<duckvil_memory_debug_info>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "duckvil_memory_debug_info");
_recordedTypes.push_back(_type);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 21);
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<Duckvil::Memory::allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_memory_debug_info, m_pAllocator), "m_pAllocator");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_debug_info*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_memory_debug_info, m_pParent), "m_pParent");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_allocator_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_memory_debug_info, m_allocatorType), "m_allocatorType");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_memory_debug_info, m_aLabel), "m_aLabel");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<std::vector<duckvil_memory_debug_info*>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_memory_debug_info, m_aOther), "m_aOther");
#endif
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Memory/MemoryDebugger.h", 21 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 21;
}
#endif
