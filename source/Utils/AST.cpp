#include "Utils/AST.h"

#include "Utils/Utils.h"

namespace Duckvil { namespace Utils {

    Parser::__behavior user_define_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult)
    {
        uint32_t _roundBrackets = 0;
        std::string _token;
        std::string _curr;

        Duckvil::Parser::__ast_entity_define* _defineEntity = new Duckvil::Parser::__ast_entity_define();

        while(_pLexer->next_token(_pLexerData, &_token))
        {
            if(_token == "(")
            {
                _roundBrackets++;
            }
            else if (_token == ",")
            {
                if(_curr != "")
                {
                    _defineEntity->m_aArguments.push_back(_curr);
                }

                _curr.clear();
            }
            else if(_token == ")")
            {
                _roundBrackets--;

                if(_roundBrackets == 0)
                {
                    if(_curr != "")
                    {
                        _defineEntity->m_aArguments.push_back(_curr);
                    }

                    break;
                }
            }
            else if((_pLexerData->m_bNewLine || _token == ";") && _roundBrackets == 0)
            {
                break;
            }
            else
            {
                if(_pLexerData->m_bSpace)
                {
                    _curr += " ";
                }

                _curr += _token;
            }
        }

        _defineEntity->m_sName = _sUserDefine;
        _defineEntity->m_pParentScope = _pAST->m_pCurrentScope;

        _pAST->m_pCurrentScope->m_aScopes.push_back(_defineEntity);

        return Parser::__behavior::__behavior_Proceed;
    }

    Parser::__behavior user_define_resource_behavior(Parser::__ast* _pAST, Parser::__lexer_ftable* _pLexer, Parser::__lexer_data* _pLexerData, const std::string& _sUserDefine, std::string* _spResult)
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

        return Parser::__behavior::__behavior_Proceed;
    }

    bool ast_resolve_namespace(const std::string& _metaNamespace, std::string& _entityMetaNamespace, Parser::__ast_entity* _pScope, uint32_t _uiIndex)
    {
        if (_metaNamespace == _entityMetaNamespace)
        {
            return true;
        }

        auto _splittedNamespace = Utils::split(Utils::replace_all(_metaNamespace, "::", " "), ' ');
        auto _splittedMetaNamespace = Utils::split(Utils::replace_all(_entityMetaNamespace, "::", " "), ' ');

        if (_uiIndex > _splittedMetaNamespace.size())
        {
            auto _curr = _pScope->m_pParentScope;

            while (_curr->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_namespace && _curr->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
            {
                _curr = _curr->m_pParentScope;
            }

            if (_curr->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
            {
                auto _x = static_cast<Parser::__ast_entity_namespace*>(_curr);
                auto _name = _x->m_sName + "::";

                _entityMetaNamespace.insert(_entityMetaNamespace.begin(), _name.begin(), _name.end());

                return ast_resolve_namespace(_metaNamespace, _entityMetaNamespace, _curr, _uiIndex);
            }
            else if (_curr->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_main)
            {
                return false;
            }
        }

        std::string _nCurr = _splittedMetaNamespace[_splittedMetaNamespace.size() - _uiIndex];

        if (_nCurr != _splittedNamespace[_splittedNamespace.size() - _uiIndex])
        {
            auto _curr = _pScope;

            do
            {
                _curr = _curr->m_pParentScope;

                for (const auto& _x : _curr->m_aNamepaceAliases)
                {
                    auto _x2 = static_cast<Parser::__ast_entity_namespace_alias*>(_x);

                    if (_x2->m_sName == _nCurr)
                    {
                        _entityMetaNamespace.erase(_entityMetaNamespace.begin(), _entityMetaNamespace.begin() + _entityMetaNamespace.find_first_of("::"));

                        auto _t = _x2->m_sTarget;

                        _entityMetaNamespace.insert(_entityMetaNamespace.begin(), _t.begin(), _t.end());

                        return ast_resolve_namespace(_metaNamespace, _entityMetaNamespace, _curr, _uiIndex);
                    }
                }

                for (const auto& _x : _curr->m_aTypeAliases)
                {
                    auto _x2 = static_cast<Parser::__ast_entity_type_alias*>(_x);

                    if (_x2->m_sName == _nCurr)
                    {
                        _entityMetaNamespace.erase(_entityMetaNamespace.begin(), _entityMetaNamespace.begin() + _entityMetaNamespace.find_first_of("::"));

                        auto _t = _x2->m_sTarget;

                        _entityMetaNamespace.insert(_entityMetaNamespace.begin(), _t.begin(), _t.end());

                        return ast_resolve_namespace(_metaNamespace, _entityMetaNamespace, _curr, _uiIndex);
                    }
                }
            } while (_curr->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_namespace && _curr->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main);

            switch (_curr->m_scopeType)
            {
            case Parser::__ast_entity_type::__ast_entity_type_namespace:
            {
                auto _x = static_cast<Parser::__ast_entity_namespace*>(_curr);
                auto _name = _x->m_sName + "::";

                _entityMetaNamespace.insert(_entityMetaNamespace.begin(), _name.begin(), _name.end());

                return ast_resolve_namespace(_metaNamespace, _entityMetaNamespace, _curr, _uiIndex);
            } break;
            case Parser::__ast_entity_type::__ast_entity_type_main:
                return false;
            }
        }

        return ast_resolve_namespace(_metaNamespace, _entityMetaNamespace, _pScope, ++_uiIndex);
    }

}}