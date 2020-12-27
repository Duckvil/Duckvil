#include "Parser\Lexer.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
Duckvil::Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemoryInterface, _pAllocator, 1);
using namespace Duckvil::Parser;
_type = record_type<Duckvil::Parser::__lexer_token>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__lexer_token");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Parser");
_property = record_property<__lexer_token_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_token, m_tokenType), "m_tokenType");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::Parser::__lexer_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__lexer_data");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Parser");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_aLines), "m_aLines");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_sCurrentLine), "m_sCurrentLine");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_uiCurrentLine), "m_uiCurrentLine");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_uiCurrentCharacterIndex), "m_uiCurrentCharacterIndex");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_sToken), "m_sToken");
_property = record_property<char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_cCurrentCharacter), "m_cCurrentCharacter");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_bWasSpace), "m_bWasSpace");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_bSpace), "m_bSpace");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_bNewLine), "m_bNewLine");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Parser::__lexer_data, m_bEnd), "m_bEnd");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::Parser::__lexer_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__lexer_ftable");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Parser");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
return _types;
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
