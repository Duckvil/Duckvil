#include "Utils\CommandArgumentsParser.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::Utils;
_type = record_type<CommandArgumentsParser>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "CommandArgumentsParser");
_constructor = record_constructor<CommandArgumentsParser, int, char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &CommandArgumentsParser::Parse, "Parse");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &CommandArgumentsParser::operator, "operator");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &CommandArgumentsParser::operator, "operator");
_type = record_type<CommandArgumentsParser::Descriptor>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Descriptor");
_constructor = record_constructor<CommandArgumentsParser::Descriptor>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<CommandArgumentsParser::Descriptor, A, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_uiIndex), "m_uiIndex");
_property = record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_sOption), "m_sOption");
_property = record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_sResult), "m_sResult");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(CommandArgumentsParser::Descriptor, m_bIsSet), "m_bIsSet");
}
