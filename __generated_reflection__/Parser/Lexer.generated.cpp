#include "Parser\Lexer.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::Parser;
_type = record_type<__lexer_token>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__lexer_token");
_property = record_property<__lexer_token_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_token, m_tokenType), "m_tokenType");
_type = record_type<__lexer_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__lexer_data");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_aLines), "m_aLines");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_sCurrentLine), "m_sCurrentLine");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_uiCurrentLine), "m_uiCurrentLine");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_uiCurrentCharacterIndex), "m_uiCurrentCharacterIndex");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_sToken), "m_sToken");
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_cCurrentCharacter), "m_cCurrentCharacter");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_bWasSpace), "m_bWasSpace");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_bSpace), "m_bSpace");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_bNewLine), "m_bNewLine");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__lexer_data, m_bEnd), "m_bEnd");
_type = record_type<__lexer_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__lexer_ftable");
}
