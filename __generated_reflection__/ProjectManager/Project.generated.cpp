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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::ProjectManager;
_type = record_type<Duckvil::ProjectManager::project>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "project");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "ProjectManager");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<Memory::Vector<duckvil_recorderd_types>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_aTypes), "m_aTypes");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_pObject), "m_pObject");
_property = record_property<Utils::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_sPath), "m_sPath");
_property = record_property<PlugNPlay::__module_information>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_module), "m_module");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_uiTypeHandleID), "m_uiTypeHandleID");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_uiInitFunctionHandleID), "m_uiInitFunctionHandleID");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::ProjectManager::project, m_uiUpdateFunctionHandleID), "m_uiUpdateFunctionHandleID");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "ProjectManager/Project.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
