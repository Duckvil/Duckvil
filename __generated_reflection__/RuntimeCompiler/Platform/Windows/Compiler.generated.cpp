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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::WindowsCompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "WindowsCompiler");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
record_inheritance<ICompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::WindowsCompiler, const Process::ftable&, Process::data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::RuntimeCompiler::WindowsCompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::vector<std::string>&, const Options&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::Compile, "Compile");
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddDefine, "AddDefine");
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddFlag, "AddFlag");
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, Flag>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddFlag, "AddFlag");
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddInclude, "AddInclude");
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddLibraryPath, "AddLibraryPath");
record_function<Duckvil::RuntimeCompiler::WindowsCompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::WindowsCompiler::AddLibrary, "AddLibrary");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "RuntimeCompiler/Platform/Windows/Compiler.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
