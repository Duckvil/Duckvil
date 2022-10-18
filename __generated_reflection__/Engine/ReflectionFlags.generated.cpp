#include "Engine/ReflectionFlags.h"
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
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::ReflectionFlags>(_data, _ntype, "ReflectionFlags");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ReflectionFlags::ReflectionFlags_EngineSystem, "ReflectionFlags_EngineSystem");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ReflectionFlags::ReflectionFlags_UserSystem, "ReflectionFlags_UserSystem");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule, "ReflectionFlags_ReflectionModule");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ReflectionFlags::ReflectionFlags_AutoEventsAdding, "ReflectionFlags_AutoEventsAdding");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::ReflectionFlags::ReflectionFlags_AutoInstantiate, "ReflectionFlags_AutoInstantiate");
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/ReflectionFlags.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
