#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
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
_namespaces.push_back("duckvil_recorderd_types");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<duckvil_recorderd_types>(_data, "duckvil_recorderd_types");
_recordedTypes.push_back(_type);
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_type_t*>(_data, _type, offsetof(duckvil_recorderd_types, m_aTypes), "m_aTypes");
_property = record_property<size_t>(_data, _type, offsetof(duckvil_recorderd_types, m_ullCount), "m_ullCount");
_property = record_property<char const*>(_data, _type, offsetof(duckvil_recorderd_types, m_sFile), "m_sFile");
_property = record_property<Duckvil::PlugNPlay::__module_information const*>(_data, _type, offsetof(duckvil_recorderd_types, m_pModule), "m_pModule");
_property = record_property<uint32_t>(_data, _type, offsetof(duckvil_recorderd_types, m_uiRecorderID), "m_uiRecorderID");
_namespaces.pop_back();
_namespaces.push_back("duckvil_runtime_reflection_recorder_stuff");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<duckvil_runtime_reflection_recorder_stuff>(_data, "duckvil_runtime_reflection_recorder_stuff");
_recordedTypes.push_back(_type);
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_property = record_property<Duckvil::Memory::ftable const*>(_data, _type, offsetof(duckvil_runtime_reflection_recorder_stuff, _pMemoryInterface), "_pMemoryInterface");
_property = record_property<Duckvil::Memory::free_list_allocator*>(_data, _type, offsetof(duckvil_runtime_reflection_recorder_stuff, _pAllocator), "_pAllocator");
_property = record_property<Duckvil::RuntimeReflection::__recorder_ftable const*>(_data, _type, offsetof(duckvil_runtime_reflection_recorder_stuff, _pFunctions), "_pFunctions");
_property = record_property<Duckvil::RuntimeReflection::__data*>(_data, _type, offsetof(duckvil_runtime_reflection_recorder_stuff, _pData), "_pData");
_namespaces.pop_back();
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("RuntimeReflection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("__recorder_meta_info");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__recorder_meta_info>(_data, "__recorder_meta_info");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullKeyTypeID), "m_ullKeyTypeID");
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullKeyTypeSize), "m_ullKeyTypeSize");
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullValueTypeID), "m_ullValueTypeID");
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullValueTypeSize), "m_ullValueTypeSize");
_property = record_property<void const*>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_pKeyData), "m_pKeyData");
_property = record_property<void const*>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_pValueData), "m_pValueData");
_property = record_property<property_traits>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_keyTraits), "m_keyTraits");
_property = record_property<property_traits>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_valueTraits), "m_valueTraits");
_property = record_property<uint8_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ucKeyTypeAlignment), "m_ucKeyTypeAlignment");
_property = record_property<uint8_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ucValueTypeAlignment), "m_ucValueTypeAlignment");
_namespaces.pop_back();
_namespaces.push_back("remove_all");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("remove_all");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("recorders_count_getter_not_found");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::recorders_count_getter_not_found>(_data, "recorders_count_getter_not_found");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_inheritance<std::exception>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeReflection::recorders_count_getter_not_found>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("__recorder_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__recorder_ftable>(_data, "__recorder_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_type_t(*)(duckvil_runtime_reflection_recorder_stuff const&,std::size_t,char const*,std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordType), "m_fnRecordType");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_constructor_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,std::size_t,uint8_t*,Memory::Queue<__argument_t>&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordConstructor), "m_fnRecordConstructor");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_destructor_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,uint8_t*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordDestructor), "m_fnRecordDestructor");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_property_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,std::size_t,char const*,std::size_t,uintptr_t,property_traits const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordPropertyByOffset), "m_fnRecordPropertyByOffset");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_property_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,std::size_t,char const*,std::size_t,uintptr_t,property_traits const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordPropertyByAddress), "m_fnRecordPropertyByAddress");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_namespace_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,char const*,std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordNamespace), "m_fnRecordNamespace");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_inheritance_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,std::size_t,__protection)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordInheritance), "m_fnRecordInheritance");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_function_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,__ifunction*,char const*,std::size_t,std::size_t,std::size_t,Memory::Queue<__argument_t>&,function_traits const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFunction), "m_fnRecordFunction");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordTypeMeta), "m_fnRecordTypeMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,Duckvil::RuntimeReflection::__duckvil_resource_property_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordPropertyMeta), "m_fnRecordPropertyMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,Duckvil::RuntimeReflection::__duckvil_resource_constructor_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordConstructorMeta), "m_fnRecordConstructorMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,Duckvil::RuntimeReflection::__duckvil_resource_constructor_t,uint32_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordConstructorArgumentMeta), "m_fnRecordConstructorArgumentMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,Duckvil::RuntimeReflection::__duckvil_resource_function_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFunctionMeta), "m_fnRecordFunctionMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,Duckvil::RuntimeReflection::__duckvil_resource_function_t,uint32_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFunctionArgumentMeta), "m_fnRecordFunctionArgumentMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_meta_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_type_t,Duckvil::RuntimeReflection::__duckvil_resource_enum_t,__recorder_meta_info const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordEnumMeta), "m_fnRecordEnumMeta");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t(*)(duckvil_runtime_reflection_recorder_stuff const&,std::vector<const char *> const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordNType), "m_fnRecordNType");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_enum_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_ntype_t,std::size_t,char const*,std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordNEnum), "m_fnRecordNEnum");
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_enum_element_t(*)(duckvil_runtime_reflection_recorder_stuff const&,Duckvil::RuntimeReflection::__duckvil_resource_ntype_t,Duckvil::RuntimeReflection::__duckvil_resource_enum_t,void*,std::size_t,char const*,std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordNEnumElement), "m_fnRecordNEnumElement");
_property = record_property<duckvil_recorderd_types(*)(duckvil_runtime_reflection_recorder_stuff const&,PlugNPlay::__module_information const&,RuntimeReflection::RecordFunction)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordFile), "m_fnRecordFile");
_property = record_property<Memory::Vector<duckvil_recorderd_types>(*)(duckvil_runtime_reflection_recorder_stuff const&,PlugNPlay::__module const&,PlugNPlay::__module_information const&,Memory::FreeList const&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__recorder_ftable, m_fnRecordModule), "m_fnRecordModule");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeReflection/Recorder.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
