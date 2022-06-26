#include "ProjectManager/Project.h"
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
_namespaces.push_back("ProjectManager");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("project");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::ProjectManager;
_type = record_type<Duckvil::ProjectManager::project>(_data, "project");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "ProjectManager");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<Memory::Vector<duckvil_recorderd_types>>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_aTypes), "m_aTypes");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_pObject), "m_pObject");
_property = record_property<Utils::string>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_sPath), "m_sPath");
_property = record_property<PlugNPlay::__module_information>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_module), "m_module");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_uiTypeHandleID), "m_uiTypeHandleID");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_uiInitFunctionHandleID), "m_uiInitFunctionHandleID");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(_data, _type, offsetof(Duckvil::ProjectManager::project, m_uiUpdateFunctionHandleID), "m_uiUpdateFunctionHandleID");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "ProjectManager/Project.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
