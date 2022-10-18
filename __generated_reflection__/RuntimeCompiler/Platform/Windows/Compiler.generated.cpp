#include "RuntimeCompiler/Platform/Windows/Compiler.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(2)
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
_namespaces.push_back("WindowsCompiler");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::WindowsCompiler>(_data, "WindowsCompiler");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeCompiler");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
record_inheritance<ICompiler>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::WindowsCompiler, const Process::ftable&, Process::data*>(_data, _type);
_destructor = record_destructor<Duckvil::RuntimeCompiler::WindowsCompiler>(_data, _type);
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::vector<std::string>&, const Options&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::Compile, "Compile");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddDefine, "AddDefine");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, Flag>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddInclude, "AddInclude");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddLibraryPath, "AddLibraryPath");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddLibrary, "AddLibrary");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::GetDefines, "GetDefines");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::GetFlags, "GetFlags");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::GetIncludes, "GetIncludes");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::GetLibrariesPaths, "GetLibrariesPaths");
_function = record_function<Duckvil::RuntimeCompiler::WindowsCompiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::GetLibraries, "GetLibraries");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeCompiler/Platform/Windows/Compiler.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
