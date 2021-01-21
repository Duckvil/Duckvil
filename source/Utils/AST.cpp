#include "Utils/AST.h"

namespace Duckvil { namespace Utils {

    bool user_define_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult)
    {
        uint32_t _roundBrackets = 0;
        std::string _token;

        while(_pLexer->next_token(_pLexerData, &_token))
        {
            if(_token == "(")
            {
                _roundBrackets++;
            }
            else if(_token == ")")
            {
                _roundBrackets--;

                if(_roundBrackets == 0)
                {
                    break;
                }
            }
            else if((_pLexerData->m_bNewLine || _token == ";") && _roundBrackets == 0)
            {
                break;
            }
        }

        Duckvil::Parser::__ast_entity_define* _defineEntity = new Duckvil::Parser::__ast_entity_define();

        _defineEntity->m_sName = _sUserDefine;
        _defineEntity->m_pParentScope = _pAST->m_pCurrentScope;

        _pAST->m_pCurrentScope->m_aScopes.push_back(_defineEntity);

        return true;
    }

    bool user_define_resource_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult)
    {
        uint32_t _roundBrackets = 0;
        std::string _token;
        bool _end = false;

        _spResult->append(_sUserDefine);

        while(!_end && _pLexer->next_token(_pLexerData, &_token))
        {
            if(_token == "(")
            {
                _roundBrackets++;
            }
            else if(_token == ")")
            {
                _roundBrackets--;

                if(_roundBrackets == 0)
                {
                    _end = true;
                }
            }
            else if((_pLexerData->m_bNewLine || _token == ";") && _roundBrackets == 0)
            {
                break;
            }

            _spResult->append(_token);
        }

        return true;
    }

}}