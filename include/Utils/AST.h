#pragma once

#include "Parser/AST.h"

namespace Duckvil { namespace Utils {

    Parser::__behavior user_define_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult);
    Parser::__behavior user_define_resource_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult);

}}