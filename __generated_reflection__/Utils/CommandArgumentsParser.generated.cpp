#include "Utils\CommandArgumentsParser.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
using namespace Duckvil::Utils;
_type = record_type<CommandArgumentsParser>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "CommandArgumentsParser");
record_constructor<CommandArgumentsParser, int, char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &CommandArgumentsParser::Parse, "Parse");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &CommandArgumentsParser::operator, "operator");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &CommandArgumentsParser::operator, "operator");
_type = record_type<CommandArgumentsParser::Descriptor>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Descriptor");
record_constructor<CommandArgumentsParser::Descriptor>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_constructor<CommandArgumentsParser::Descriptor, A, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_uiIndex), "m_uiIndex");
record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_sOption), "m_sOption");
record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_sResult), "m_sResult");
record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_bIsSet), "m_bIsSet");
}
