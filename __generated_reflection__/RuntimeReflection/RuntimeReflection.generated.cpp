#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(6)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__variant>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__variant");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_ullSize), "m_ullSize");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_ullTypeID), "m_ullTypeID");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_pData), "m_pData");
_property = record_property<__traits>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__variant, m_traits), "m_traits");
_type = record_type<Duckvil::RuntimeReflection::__ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
_property = record_property<__data*(*)(Memory::IMemory*, Memory::__free_list_allocator*, __ftable*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInit), "m_fnInit");
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(__data*, const char)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeHandleByName), "m_fnGetTypeHandleByName");
_property = record_property<DUCKVIL_RESOURCE(type_t)(*)(__data*, const std::size_t&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypeHandleByTypeID), "m_fnGetTypeHandleByTypeID");
_property = record_property<const __type_t&(*)(__data*, const DUCKVIL_RESOURCE(type_t)&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetType), "m_fnGetType");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(type_t)>(*)(__data*, Memory::IMemory*, Memory::__free_list_allocator*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetTypes), "m_fnGetTypes");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(constructor_t)>(*)(__data*, Memory::IMemory*, Memory::__free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetConstructors), "m_fnGetConstructors");
_property = record_property<void*(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(property_t), const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyByHandle), "m_fnGetPropertyByHandle");
_property = record_property<void*(*)(__data*, DUCKVIL_RESOURCE(type_t), const char, const void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyByName), "m_fnGetPropertyByName");
_property = record_property<DUCKVIL_RESOURCE(property_t)(*)(__data*, DUCKVIL_RESOURCE(type_t), const char)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetPropertyHandleByName), "m_fnGetPropertyHandleByName");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(argument_t)>(*)(__data*, Memory::IMemory*, Memory::__free_list_allocator*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetArguments), "m_fnGetArguments");
_property = record_property<const __argument_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), DUCKVIL_RESOURCE(argument_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetArgument), "m_fnGetArgument");
_property = record_property<Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)>(*)(__data*, Memory::IMemory*, Memory::__free_list_allocator*, DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetInheritances), "m_fnGetInheritances");
_property = record_property<const __inheritance_t&(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(inheritance_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnGetInheritance), "m_fnGetInheritance");
_property = record_property<bool(*)(__data*, DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(type_t))>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__ftable, m_fnInherits), "m_fnInherits");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "RuntimeReflection/RuntimeReflection.h", 6 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
