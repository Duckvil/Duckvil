#pragma once

#include "Parser/AST.h"

namespace Duckvil { namespace Utils {

    bool user_define_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult);
    bool user_define_resource_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult);

}}