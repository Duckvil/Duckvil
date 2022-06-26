#include "PlugNPlay/AutoLoader.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
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
_namespaces.push_back("PlugNPlay");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("AutoLoader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::PlugNPlay;
_type = record_type<Duckvil::PlugNPlay::AutoLoader>(_data, "AutoLoader");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "PlugNPlay");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_constructor = record_constructor<Duckvil::PlugNPlay::AutoLoader, const std::filesystem::path&>(_data, _type);
_destructor = record_destructor<Duckvil::PlugNPlay::AutoLoader>(_data, _type);
_function = record_function<Duckvil::PlugNPlay::AutoLoader, void, Memory::ftable*, Memory::free_list_allocator*, __module_information**, uint32_t*>(_data, _type, &Duckvil::PlugNPlay::AutoLoader::LoadAll, "LoadAll");
_function = record_function<Duckvil::PlugNPlay::AutoLoader, void, Memory::ftable*, Memory::free_list_allocator*, Memory::Vector<__module_information>*>(_data, _type, &Duckvil::PlugNPlay::AutoLoader::LoadAll, "LoadAll");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "PlugNPlay/AutoLoader.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
