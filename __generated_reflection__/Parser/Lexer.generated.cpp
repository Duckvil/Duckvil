#include "Parser/Lexer.h"
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
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Parser");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("blank_file");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Parser;
_type = record_type<Duckvil::Parser::blank_file>(_data, "blank_file");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
record_inheritance<std::exception>(_data, _type, __protection::__protection_public);
_function = record_function<Duckvil::Parser::blank_file, char const*>(_data, _type, &Duckvil::Parser::blank_file::what, "what");
_namespaces.pop_back();
_enum = record_enum<Duckvil::Parser::__lexer_token_type>(_data, _ntype, "__lexer_token_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_symbol, "__lexer_token_type_symbol");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_number, "__lexer_token_type_number");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_text, "__lexer_token_type_text");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_string, "__lexer_token_type_string");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_floating, "__lexer_token_type_floating");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_integer, "__lexer_token_type_integer");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_negative_integer, "__lexer_token_type_negative_integer");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__lexer_token_type::__lexer_token_type_negative_floating, "__lexer_token_type_negative_floating");
_namespaces.push_back("__lexer_token");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__lexer_token>(_data, "__lexer_token");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<Duckvil::Parser::__lexer_token_type>(_data, _type, offsetof(Duckvil::Parser::__lexer_token, m_tokenType), "m_tokenType");
_namespaces.pop_back();
_namespaces.push_back("__lexer_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__lexer_data>(_data, "__lexer_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_aLines), "m_aLines");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_sCurrentLine), "m_sCurrentLine");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_uiCurrentLine), "m_uiCurrentLine");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_uiCurrentCharacterIndex), "m_uiCurrentCharacterIndex");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_sToken), "m_sToken");
_property = record_property<char>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_cCurrentCharacter), "m_cCurrentCharacter");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_bWasSpace), "m_bWasSpace");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_bSpace), "m_bSpace");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_bNewLine), "m_bNewLine");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_bEnd), "m_bEnd");
_property = record_property<IConfig*>(_data, _type, offsetof(Duckvil::Parser::__lexer_data, m_pConfig), "m_pConfig");
_namespaces.pop_back();
_namespaces.push_back("__lexer_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__lexer_ftable>(_data, "__lexer_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<void(*)(__lexer_data*,char const[256])>(_data, _type, offsetof(Duckvil::Parser::__lexer_ftable, load_file), "load_file");
_property = record_property<bool(*)(__lexer_data*,std::string*)>(_data, _type, offsetof(Duckvil::Parser::__lexer_ftable, next_token), "next_token");
_property = record_property<bool(*)(__lexer_data const&)>(_data, _type, offsetof(Duckvil::Parser::__lexer_ftable, space), "space");
_property = record_property<IConfig*(*)(__lexer_data*)>(_data, _type, offsetof(Duckvil::Parser::__lexer_ftable, init), "init");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Parser/Lexer.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
