#include "ScriptEngine/Language.h"
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
_namespaces.push_back("ScriptEngine");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Language");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::ScriptEngine;
_type = record_type<Duckvil::ScriptEngine::Language>(_data, "Language");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "ScriptEngine");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_function = record_function<Duckvil::ScriptEngine::Language, bool>(_data, _type, &Duckvil::ScriptEngine::Language::Setup, "Setup");
_function = record_function<Duckvil::ScriptEngine::Language, bool>(_data, _type, &Duckvil::ScriptEngine::Language::Init, "Init");
_function = record_function<Duckvil::ScriptEngine::Language, void>(_data, _type, &Duckvil::ScriptEngine::Language::Update, "Update");
_function = record_function<Duckvil::ScriptEngine::Language, bool>(_data, _type, &Duckvil::ScriptEngine::Language::Shutdown, "Shutdown");
_function = record_function<Duckvil::ScriptEngine::Language, void*, char const*, char const**, uint32_t>(_data, _type, &Duckvil::ScriptEngine::Language::GetType, "GetType");
_function = record_function<Duckvil::ScriptEngine::Language, void*, void*>(_data, _type, &Duckvil::ScriptEngine::Language::CreateObject, "CreateObject");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "ScriptEngine/Language.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
