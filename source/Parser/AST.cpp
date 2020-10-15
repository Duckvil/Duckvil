#include "Parser/AST.h"

#include <stack>
#include <queue>

#include "Utils/Utils.h"

#define DUCKVIL_SPACES \
    for(uint32_t i = 0; i < _uiScope; i++) \
    { \
        printf(" "); \
    }

namespace Duckvil { namespace Parser {

    __ast_entity* recursive(const __ast_entity* _entity, uint32_t& _uiScope)
    {
        DUCKVIL_SPACES

        switch(_entity->m_scopeType)
        {
        case __ast_entity_type::__ast_entity_type_structure:
            printf("Structure %s\n", ((__ast_entity_structure*)_entity)->m_sName.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_namespace:
            printf("Namespace %s\n", ((__ast_entity_namespace*)_entity)->m_sName.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_enum:
            printf("Enum %s\n", ((__ast_entity_enum*)_entity)->m_sName.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_variable:
            printf("Variable\n");
            DUCKVIL_SPACES
            printf(" Name: %s\n", ((__ast_entity_variable*)_entity)->m_sName.c_str());
            DUCKVIL_SPACES
            printf(" Type: %s\n", ((__ast_entity_variable*)_entity)->m_sType.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_constructor:
            printf("Constructor\n");
            break;
        case __ast_entity_type::__ast_entity_type_callback:
            printf("Callback\n");
            break;
        case __ast_entity_type::__ast_entity_type_function:
            printf("Function\n");
            break;
        }

        for(__ast_meta _meta : _entity->m_aMeta)
        {
            DUCKVIL_SPACES
            printf("  Meta: %s %s\n", _meta.m_sKey.c_str(), _meta.m_sValue.c_str());
        }

        _uiScope++;

        for(__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_ent, _uiScope);
        }

        _uiScope--;

        return 0;
    }

    std::vector<__ast_entity_argument> process_arguments(__lexer_ftable* _pLexer, __lexer_data& _lexerData, const std::string& _sArgs, bool& _error)
    {
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
        bool _continue = false;
        bool _wasType = false;

        while(_continue || _pLexer->next_token(&_exp, &_token))
        {
            _continue = false;

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
            else if(_token == "*" && _roundBrackets == 0)
            {
                _error = true;

                break;
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

                _wasType = false;

                while(_pLexer->next_token(&_exp, &_token))
                {
                    if(_token != "")
                    {
                        _continue = true;

                        break;
                    }
                }
            }
            else if(_token == "=")
            {
                while(_pLexer->next_token(&_exp, &_token))
                {
                    if(_token == "," || _token == ")")
                    {
                        _continue = true;

                        break;
                    }
                }
            }
            else if(_exp.m_bSpace)
            {
                if(_triBrackets == 0 && !_wasType)
                {
                    if(!_keyword)
                    {
                        _type = _tmp;
                        _wasType = true;

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

    bool check_function(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData, std::string& _token, bool& _error, std::string _expression)
    {
        bool _res = false;

        if(_expression.size() <= 0)
        {
            return _res;
        }

        while(_expression[_expression.size() - 1] == ' ')
        {
            _expression.erase(_expression.begin() + _expression.size() - 1);
        }

        __lexer_data _exp = {};

        _exp.m_bWasSpace = false;
        _exp.m_bSpace = false;
        _exp.m_bEnd = false;
        _exp.m_bNewLine = false;
        _exp.m_uiCurrentCharacterIndex = 0;
        _exp.m_uiCurrentLine = 0;

        _exp.m_aLines.push_back(_expression);
        _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

        bool _space = false;
        uint32_t _triBrackets = 0;
        uint32_t _roundBrackets = 0;
        std::string _tmp;
        bool _wasType = false;
        std::string _type;
        std::string _name;
        bool _callback = false;
        bool _wasOpenRoundBracket = false;
        std::size_t _index = std::string::npos;
        bool _wasKeyword = false;

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
            else if(_token == "(")
            {
                _roundBrackets++;

                // If it is not part of template
                if(_triBrackets == 0)
                {
                    _wasOpenRoundBracket = true;

                    _name = _tmp;

                    _index = _exp.m_uiCurrentCharacterIndex - 1;
                }
            }
            else if(_token == ")")
            {
                _roundBrackets--;
            }
            else if(_roundBrackets == 1 && _token == "*")
            {
                _callback = true;
            }
            else if(_token == "inline")
            {

            }
            else if(_token == "const" && _roundBrackets == 0)
            {
                _tmp += _token + " ";

                _wasKeyword = true;
            }
            else if(_exp.m_bSpace && _triBrackets == 0 && !_wasKeyword)
            {
                if(!_wasType)
                {
                    _type = _tmp;
                }

                _wasType = true;

                _tmp.clear();
            }
            else if(_wasKeyword)
            {
                _wasKeyword = false;
            }
            else
            {
                _tmp += _token;
            }
        }

        if(!_callback && !_wasOpenRoundBracket)
        {
            __ast_entity_variable* _scope = nullptr;

            if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_variable)
            {
                _scope = (__ast_entity_variable*)_pAST->m_pPendingScope;
            }
            else
            {
                _scope = new __ast_entity_variable();
            }

            _scope->m_sName = _tmp;
            _scope->m_sType = _type;
            _scope->m_accessLevel = _pAST->m_currentAccess;

            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

            _scope->m_pParentScope = _pAST->m_pCurrentScope;

            _pAST->m_pPendingScope = nullptr;
        }
        else if(_wasOpenRoundBracket && !_callback)
        {
            __ast_entity_function* _scope = nullptr;

            if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
            {
                _scope = (__ast_entity_function*)_pAST->m_pPendingScope;
            }
            else
            {
                _scope = new __ast_entity_function();
            }

            _scope->m_accessLevel = _pAST->m_currentAccess;
            _scope->m_sReturnType = _type;
            _scope->m_sName = _name;

            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

            _scope->m_pParentScope = _pAST->m_pCurrentScope;

            _tmp.clear();

            std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_index), _error);

            _scope->m_aArguments.insert(_scope->m_aArguments.begin(), _args.begin(), _args.end());

            _pAST->m_pPendingScope = nullptr;

            _res = true;
        }
        else
        {
            __ast_entity_callback* _entity = nullptr;

            if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_callback)
            {
                _entity = (__ast_entity_callback*)_pAST->m_pPendingScope;
            }
            else
            {
                _entity = new __ast_entity_callback();
            }

            __ast_entity_callback* _castedEntity = (__ast_entity_callback*)_entity;

            _castedEntity->m_sReturnType = _type;
            _castedEntity->m_accessLevel = _pAST->m_currentAccess;
            _castedEntity->m_sName = _name;

            _pLexer->next_token(&_exp, &_token); // )

            std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex), _error);

            _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

            _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

            _entity->m_pParentScope = _pAST->m_pCurrentScope;

            _pAST->m_pPendingScope = nullptr;
        }

        // if(_expression[_expression.size() - 1] == ')')
        // {
        //     // Function or callback

        //     __lexer_data _exp = {};

        //     _exp.m_bWasSpace = false;
        //     _exp.m_bSpace = false;
        //     _exp.m_bEnd = false;
        //     _exp.m_bNewLine = false;
        //     _exp.m_uiCurrentCharacterIndex = 0;
        //     _exp.m_uiCurrentLine = 0;

        //     _exp.m_aLines.push_back(_expression);
        //     _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

        //     bool _space = false;
        //     uint32_t _triBrackets = 0;
        //     std::string _tmp;
        //     bool _wasType = false;
        //     __ast_entity* _entity = 0;

        //     while(_pLexer->next_token(&_exp, &_token))
        //     {
        //         if(_token == "<")
        //         {
        //             _triBrackets++;
        //         }
        //         else if(_token == ">")
        //         {
        //             _triBrackets--;
        //         }
        //         else if(_exp.m_bSpace && _triBrackets == 0)
        //         {
        //             _wasType = true;
        //         }
        //         else if(_token == "inline")
        //         {

        //         }
        //         else if(_wasType && _token == "(")
        //         {
        //             // callback

        //             if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_callback)
        //             {
        //                 _entity = (__ast_entity_callback*)_pAST->m_pPendingScope;
        //             }
        //             else
        //             {
        //                 _entity = new __ast_entity_callback();
        //             }

        //             __ast_entity_callback* _castedEntity = (__ast_entity_callback*)_entity;

        //             _castedEntity->m_sReturnType = _tmp;
        //             _castedEntity->m_accessLevel = _pAST->m_currentAccess;

        //             _pLexer->next_token(&_exp, &_token); // *
        //             _pLexer->next_token(&_exp, &_token); // name

        //             _castedEntity->m_sName = _token;

        //             _pLexer->next_token(&_exp, &_token); // )

        //             std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex), _error);

        //             _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

        //             _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

        //             _entity->m_pParentScope = _pAST->m_pCurrentScope;

        //             _pAST->m_pPendingScope = nullptr;

        //             break;
        //         }
        //         else
        //         {
        //             if(_wasType)
        //             {
        //                 // function

        //                 if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        //                 {
        //                     _entity = (__ast_entity_function*)_pAST->m_pPendingScope;
        //                 }
        //                 else
        //                 {
        //                     _entity = new __ast_entity_function();
        //                 }

        //                 __ast_entity_function* _castedEntity = (__ast_entity_function*)_entity;

        //                 _castedEntity->m_accessLevel = _pAST->m_currentAccess;
        //                 _castedEntity->m_sReturnType = _tmp;

        //                 _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

        //                 _entity->m_pParentScope = _pAST->m_pCurrentScope;

        //                 _tmp.clear();

        //                 std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex), _error);

        //                 _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

        //                 _pAST->m_pPendingScope = nullptr;

        //                 _res = true;

        //                 break;
        //             }

        //             _tmp += _token;
        //         }
        //     }
        // }
        // else
        // {
        //     // Variable

        //     std::size_t _position = _expression.find_last_of(' ');
        //     __ast_entity_variable* _scope = nullptr;

        //     if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_variable)
        //     {
        //         _scope = (__ast_entity_variable*)_pAST->m_pPendingScope;
        //     }
        //     else
        //     {
        //         _scope = new __ast_entity_variable();
        //     }

        //     _scope->m_sName = _expression.substr(_position + 1);
        //     _scope->m_sType = _expression.substr(0, _position);
        //     _scope->m_accessLevel = _pAST->m_currentAccess;

        //     _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

        //     _scope->m_pParentScope = _pAST->m_pCurrentScope;

        //     _pAST->m_pPendingScope = nullptr;
        // }

        return _res;
    }

    std::vector<__ast_meta> parse_meta(__lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        std::vector<__ast_meta> _result;
        std::string _token;
        uint32_t _roundBrackets = 0;
        std::string _tmp_expression;
        std::string _key;

        while(_pLexer->next_token(&_lexerData, &_token))
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
                    __ast_meta _meta = {};

                    _meta.m_sKey = _key;
                    _meta.m_sValue = _tmp_expression;

                    _result.push_back(_meta);

                    break;
                }
            }
            else if(_token == ",")
            {
                __ast_meta _meta = {};

                _meta.m_sKey = _key;
                _meta.m_sValue = _tmp_expression;

                _result.push_back(_meta);

                _tmp_expression.clear();
            }
            else if(_token == "=")
            {
                _key = _tmp_expression;

                _tmp_expression.clear();
            }
            else
            {
                _tmp_expression += _token;
            }
        }

