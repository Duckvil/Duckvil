#include "Memory/MemoryDebugger.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(23)
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
_namespaces.push_back("Memory");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("allocator");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Memory;
_type = record_type<Duckvil::Memory::allocator>(_data, "allocator");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Memory");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 23);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
#ifdef DUCKVIL_MEMORY_DEBUGGER
_enum = record_enum<duckvil_memory_allocator_type>(_data, _ntype, "duckvil_memory_allocator_type");
_enumElement = record_enum_element(_data, _ntype, _enum, duckvil_memory_allocator_type::duckvil_memory_allocator_type_free_list, "duckvil_memory_allocator_type_free_list");
_enumElement = record_enum_element(_data, _ntype, _enum, duckvil_memory_allocator_type::duckvil_memory_allocator_type_linear, "duckvil_memory_allocator_type_linear");
_enumElement = record_enum_element(_data, _ntype, _enum, duckvil_memory_allocator_type::duckvil_memory_allocator_type_vector, "duckvil_memory_allocator_type_vector");
_enumElement = record_enum_element(_data, _ntype, _enum, duckvil_memory_allocator_type::duckvil_memory_allocator_type_queue, "duckvil_memory_allocator_type_queue");
_enumElement = record_enum_element(_data, _ntype, _enum, duckvil_memory_allocator_type::duckvil_memory_allocator_type_stack, "duckvil_memory_allocator_type_stack");
_enumElement = record_enum_element(_data, _ntype, _enum, duckvil_memory_allocator_type::duckvil_memory_allocator_type_array, "duckvil_memory_allocator_type_array");
#endif
_namespaces.push_back("duckvil_memory_debug_info");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
#ifdef DUCKVIL_MEMORY_DEBUGGER
_type = record_type<duckvil_memory_debug_info>(_data, "duckvil_memory_debug_info");
_recordedTypes.push_back(_type);
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 23);
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<Duckvil::Memory::allocator*>(_data, _type, offsetof(duckvil_memory_debug_info, m_pAllocator), "m_pAllocator");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_debug_info*>(_data, _type, offsetof(duckvil_memory_debug_info, m_pParent), "m_pParent");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_allocator_type>(_data, _type, offsetof(duckvil_memory_debug_info, m_allocatorType), "m_allocatorType");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<char>(_data, _type, offsetof(duckvil_memory_debug_info, m_aLabel), "m_aLabel");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<std::vector<duckvil_memory_debug_info*>>(_data, _type, offsetof(duckvil_memory_debug_info, m_aOther), "m_aOther");
#endif
_namespaces.pop_back();
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Memory/MemoryDebugger.h", 23);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 23;
}
#endif
