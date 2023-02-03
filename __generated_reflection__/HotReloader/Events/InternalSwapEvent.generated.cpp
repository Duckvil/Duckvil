#include "HotReloader/Events/InternalSwapEvent.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
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
_namespaces.push_back("HotReloader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("InternalSwapEvent");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::HotReloader;
_type = record_type<Duckvil::HotReloader::InternalSwapEvent>(_data, "InternalSwapEvent");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<Memory::Vector<RuntimeCompilerSystem::hot_object>*>(_data, _type, offsetof(Duckvil::HotReloader::InternalSwapEvent, m_pHotObjects), "m_pHotObjects");
_property = record_property<duckvil_recorderd_types>(_data, _type, offsetof(Duckvil::HotReloader::InternalSwapEvent, m_recordedTypes), "m_recordedTypes");
_property = record_property<void(*)(Memory::Vector<RuntimeCompilerSystem::hot_object>*,duckvil_recorderd_types&)>(_data, _type, offsetof(Duckvil::HotReloader::InternalSwapEvent, m_fnSwap), "m_fnSwap");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/Events/InternalSwapEvent.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
