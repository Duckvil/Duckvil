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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::Compiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Compiler");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_constructor = record_constructor<Duckvil::RuntimeCompiler::Compiler, const Memory::FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::RuntimeCompiler::Compiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_function = record_function<Duckvil::RuntimeCompiler::Compiler, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::Setup, "Setup");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::vector<std::string>&, const Options&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::Compile, "Compile");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddDefine, "AddDefine");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, Flag>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddFlag, "AddFlag");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddInclude, "AddInclude");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddLibraryPath, "AddLibraryPath");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddLibrary, "AddLibrary");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::GetDefines, "GetDefines");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::GetFlags, "GetFlags");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::GetIncludes, "GetIncludes");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::GetLibrariesPaths, "GetLibrariesPaths");
_function = record_function<Duckvil::RuntimeCompiler::Compiler, const std::vector<std::string>&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::GetLibraries, "GetLibraries");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "RuntimeCompiler/RuntimeCompiler.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
