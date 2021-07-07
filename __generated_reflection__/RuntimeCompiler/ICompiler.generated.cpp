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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::Options>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Options");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aFlags), "m_aFlags");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aDefines), "m_aDefines");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aIncludes), "m_aIncludes");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aLibrariesPaths), "m_aLibrariesPaths");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::Options, m_aLibraries), "m_aLibraries");
_type = record_type<Duckvil::RuntimeCompiler::ICompiler>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ICompiler");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::vector<std::string>&, const Options&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::Compile, "Compile");
record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::AddDefine, "AddDefine");
record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::AddFlag, "AddFlag");
record_function<Duckvil::RuntimeCompiler::ICompiler, void, Flag>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::AddFlag, "AddFlag");
record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::AddInclude, "AddInclude");
record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::AddLibraryPath, "AddLibraryPath");
record_function<Duckvil::RuntimeCompiler::ICompiler, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::ICompiler::AddLibrary, "AddLibrary");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "RuntimeCompiler/ICompiler.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
