#include "Process/Process.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(2)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Process;
_type = record_type<Duckvil::Process::data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Process");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::data, m_bComplete), "m_bComplete");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::data, m_bRunning), "m_bRunning");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::data, m_pImplementationData), "m_pImplementationData");
_type = record_type<Duckvil::Process::ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Process");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<void(*)(Duckvil::Memory::IMemory*, Duckvil::Memory::__free_list_allocator*, data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::ftable, m_fnInit), "m_fnInit");
_property = record_property<bool(*)(data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::ftable, m_fnSetup), "m_fnSetup");
_property = record_property<void(*)(data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::ftable, m_fnStart), "m_fnStart");
_property = record_property<void(*)(data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::ftable, m_fnStop), "m_fnStop");
_property = record_property<void(*)(data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::ftable, m_fnWait), "m_fnWait");
_property = record_property<void(*)(data*, const char*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Process::ftable, m_fnWrite), "m_fnWrite");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Process/Process.h", 2 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
