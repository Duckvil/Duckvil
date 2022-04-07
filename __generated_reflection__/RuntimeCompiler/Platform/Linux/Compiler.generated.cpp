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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::LinuxCompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "LinuxCompiler");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
record_inheritance<ICompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::LinuxCompiler, const Process::ftable&, Process::data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::RuntimeCompiler::LinuxCompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, const std::vector<std::string>&, const Options&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::Compile, "Compile");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddDefine, "AddDefine");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, Flag>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddInclude, "AddInclude");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddLibraryPath, "AddLibraryPath");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::AddLibrary, "AddLibrary");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetDefines, "GetDefines");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetFlags, "GetFlags");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetIncludes, "GetIncludes");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetLibrariesPaths, "GetLibrariesPaths");
_function = record_function<Duckvil::RuntimeCompiler::LinuxCompiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::LinuxCompiler::GetLibraries, "GetLibraries");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "RuntimeCompiler/Platform/Linux/Compiler.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
