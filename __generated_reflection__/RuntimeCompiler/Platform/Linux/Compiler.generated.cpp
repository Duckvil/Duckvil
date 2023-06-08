#include "RuntimeCompiler/Platform/Linux/Compiler.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
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
_namespaces.push_back("LinuxCompiler");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::LinuxCompiler>(_data, "LinuxCompiler");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeCompiler");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
record_inheritance<ICompiler>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::LinuxCompiler, Process::ftable const&, Process::data*>(_data, _type);
_destructor = record_destructor<Duckvil::RuntimeCompiler::LinuxCompiler>(_data, _type);
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, std::vector<std::string> const&, Options const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::Compile, "Compile");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, std::string const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddDefine, "AddDefine");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, std::string const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, Flag>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, std::string const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddInclude, "AddInclude");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, std::string const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddLibraryPath, "AddLibraryPath");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, std::string const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddLibrary, "AddLibrary");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, std::vector<std::string> const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetDefines, "GetDefines");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, std::vector<std::string> const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetFlags, "GetFlags");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, std::vector<std::string> const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetIncludes, "GetIncludes");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, std::vector<std::string> const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetLibrariesPaths, "GetLibrariesPaths");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, std::vector<std::string> const&>(_data, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetLibraries, "GetLibraries");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeCompiler/Platform/Linux/Compiler.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
