#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(9)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_type = record_type<Duckvil::RuntimeReflection::__data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_type = record_type<Duckvil::RuntimeReflection::__recorder_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__recorder_ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_type = record_type<duckvil_frontend_reflection_context>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "duckvil_frontend_reflection_context");
_recordedTypes.push_back(_type);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<Duckvil::RuntimeReflection::__ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_frontend_reflection_context, m_pReflection), "m_pReflection");
_property = record_property<Duckvil::RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_frontend_reflection_context, m_pReflectionData), "m_pReflectionData");
_property = record_property<Duckvil::RuntimeReflection::__recorder_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_frontend_reflection_context, m_pRecorder), "m_pRecorder");
_type = record_type<Duckvil::RuntimeReflection::__variant>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__variant");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_constructor = record_constructor<Duckvil::RuntimeReflection::__variant>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_ullSize), "m_ullSize");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_ullTypeID), "m_ullTypeID");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_pData), "m_pData");
_property = record_property<property_traits>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_traits), "m_traits");
_type = record_type<Duckvil::RuntimeReflection::__data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__data");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<DUCKVIL_SLOT_ARRAY(__type_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__data, m_aTypes), "m_aTypes");
_property = record_property<DUCKVIL_SLOT_ARRAY(__object_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__data, m_aObjects), "m_aObjects");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__data, m_pEvents), "m_pEvents");
_type = record_type<Duckvil::RuntimeReflection::__ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 9);
_property = record_property<__data*(*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInit), "m_fnInit");
_property = record_property<duckvil_frontend_reflection_context*(*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnCreateContext), "m_fnCreateContext");
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(__data*, const char*, std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeHandleByName), "m_fnGetTypeHandleByName");
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(__data*, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeHandleByTypeID), "m_fnGetTypeHandleByTypeID");
_property = record_property<const __type_t&(*)(__data*, const DUCKVIL_RESOURCE(type_t)&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetType), "m_fnGetType");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(type_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypes), "m_fnGetTypes");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(constructor_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructors), "m_fnGetConstructors");
_property = record_property<DUCKVIL_RESOURCE(constructor_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorHandleByTypeID), "m_fnGetConstructorHandleByTypeID");
_property = record_property<const __constructor_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructor), "m_fnGetConstructor");
_property = record_property<DUCKVIL_RESOURCE(destructor_t)(*)(__data*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetDestructorHandle), "m_fnGetDestructorHandle");
_property = record_property<const void*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(property_t), const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyByHandle), "m_fnGetPropertyByHandle");
_property = record_property<const void*(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyByName), "m_fnGetPropertyByName");
_property = record_property<DUCKVIL_RESOURCE(property_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyHandleByName), "m_fnGetPropertyHandleByName");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(function_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctions), "m_fnGetFunctions");
_property = record_property<DUCKVIL_RESOURCE(function_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t, std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionHandle), "m_fnGetFunctionHandle");
_property = record_property<__ifunction*(*)(__data*, DUCKVIL_RESOURCE(type_t), const char*, std::size_t, std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionCallback), "m_fnGetFunctionCallback");
_property = record_property<__ifunction*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionCallbackByHandle), "m_fnGetFunctionCallbackByHandle");
_property = record_property<const __function_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionByHandle), "m_fnGetFunctionByHandle");
_property = record_property<const void*(*)(__data*, const char*, std::size_t, std::size_t, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetProperty), "m_fnGetProperty");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(property_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetProperties), "m_fnGetProperties");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(argument_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArguments), "m_fnGetConstructorArguments");
_property = record_property<const __argument_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), DUCKVIL_RESOURCE(argument_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArgument), "m_fnGetConstructorArgument");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(argument_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArguments), "m_fnGetFunctionArguments");
_property = record_property<const __argument_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), DUCKVIL_RESOURCE(argument_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArgument), "m_fnGetFunctionArgument");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)>(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetInheritances), "m_fnGetInheritances");
_property = record_property<const __inheritance_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(inheritance_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetInheritance), "m_fnGetInheritance");
_property = record_property<bool(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInherits), "m_fnInherits");
_property = record_property<bool(*)(__data*, DUCKVIL_RESOURCE(type_t), std::size_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInheritsByTypeID), "m_fnInheritsByTypeID");
_property = record_property<DUCKVIL_RESOURCE(variant_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeMetaHandle), "m_fnGetTypeMetaHandle");
_property = record_property<void*(*)(__data*, DUCKVIL_RESOURCE(type_t), const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeMetaValue), "m_fnGetTypeMetaValue");
_property = record_property<const __variant&(*)(__data*, DUCKVIL_RESOURCE(type_t), const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeMetaVariant), "m_fnGetTypeMetaVariant");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(meta_t)>(*)(const Memory::FreeList&, __data*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeMetas), "m_fnGetTypeMetas");
_property = record_property<DUCKVIL_RESOURCE(variant_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), uint32_t, const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArgumentMetaHandle), "m_fnGetConstructorArgumentMetaHandle");
_property = record_property<void*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), uint32_t, const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArgumentMetaValue), "m_fnGetConstructorArgumentMetaValue");
_property = record_property<const __variant&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), uint32_t, const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArgumentMetaVariant), "m_fnGetConstructorArgumentMetaVariant");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(meta_t)>(*)(const Memory::FreeList&, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructorArgumentMetas), "m_fnGetConstructorArgumentMetas");
_property = record_property<DUCKVIL_RESOURCE(variant_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), uint32_t, const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArgumentMetaHandle), "m_fnGetFunctionArgumentMetaHandle");
_property = record_property<void*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), uint32_t, const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArgumentMetaValue), "m_fnGetFunctionArgumentMetaValue");
_property = record_property<const __variant&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), uint32_t, const void*, const std::size_t&, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArgumentMetaVariant), "m_fnGetFunctionArgumentMetaVariant");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(meta_t)>(*)(const Memory::FreeList&, __data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), uint32_t)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetFunctionArgumentMetas), "m_fnGetFunctionArgumentMetas");
_property = record_property<DUCKVIL_RESOURCE(meta_t)(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, void*, std::size_t, std::size_t, uint8_t, property_traits, const void*, std::size_t, std::size_t, uint8_t, property_traits, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnAddObjectMeta), "m_fnAddObjectMeta");
_property = record_property<const __variant_t&(*)(__data*, void*, std::size_t, std::size_t, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetObjectMeta), "m_fnGetObjectMeta");
_property = record_property<void(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, void*, std::size_t, std::size_t, const void*, std::size_t, std::size_t, uint8_t, property_traits, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnSetObjectMeta), "m_fnSetObjectMeta");
_property = record_property<void(*)(__data*, void*, std::size_t, std::size_t, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnRemoveObjectMeta), "m_fnRemoveObjectMeta");
_property = record_property<void(*)(__data*, Memory::ftable*, Memory::free_list_allocator*, void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnClearObjectMetas), "m_fnClearObjectMetas");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "RuntimeReflection/RuntimeReflection.h", 9);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 9;
}
#endif
