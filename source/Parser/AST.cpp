#include "Parser/AST.h"

#include <stack>
#include <queue>

#include "Utils/Utils.h"

namespace Duckvil { namespace Parser {

    std::vector<__ast_entity_argument> process_arguments(__lexer_ftable* _pLexer, __lexer_data& _lexerData, const std::string& _sArgs)
    {
        // std::string _args = _sArgs.substr(1, _sArgs.size() - 2); // remove ()
        // std::vector<std::string> _splitted = Utils::split(_args, ',');
        std::vector<__ast_entity_argument> _res;

        __lexer_data _exp = {};

        _exp.m_bWasSpace = false;
        _exp.m_bSpace = false;
        _exp.m_bEnd = false;
        _exp.m_bNewLine = false;
        _exp.m_uiCurrentCharacterIndex = 0;
        _exp.m_uiCurrentLine = 0;

        std::string _token;

        _exp.m_aLines.push_back(_sArgs);
        _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

        uint32_t _roundBrackets = 0;
        uint32_t _triBrackets = 0;
        std::string _tmp;
        std::string _type;
        bool _keyword = false;

        while(_pLexer->next_token(&_exp, &_token))
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
                    if(_type.size() <= 0 && _tmp.size() <= 0)
                    {
                        continue;
                    }

                    __ast_entity_argument _arg = {};

                    _arg.m_sType = _type;
                    _arg.m_sName = _tmp;

                    _res.push_back(_arg);

                    _tmp.clear();
                }
            }
            else if(_token == "<")
            {
                _triBrackets++;
            }
            else if(_token == ">")
            {
                _triBrackets--;
            }
            else if(_token == "const")
            {
                _keyword = true;

                _tmp += _token;
            }
            else if(_token == ",")
            {
                if(_type.size() <= 0 && _tmp.size() <= 0)
                {
                    continue;
                }

                __ast_entity_argument _arg = {};

                _arg.m_sType = _type;
                _arg.m_sName = _tmp;

                _res.push_back(_arg);

                _tmp.clear();
            }
            else if(_exp.m_bSpace)
            {
                if(_triBrackets == 0)
                {
                    if(!_keyword)
                    {
                        _type = _tmp;

                        _tmp.clear();
                    }
                    else
                    {
                        _keyword = false;

                        _tmp += " ";
                    }
                }
            }
            else
            {
                _tmp += _token;
            }
        }

        return _res;
    }

    void generate_ast(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        _pAST->m_pCurrentScope = &_pAST->m_main;
        _pAST->m_pPendingScope = _pAST->m_pCurrentScope;

        std::string _token;
        std::string _tmp_expression;

        while(_pLexer->next_token(&_lexerData, &_token))
        {
            if(_token == "namespace")
            {
                __ast_entity_namespace* _scope = new __ast_entity_namespace();

                _scope->m_pParentScope = _pAST->m_pCurrentScope;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                _pAST->m_pPendingScope = _scope;
            }
            else if(_token == "class")
            {
                __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);

                _scope->m_pParentScope = _pAST->m_pCurrentScope;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                _pAST->m_pPendingScope = _scope;
            }
            else if(_token == "struct")
            {
                __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_struct);

                _scope->m_pParentScope = _pAST->m_pCurrentScope;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                _pAST->m_pPendingScope = _scope;
            }
            else if(_token == "public")
            {
                _pAST->m_currentAccess = __ast_access::__ast_access_public;
            }
            else if(_token == "protected")
            {
                _pAST->m_currentAccess = __ast_access::__ast_access_protected;
            }
            else if(_token == "private")
            {
                _pAST->m_currentAccess = __ast_access::__ast_access_private;
            }
            else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure && ((__ast_entity_structure*)_pAST->m_pCurrentScope)->m_sName == _token)
            {
                __ast_entity_constructor* _scope = new __ast_entity_constructor();

                _scope->m_pParentScope = _pAST->m_pCurrentScope;
                _scope->m_accessLevel = _pAST->m_currentAccess;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _lexerData.m_sCurrentLine.substr(_lexerData.m_uiCurrentCharacterIndex));

            // TODO: Fix round brackets while parsing arguments

                _scope->m_aArguments.insert(_scope->m_aArguments.begin(), _args.begin(), _args.end());

                _pAST->m_pPendingScope = _scope;
            }
            else if(_token == "{")
            {
                _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
                _pAST->m_pPendingScope = nullptr;

                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_constructor)
                {
                    __ast_entity_constructor* _castedEntity = (__ast_entity_constructor*)_pAST->m_pCurrentScope->m_scopeType;
                }
            }
            else if(_token == "}")
            {
                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    _pLexer->next_token(&_lexerData, &_token); // Skip ;
                }

                _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
                _pAST->m_pPendingScope = nullptr;
            }
            else if(_token == "#")
            {
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_lexerData.m_bNewLine)
                    {
                        break;
                    }
                }
            }
            else if(_token == ";")
            {
                if(_tmp_expression.size() > 0 && _tmp_expression[_tmp_expression.size() - 1] == ')')
                {
                    // Function or callback

                    __lexer_data _exp = {};

                    _exp.m_bWasSpace = false;
                    _exp.m_bSpace = false;
                    _exp.m_bEnd = false;
                    _exp.m_bNewLine = false;
                    _exp.m_uiCurrentCharacterIndex = 0;
                    _exp.m_uiCurrentLine = 0;

                    _exp.m_aLines.push_back(_tmp_expression);
                    _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

                    bool _space = false;
                    uint32_t _triBrackets = 0;
                    std::string _tmp;
                    bool _wasType = false;
                    __ast_entity* _entity = 0;

                    while(_pLexer->next_token(&_exp, &_token))
                    {
                        if(_token == "<")
                        {
                            _triBrackets++;
                        }
                        else if(_token == ">")
                        {
                            _triBrackets--;
                        }
                        else if(_exp.m_bSpace && _triBrackets == 0)
                        {
                            _wasType = true;
                        }
                        else if(_wasType && _token == "(")
                        {
                            // callback

                            _entity = new __ast_entity_callback();

                            __ast_entity_callback* _castedEntity = (__ast_entity_callback*)_entity;

                            _castedEntity->m_sReturnType = _tmp;
                            _castedEntity->m_accessLevel = _pAST->m_currentAccess;

                            _pLexer->next_token(&_exp, &_token); // *
                            _pLexer->next_token(&_exp, &_token); // name

                            _castedEntity->m_sName = _token;

                            _pLexer->next_token(&_exp, &_token); // )

                            std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex));

                            _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

                            _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                            _entity->m_pParentScope = _pAST->m_pCurrentScope;

                            break;
                        }
                        else if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_constructor)
                        {
                            printf("AAAAA\n");
                        }
                        else
                        {
                            if(_wasType)
                            {
                                // function

                                _entity = new __ast_entity_function();

                                __ast_entity_function* _castedEntity = (__ast_entity_function*)_entity;

                                _castedEntity->m_accessLevel = _pAST->m_currentAccess;
                                _castedEntity->m_sReturnType = _tmp;

                                _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                                _entity->m_pParentScope = _pAST->m_pCurrentScope;

                                _tmp.clear();

                                std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex));

                                _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

                                break;
                            }

                            _tmp += _token;
                        }
                    }
                }
                else
                {
                    // Variable

                    std::size_t _position = _tmp_expression.find_last_of(' ');

                    __ast_entity_variable* _scope = new __ast_entity_variable();

                    _scope->m_sName = _tmp_expression.substr(_position + 1);
                    _scope->m_sType = _tmp_expression.substr(0, _position);
                    _scope->m_accessLevel = _pAST->m_currentAccess;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                    _scope->m_pParentScope = _pAST->m_pCurrentScope;
                }

                _tmp_expression.clear();
            }
            else if(_token == "\n")
            {
                continue;
            }
            else if(_token != "")
            {
                bool _defineFound = false;

                for(const std::string& _userDefine : _pAST->m_aUserDefines)
                {
                    if(_token == _userDefine)
                    {
                        _defineFound = true;

                        break;
                    }
                }

                if(_defineFound)
                {
                    continue;
                }

                if(_pAST->m_pPendingScope != nullptr)
                {
                    if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_namespace)
                    {
                        ((__ast_entity_namespace*)_pAST->m_pPendingScope)->m_sName = _token;
                    }
                    else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                    {
                        ((__ast_entity_structure*)_pAST->m_pPendingScope)->m_sName = _token;
                    }
                }
                else
                {
                    _tmp_expression += _token;
                }
            }
            else if(_lexerData.m_bSpace && _tmp_expression.size() > 0)
            {
                _tmp_expression += ' ';
            }
        }
    }

}}

Duckvil::Parser::__ast_ftable* duckvil_ast_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Parser::__ast_ftable* _table = (Duckvil::Parser::__ast_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Parser::__ast_ftable), alignof(Duckvil::Parser::__ast_ftable));

    _table->generate_ast = &Duckvil::Parser::generate_ast;

    return _table;
}