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
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Process");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Process;
_type = record_type<Duckvil::Process::data>(_data, "data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Process");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<void*>(_data, _type, offsetof(Duckvil::Process::data, m_pImplementationData), "m_pImplementationData");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Process::data, m_bComplete), "m_bComplete");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Process::data, m_bRunning), "m_bRunning");
_namespaces.pop_back();
_namespaces.push_back("ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Process::ftable>(_data, "ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Process");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_property = record_property<void(*)(Duckvil::Memory::ftable*, Duckvil::Memory::free_list_allocator*, data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnInit), "m_fnInit");
_property = record_property<bool(*)(data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnSetup), "m_fnSetup");
_property = record_property<void(*)(data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnStart), "m_fnStart");
_property = record_property<void(*)(data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnStop), "m_fnStop");
_property = record_property<void(*)(data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnWait), "m_fnWait");
_property = record_property<void(*)(data*, const char*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnWrite), "m_fnWrite");
_property = record_property<bool(*)(data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnTerminate), "m_fnTerminate");
_property = record_property<void(*)(Duckvil::Memory::ftable*, Duckvil::Memory::free_list_allocator*, data*)>(_data, _type, offsetof(Duckvil::Process::ftable, m_fnCleanup), "m_fnCleanup");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Process/Process.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
