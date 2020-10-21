#include "RuntimeReflection\Generator.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
using namespace Duckvil::RuntimeReflection;
_type = record_type<__generator_namespace>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__generator_namespace");
record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__generator_namespace, m_sName), "m_sName");
record_property<std::vector<Parser::__ast_meta>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__generator_namespace, m_aMeta), "m_aMeta");
_type = record_type<__generator_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__generator_data");
record_property<std::queue<__generator_namespace>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__generator_data, m_aNamespaces), "m_aNamespaces");
record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__generator_data, m_bWasNamespaces), "m_bWasNamespaces");
record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__generator_data, m_sInclude), "m_sInclude");
record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__generator_data, m_uiRecorderIndex), "m_uiRecorderIndex");
_type = record_type<__generator_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__generator_ftable");
}
