#include "RuntimeReflection/Resources.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(10)
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
_namespaces.push_back("RuntimeReflection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("__ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__ftable>(_data, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_namespaces.pop_back();
_namespaces.push_back("__data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__data>(_data, "__data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_namespaces.pop_back();
_namespaces.push_back("__recorder_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__recorder_ftable>(_data, "__recorder_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.push_back("duckvil_frontend_reflection_context");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<duckvil_frontend_reflection_context>(_data, "duckvil_frontend_reflection_context");
_recordedTypes.push_back(_type);
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_property = record_property<Duckvil::RuntimeReflection::__ftable*>(_data, _type, offsetof(duckvil_frontend_reflection_context, m_pReflection), "m_pReflection");
_property = record_property<Duckvil::RuntimeReflection::__data*>(_data, _type, offsetof(duckvil_frontend_reflection_context, m_pReflectionData), "m_pReflectionData");
_property = record_property<Duckvil::RuntimeReflection::__recorder_ftable*>(_data, _type, offsetof(duckvil_frontend_reflection_context, m_pRecorder), "m_pRecorder");
_namespaces.pop_back();
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("RuntimeReflection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::RuntimeReflection::__protection>(_data, _ntype, "__protection");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__protection::__protection_public, "__protection_public");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__protection::__protection_protected, "__protection_protected");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__protection::__protection_private, "__protection_private");
_enum = record_enum<Duckvil::RuntimeReflection::__variant_owner>(_data, _ntype, "__variant_owner");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_object, "__variant_owner_object");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_type, "__variant_owner_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_property, "__variant_owner_property");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_constructor, "__variant_owner_constructor");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_constructor_argument, "__variant_owner_constructor_argument");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_function, "__variant_owner_function");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeReflection::__variant_owner::__variant_owner_function_argument, "__variant_owner_function_argument");
_namespaces.push_back("__variant");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__variant>(_data, "__variant");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_constructor = record_constructor<Duckvil::RuntimeReflection::__variant>(_data, _type);
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_ullSize), "m_ullSize");
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_ullTypeID), "m_ullTypeID");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_pData), "m_pData");
_property = record_property<property_traits>(_data, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_traits), "m_traits");
_namespaces.pop_back();
_namespaces.push_back("__data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__data>(_data, "__data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_property = record_property<DUCKVIL_SLOT_ARRAY(__type_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__data, m_aTypes), "m_aTypes");
_property = record_property<DUCKVIL_SLOT_ARRAY(__ntype_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__data, m_aNTypes), "m_aNTypes");
_property = record_property<DUCKVIL_SLOT_ARRAY(__object_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__data, m_aObjects), "m_aObjects");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::RuntimeReflection::__data, m_pEvents), "m_pEvents");
_property = record_property<std::unordered_map<std::size_t,__duckvil_resource_type_t>*>(_data, _type, offsetof(Duckvil::RuntimeReflection::__data, m_pTypesByTypeID), "m_pTypesByTypeID");
_namespaces.pop_back();
_namespaces.push_back("__ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeReflection::__ftable>(_data, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeReflection");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 10);
_property = record_property<__data*(*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInit), "m_fnInit");
_property = record_property<duckvil_frontend_reflection_context*(*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnCreateContext), "m_fnCreateContext");
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(__data*, const char*, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeHandleByName), "m_fnGetTypeHandleByName");
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(__data*, const std::size_t&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeHandleByTypeID), "m_fnGetTypeHandleByTypeID");
_property = record_property<const __type_t&(*)(__data*, const DUCKVIL_RESOURCE(type_t)&)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeByHandle), "m_fnGetTypeByHandle");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(type_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypes), "m_fnGetTypes");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(constructor_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructors), "m_fnGetConstructors");
_property = record_property<DUCKVIL_RESOURCE(constructor_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorHandleByTypeID), "m_fnGetConstructorHandleByTypeID");
_property = record_property<const __constructor_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructor), "m_fnGetConstructor");
_property = record_property<DUCKVIL_RESOURCE(destructor_t)(*)(__data*, DUCKVIL_RESOURCE(type_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetDestructorHandle), "m_fnGetDestructorHandle");
_property = record_property<const void*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(property_t), const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyValueByHandle), "m_fnGetPropertyValueByHandle");
_property = record_property<const void*(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t, const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyValueByName), "m_fnGetPropertyValueByName");
_property = record_property<DUCKVIL_RESOURCE(property_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyHandleByName), "m_fnGetPropertyHandleByName");
_property = record_property<const void*(*)(__data*, const char*, std::size_t, std::size_t, const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyValue), "m_fnGetPropertyValue");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(property_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetProperties), "m_fnGetProperties");
_property = record_property<const __property_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(property_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyByHandle), "m_fnGetPropertyByHandle");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(function_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctions), "m_fnGetFunctions");
_property = record_property<DUCKVIL_RESOURCE(function_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionHandle), "m_fnGetFunctionHandle");
_property = record_property<DUCKVIL_RESOURCE(function_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionHandleByPointer), "m_fnGetFunctionHandleByPointer");
_property = record_property<__ifunction*(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionCallback), "m_fnGetFunctionCallback");
_property = record_property<__ifunction*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionCallbackByHandle), "m_fnGetFunctionCallbackByHandle");
_property = record_property<const __function_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionByHandle), "m_fnGetFunctionByHandle");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(argument_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArguments), "m_fnGetConstructorArguments");
_property = record_property<const __argument_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), DUCKVIL_RESOURCE(argument_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArgument), "m_fnGetConstructorArgument");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(argument_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArguments), "m_fnGetFunctionArguments");
_property = record_property<const __argument_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), DUCKVIL_RESOURCE(argument_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArgument), "m_fnGetFunctionArgument");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetInheritances), "m_fnGetInheritances");
_property = record_property<const __inheritance_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(inheritance_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetInheritance), "m_fnGetInheritance");
_property = record_property<bool(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(type_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInherits), "m_fnInherits");
_property = record_property<bool(*)(__data*, DUCKVIL_RESOURCE(type_t), std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInheritsByTypeID), "m_fnInheritsByTypeID");
_property = record_property<const __variant_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(variant_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetVariantKeyByHandle), "m_fnGetVariantKeyByHandle");
_property = record_property<const __variant_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(variant_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetVariantValueByHandle), "m_fnGetVariantValueByHandle");
_property = record_property<DUCKVIL_RESOURCE(ntype_t)(*)(__data*, __data*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNTypeHandleByNamespaces), "m_fnGetNTypeHandleByNamespaces");
_property = record_property<const __ntype_t&(*)(__data*, DUCKVIL_RESOURCE(ntype_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNTypeByHandle), "m_fnGetNTypeByHandle");
_property = record_property<DUCKVIL_RESOURCE(enum_t)(*)(__data*, DUCKVIL_RESOURCE(ntype_t), const char*, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumHandleByName), "m_fnGetNEnumHandleByName");
_property = record_property<DUCKVIL_RESOURCE(enum_t)(*)(__data*, DUCKVIL_RESOURCE(ntype_t), std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumHandleByTypeID), "m_fnGetNEnumHandleByTypeID");
_property = record_property<const __enum_t&(*)(__data*, DUCKVIL_RESOURCE(ntype_t), DUCKVIL_RESOURCE(enum_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumByHandle), "m_fnGetNEnumByHandle");
_property = record_property<const __enum_t&(*)(__data*, std::size_t, DUCKVIL_RESOURCE(ntype_t)*, DUCKVIL_RESOURCE(enum_t)*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumByTypeID), "m_fnGetNEnumByTypeID");
_property = record_property<DUCKVIL_RESOURCE(enum_element_t)(*)(__data*, DUCKVIL_RESOURCE(ntype_t), DUCKVIL_RESOURCE(enum_t), const char*, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumElementHandleByName), "m_fnGetNEnumElementHandleByName");
_property = record_property<DUCKVIL_RESOURCE(enum_element_t)(*)(__data*, DUCKVIL_RESOURCE(ntype_t), DUCKVIL_RESOURCE(enum_t), const void*, std::size_t)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumElementHandleByValue), "m_fnGetNEnumElementHandleByValue");
_property = record_property<const __enum_element_t&(*)(__data*, DUCKVIL_RESOURCE(ntype_t), DUCKVIL_RESOURCE(enum_t), DUCKVIL_RESOURCE(enum_element_t))>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetNEnumElementByHandle), "m_fnGetNEnumElementByHandle");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, void*, std::size_t, std::size_t, uint8_t, property_traits, const void*, std::size_t, std::size_t, uint8_t, property_traits, const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnAddObjectMeta), "m_fnAddObjectMeta");
_property = record_property<const __variant_t&(*)(__data*, void*, std::size_t, std::size_t, const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetObjectMeta), "m_fnGetObjectMeta");
_property = record_property<void(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, void*, std::size_t, std::size_t, const void*, std::size_t, std::size_t, uint8_t, property_traits, const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnSetObjectMeta), "m_fnSetObjectMeta");
_property = record_property<void(*)(__data*, void*, std::size_t, std::size_t, const void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnRemoveObjectMeta), "m_fnRemoveObjectMeta");
_property = record_property<void(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, void*)>(_data, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnClearObjectMetas), "m_fnClearObjectMetas");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeReflection/Resources.h", 10);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 10;
}
#endif
