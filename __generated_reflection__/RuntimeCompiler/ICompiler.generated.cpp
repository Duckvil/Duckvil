#include "RuntimeCompiler/ICompiler.h"
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
_namespaces.push_back("RuntimeCompiler");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Options");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::Options>(_data, "Options");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeCompiler");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aFlags), "m_aFlags");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aDefines), "m_aDefines");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aIncludes), "m_aIncludes");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aLibrariesPaths), "m_aLibrariesPaths");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aLibraries), "m_aLibraries");
_namespaces.pop_back();
_enum = record_enum<Duckvil::RuntimeCompiler::Flag>(_data, _ntype, "Flag");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeCompiler::Flag::Flag_None, "Flag_None");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeCompiler::Flag::Flag_SharedLibrary, "Flag_SharedLibrary");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeCompiler::Flag::Flag_StaticLibrary, "Flag_StaticLibrary");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::RuntimeCompiler::Flag::Flag_DebugInfo, "Flag_DebugInfo");
_namespaces.push_back("ICompiler");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::RuntimeCompiler::ICompiler>(_data, "ICompiler");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeCompiler");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::vector<std::string>&, const Options&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::Compile, "Compile");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::AddDefine, "AddDefine");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, Flag>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::AddInclude, "AddInclude");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::AddLibraryPath, "AddLibraryPath");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::AddLibrary, "AddLibrary");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::GetDefines, "GetDefines");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::GetFlags, "GetFlags");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::GetIncludes, "GetIncludes");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::GetLibrariesPaths, "GetLibrariesPaths");
_function = record_function<Duckvil::RuntimeCompiler::ICompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::ICompiler::GetLibraries, "GetLibraries");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeCompiler/ICompiler.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
