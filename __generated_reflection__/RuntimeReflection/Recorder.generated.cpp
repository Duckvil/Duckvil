#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(7)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
_type = record_type<duckvil_recorderd_types>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "duckvil_recorderd_types");
_recordedTypes.push_back(_type);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_type_t*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_aTypes), "m_aTypes");
_property = record_property<size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_ullCount), "m_ullCount");
_property = record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_sFile), "m_sFile");
_property = record_property<Duckvil::PlugNPlay::__module_information*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_pModule), "m_pModule");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_uiRecorderID), "m_uiRecorderID");
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__recorder_meta_info>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__recorder_meta_info");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullKeyTypeID), "m_ullKeyTypeID");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullKeyTypeSize), "m_ullKeyTypeSize");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullValueTypeID), "m_ullValueTypeID");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullValueTypeSize), "m_ullValueTypeSize");
_property = record_property<const void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_pKeyData), "m_pKeyData");
_property = record_property<const void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_pValueData), "m_pValueData");
_property = record_property<property_traits>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_keyTraits), "m_keyTraits");
_property = record_property<property_traits>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_valueTraits), "m_valueTraits");
_property = record_property<uint8_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ucKeyTypeAlignment), "m_ucKeyTypeAlignment");
_property = record_property<uint8_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ucValueTypeAlignment), "m_ucValueTypeAlignment");
_type = record_type<Duckvil::RuntimeReflection::__recorder_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__recorder_ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, std::size_t, const char*, std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordType), "m_fnRecordType");
_property = record_property<DUCKVIL_RESOURCE(constructor_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), std::size_t, uint8_t*, Memory::Queue<__argument_t>&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordConstructor), "m_fnRecordConstructor");
_property = record_property<DUCKVIL_RESOURCE(destructor_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), uint8_t*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordDestructor), "m_fnRecordDestructor");
_property = record_property<DUCKVIL_RESOURCE(property_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), std::size_t, const char*, std::size_t, uintptr_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordProperty), "m_fnRecordProperty");
_property = record_property<DUCKVIL_RESOURCE(namespace_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordNamespace), "m_fnRecordNamespace");
_property = record_property<DUCKVIL_RESOURCE(inheritance_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), std::size_t, __protection)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordInheritance), "m_fnRecordInheritance");
_property = record_property<DUCKVIL_RESOURCE(function_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), __ifunction*, const char*, std::size_t, std::size_t, std::size_t, Memory::Queue<__argument_t>&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFunction), "m_fnRecordFunction");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), const __recorder_meta_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordTypeMeta), "m_fnRecordTypeMeta");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(property_t), const __recorder_meta_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordPropertyMeta), "m_fnRecordPropertyMeta");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), const __recorder_meta_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordConstructorMeta), "m_fnRecordConstructorMeta");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), uint32_t, const __recorder_meta_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordConstructorArgumentMeta), "m_fnRecordConstructorArgumentMeta");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), const __recorder_meta_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFunctionMeta), "m_fnRecordFunctionMeta");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(Memory::ftable*, Memory::free_list_allocator*, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), uint32_t, const __recorder_meta_info&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFunctionArgumentMeta), "m_fnRecordFunctionArgumentMeta");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "RuntimeReflection/Recorder.h", 7);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 7;
}
#endif
