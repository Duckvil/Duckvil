#include "RuntimeCompiler/RuntimeCompiler.h"
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
_namespaces.push_back("RuntimeCompiler");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Compiler");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::Compiler>(_data, "Compiler");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "RuntimeCompiler");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
record_inheritance<ICompiler>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::Compiler, const Memory::FreeList&>(_data, _type);
_destructor = record_destructor<Duckvil::RuntimeCompiler::Compiler>(_data, _type);
_function = record_function<Duckvil::RuntimeCompiler::Compiler, bool>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::Setup, "Setup");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::vector<std::string>&, const Options&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::Compile, "Compile");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::AddDefine, "AddDefine");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, Flag>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::AddInclude, "AddInclude");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::AddLibraryPath, "AddLibraryPath");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::AddLibrary, "AddLibrary");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::GetDefines, "GetDefines");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::GetFlags, "GetFlags");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::GetIncludes, "GetIncludes");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::GetLibrariesPaths, "GetLibrariesPaths");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(_data, _type, &Duckvil::RuntimeCompiler::Compiler::GetLibraries, "GetLibraries");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RuntimeCompiler/RuntimeCompiler.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