        return _result;
    }

    void ast_generate(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        _pAST->m_pCurrentScope = &_pAST->m_main;
        _pAST->m_pPendingScope = _pAST->m_pCurrentScope;

        std::string _token;
        std::string _tmp_expression;

        bool _oneSlash = false;
        bool _continue = false;
        bool _error = false;

        while(_continue || _pLexer->next_token(&_lexerData, &_token))
        {
            _continue = false;

            if(_token == "DUCKVIL_STRUCT")
            {
                __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_struct);

                _pAST->m_pPendingScope = _scope;

                _scope->m_aMeta = parse_meta(_pLexer, _lexerData);
            }
            else if(_token == "DUCKVIL_CLASS")
            {
                __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);

                _pAST->m_pPendingScope = _scope;

                _scope->m_aMeta = parse_meta(_pLexer, _lexerData);
            }
            else if(_token == "DUCKVIL_NAMESPACE")
            {
                __ast_entity_namespace* _scope = new __ast_entity_namespace();

                _pAST->m_pPendingScope = _scope;

                _scope->m_aMeta = parse_meta(_pLexer, _lexerData);
            }
            else if(_token == "DUCKVIL_VARIABLE")
            {
                __ast_entity_variable* _scope = new __ast_entity_variable();

                _pAST->m_pPendingScope = _scope;

                _scope->m_aMeta = parse_meta(_pLexer, _lexerData);
            }
            else if(_token == "DUCKVIL_FUNCTION")
            {
                __ast_entity_function* _scope = new __ast_entity_function();

                _pAST->m_pPendingScope = _scope;

                _scope->m_aMeta = parse_meta(_pLexer, _lexerData);
            }
            else if(_token == "DUCKVIL_CALLBACK")
            {
                __ast_entity_callback* _scope = new __ast_entity_callback();

                _pAST->m_pPendingScope = _scope;

                _scope->m_aMeta = parse_meta(_pLexer, _lexerData);
            }
            else if(_token == "namespace")
            {
                if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_namespace)
                {
                    _pAST->m_pPendingScope->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_pAST->m_pPendingScope);
                }
                else
                {
                    __ast_entity_namespace* _scope = new __ast_entity_namespace();

                    _scope->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                    _pAST->m_pPendingScope = _scope;
                }
            }
            else if(_token == "class")
            {
                if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    _pAST->m_pPendingScope->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_pAST->m_pPendingScope);
                }
                else
                {
                    __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);

                    _scope->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                    _pAST->m_pPendingScope = _scope;
                }
            }
            else if(_token == "struct")
            {
                if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    _pAST->m_pPendingScope->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_pAST->m_pPendingScope);
                }
                else
                {
                    __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_struct);

                    _scope->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                    _pAST->m_pPendingScope = _scope;
                }
            }
            else if(_token == "enum")
            {
                __ast_entity_enum* _scope = new __ast_entity_enum();

                _scope->m_pParentScope = _pAST->m_pCurrentScope;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                _pAST->m_pPendingScope = _scope;
            }
            else if(_token == "public")
            {
                _pAST->m_currentAccess = __ast_access::__ast_access_public;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ":")
                    {
                        break;
                    }
                }
            }
            else if(_token == "protected")
            {
                _pAST->m_currentAccess = __ast_access::__ast_access_protected;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ":")
                    {
                        break;
                    }
                }
            }
            else if(_token == "private")
            {
                _pAST->m_currentAccess = __ast_access::__ast_access_private;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ":")
                    {
                        break;
                    }
                }
            }
            else if(_token == "template")
            {
                uint32_t _triBrackets = 0;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == "<")
                    {
                        _triBrackets++;
                    }
                    else if(_token == ">")
                    {
                        _triBrackets--;

                        if(_triBrackets == 0)
                        {
                            break;
                        }
                    }
                }
            }
            else if(_token == "typedef")
            {
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ";")
                    {
                        break;
                    }
                }
            }
            else if(_token == "inline")
            {

            }
            else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure && ((__ast_entity_structure*)_pAST->m_pCurrentScope)->m_sName == _token && !_error)
            {
                std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _lexerData.m_sCurrentLine.substr(_lexerData.m_uiCurrentCharacterIndex), _error);

                if(_error)
                {
                    _continue = true;

                    continue;
                }

                __ast_entity_constructor* _scope = new __ast_entity_constructor();

                _scope->m_pParentScope = _pAST->m_pCurrentScope;
                _scope->m_accessLevel = _pAST->m_currentAccess;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                _scope->m_aArguments.insert(_scope->m_aArguments.begin(), _args.begin(), _args.end());

                // _pAST->m_pPendingScope = _scope;

                uint32_t _roundBrackets = 0;
                uint32_t _mustacheBrackets = 0;

                while(_pLexer->next_token(&_lexerData, &_token))
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
                }

            // Skip ';' or if '{' - whole constructor body
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == "{")
                    {
                        _mustacheBrackets++;
                    }
                    else if(_token == "}")
                    {
                        _mustacheBrackets--;

                        if(_mustacheBrackets == 0)
                        {
                            break;
                        }
                    }
                    else if(_token == ";" && _mustacheBrackets == 0)
                    {
                        break;
                    }
                }
            }
            else if(_token == "~")
            {
                uint32_t _mustacheBrackets = 0;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == "{")
                    {
                        _mustacheBrackets++;
                    }
                    else if(_token == ";")
                    {
                        break;
                    }
                    else if(_token == "}")
                    {
                        _mustacheBrackets--;

                        if(_mustacheBrackets == 0)
                        {
                            break;
                        }
                    }
                }
            }
            else if(_token == "{")
            {
                if(_pAST->m_pPendingScope == nullptr)
                {
                    check_function(_pAST, _pLexer, _lexerData, _token, _error, _tmp_expression);

                    uint32_t _mustacheBrackets = 1;

                    while(_pLexer->next_token(&_lexerData, &_token))
                    {
                        if(_token == "{")
                        {
                            _mustacheBrackets++;
                        }
                        else if(_token == "}")
                        {
                            _mustacheBrackets--;

                            if(_mustacheBrackets == 0)
                            {
                                break;
                            }
                        }
                    }

                    _tmp_expression.clear();
                }
                else
                {
                    _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
                    _pAST->m_pPendingScope = nullptr;
                }
            }
            else if(_token == "}")
            {
                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    _pLexer->next_token(&_lexerData, &_token); // Skip ;
                }
                else if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                {
                    _pLexer->next_token(&_lexerData, &_token); // Skip ;
                }

                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure ||
                    _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum ||
                    _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_namespace)
                {
                    _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
                    _pAST->m_pPendingScope = nullptr;
                }

                _tmp_expression.clear();
            }
            else if(_token == "#")
            {
                bool _skipNewLine = false;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_lexerData.m_bNewLine)
                    {
                        if(_skipNewLine)
                        {
                            _skipNewLine = false;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if(_token == "\\")
                    {
                        _skipNewLine = true;
                    }
                }
            }
            else if(_token == "/")
            {
                if(_oneSlash)
                {
                    _oneSlash = false;

                    while(_pLexer->next_token(&_lexerData, &_token))
                    {
                        if(_lexerData.m_bNewLine)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    _oneSlash = true;
                }
            }
            else if(_token == ",")
            {

            }
            else if(_token == ":")
            {
                if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    __ast_inheritance _inh = {};
                    std::string _name;
                    __ast_entity_structure* _cased = (__ast_entity_structure*)_pAST->m_pPendingScope;

                    while(_pLexer->next_token(&_lexerData, &_token))
                    {
                        if(_token == "public")
                        {
                            _inh.m_protection = __ast_access::__ast_access_public;
                        }
                        else if(_token == "protected")
                        {
                            _inh.m_protection = __ast_access::__ast_access_public;
                        }
                        else if(_token == "private")
                        {
                            _inh.m_protection = __ast_access::__ast_access_public;
                        }
                        else if(_token == ",")
                        {
                            _inh.m_sName = _name;

                            _cased->m_aInheritance.push_back(_inh);
                        }
                        else if(_token == "{")
                        {
                            _inh.m_sName = _name;

                            _cased->m_aInheritance.push_back(_inh);

                            _continue = true;

                            break;
                        }
                        else
                        {
                            _name += _token;
                        }
                    }
                }
                else
                {
                    _tmp_expression += _token;
                }
            }
            else if(_token == "=")
            {
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ";" || _token == ",")
                    {
                        break;
                    }
                }
            }
            else if(_token == ";")
            {
                // if(_tmp_expression.size() > 0 && _tmp_expression[_tmp_expression.size() - 1] == ')')
                // {
                //     // Function or callback

                //     __lexer_data _exp = {};

                //     _exp.m_bWasSpace = false;
                //     _exp.m_bSpace = false;
                //     _exp.m_bEnd = false;
                //     _exp.m_bNewLine = false;
                //     _exp.m_uiCurrentCharacterIndex = 0;
                //     _exp.m_uiCurrentLine = 0;

                //     _exp.m_aLines.push_back(_tmp_expression);
                //     _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

                //     bool _space = false;
                //     uint32_t _triBrackets = 0;
                //     std::string _tmp;
                //     bool _wasType = false;
                //     __ast_entity* _entity = 0;

                //     while(_pLexer->next_token(&_exp, &_token))
                //     {
                //         if(_token == "<")
                //         {
                //             _triBrackets++;
                //         }
                //         else if(_token == ">")
                //         {
                //             _triBrackets--;
                //         }
                //         else if(_exp.m_bSpace && _triBrackets == 0)
                //         {
                //             _wasType = true;
                //         }
                //         else if(_token == "inline")
                //         {

                //         }
                //         else if(_wasType && _token == "(")
                //         {
                //             // callback

                //             if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_callback)
                //             {
                //                 _entity = (__ast_entity_callback*)_pAST->m_pPendingScope;
                //             }
                //             else
                //             {
                //                 _entity = new __ast_entity_callback();
                //             }

                //             __ast_entity_callback* _castedEntity = (__ast_entity_callback*)_entity;

                //             _castedEntity->m_sReturnType = _tmp;
                //             _castedEntity->m_accessLevel = _pAST->m_currentAccess;

                //             _pLexer->next_token(&_exp, &_token); // *
                //             _pLexer->next_token(&_exp, &_token); // name

                //             _castedEntity->m_sName = _token;

                //             _pLexer->next_token(&_exp, &_token); // )

                //             std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex), _error);

                //             _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

                //             _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                //             _entity->m_pParentScope = _pAST->m_pCurrentScope;

                //             _pAST->m_pPendingScope = nullptr;

                //             break;
                //         }
                //         else
                //         {
                //             if(_wasType)
                //             {
                //                 // function

                //                 if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
                //                 {
                //                     _entity = (__ast_entity_function*)_pAST->m_pPendingScope;
                //                 }
                //                 else
                //                 {
                //                     _entity = new __ast_entity_function();
                //                 }

                //                 __ast_entity_function* _castedEntity = (__ast_entity_function*)_entity;

                //                 _castedEntity->m_accessLevel = _pAST->m_currentAccess;
                //                 _castedEntity->m_sReturnType = _tmp;

                //                 _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                //                 _entity->m_pParentScope = _pAST->m_pCurrentScope;

                //                 _tmp.clear();

                //                 std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex), _error);

                //                 _castedEntity->m_aArguments.insert(_castedEntity->m_aArguments.begin(), _args.begin(), _args.end());

                //                 _pAST->m_pPendingScope = nullptr;

                //                 break;
                //             }

                //             _tmp += _token;
                //         }
                //     }
                // }
                // else
                // {
                //     // Variable

                //     std::size_t _position = _tmp_expression.find_last_of(' ');
                //     __ast_entity_variable* _scope = nullptr;

                //     if(_pAST->m_pPendingScope && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_variable)
                //     {
                //         _scope = (__ast_entity_variable*)_pAST->m_pPendingScope;
                //     }
                //     else
                //     {
                //         _scope = new __ast_entity_variable();
                //     }

                //     _scope->m_sName = _tmp_expression.substr(_position + 1);
                //     _scope->m_sType = _tmp_expression.substr(0, _position);
                //     _scope->m_accessLevel = _pAST->m_currentAccess;

                //     _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                //     _scope->m_pParentScope = _pAST->m_pCurrentScope;

                //     _pAST->m_pPendingScope = nullptr;
                // }

                check_function(_pAST, _pLexer, _lexerData, _token, _error, _tmp_expression);

                _tmp_expression.clear();
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

                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                {
                     __ast_entity_variable* _enumVariable = new __ast_entity_variable();

                    _enumVariable->m_sName = _token;
                    _enumVariable->m_sType = ((__ast_entity_enum*)_pAST->m_pCurrentScope)->m_sName;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_enumVariable);

                    _enumVariable->m_pParentScope = _pAST->m_pCurrentScope;

                    _tmp_expression.clear();
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
                    else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                    {
                        ((__ast_entity_enum*)_pAST->m_pPendingScope)->m_sName = _token;
                    }
                    else
                    {
                        _tmp_expression += _token;
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

            _error = false;
        }
    }

    void ast_print(const __ast& _AST)
    {
        uint32_t _scope = 0;

        recursive(&(_AST.m_main), _scope);
    }

}}

Duckvil::Parser::__ast_ftable* duckvil_ast_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Parser::__ast_ftable* _table = (Duckvil::Parser::__ast_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Parser::__ast_ftable), alignof(Duckvil::Parser::__ast_ftable));

    _table->ast_generate = &Duckvil::Parser::ast_generate;
    _table->ast_print = &Duckvil::Parser::ast_print;

    return _table;
}