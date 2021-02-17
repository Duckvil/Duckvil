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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::Compiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Compiler");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_constructor = record_constructor<Duckvil::RuntimeCompiler::Compiler, const Memory::FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::RuntimeCompiler::Compiler, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::Setup, "Setup");
record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::vector<std::string>&, const Options&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::Compile, "Compile");
record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddDefine, "AddDefine");
record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddFlag, "AddFlag");
record_function<Duckvil::RuntimeCompiler::Compiler, void, Flag>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddFlag, "AddFlag");
record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddInclude, "AddInclude");
record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddLibraryPath, "AddLibraryPath");
record_function<Duckvil::RuntimeCompiler::Compiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::Compiler::AddLibrary, "AddLibrary");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "RuntimeCompiler/RuntimeCompiler.h", 3 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
