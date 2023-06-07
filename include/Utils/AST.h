#pragma once

#include "Parser/AST.h"

namespace Duckvil { namespace Utils {

    Parser::__behavior user_define_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult);
    Parser::__behavior user_define_resource_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult);
    bool ast_resolve_namespace(const std::string& _metaNamespace, std::string& _entityMetaNamespace, Parser::__ast_entity* _pScope, uint32_t _uiIndex = 1);

}}