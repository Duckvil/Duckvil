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
            for(const std::string& _element : ((__ast_entity_enum*)_entity)->m_aElements)
            {
                DUCKVIL_SPACES
                printf(" Element %s\n", _element.c_str());
            }
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

    std::vector<__ast_entity_argument> process_arguments(__lexer_ftable* _pLexer, __lexer_data& _lexerData, const std::string& _sArgs)
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
        std::string _tmpExpression;

        bool _oneSlash = false;
        bool _continue = false;

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
                std::string _internal;
                std::size_t _openBracketIndex = std::string::npos;
                uint32_t _roundBrackets = 0;
                bool _wasType = false;
                std::string _type;
                bool _wasOpenBracket = false;
                std::string _name;
                bool _skip = false;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ";")
                    {
                        break;
                    }
                    else if(_token == "(")
                    {
                        _roundBrackets++;
                        _openBracketIndex = _lexerData.m_uiCurrentCharacterIndex - 1;
                        _wasOpenBracket = true;

                        if(_skip)
                        {
                            continue;
                        }

                        while(_pLexer->next_token(&_lexerData, &_token))
                        {
                            if(_token == "*")
                            {

                            }
                            else if(_token == ")")
                            {
                                _name = _internal;
                                _skip = true;

                                break;
                            }
                            else
                            {
                                _internal += _token;
                            }
                        }
                    }
                    else if(_token == ")")
                    {
                        _roundBrackets--;
                    }
                    else if(_token == "" && _lexerData.m_bSpace && _internal.size() > 0 && !_wasOpenBracket)
                    {
                        _type = _internal;
                        _wasType = true;

                        _internal.clear();
                    }
                    else
                    {
                        if(!_skip)
                        {
                            _internal += _token;
                        }
                    }
                }

                if(_openBracketIndex == std::string::npos)
                {
                    __ast_entity_typedef* _entity = new __ast_entity_typedef();

                    _entity->m_sType = _type;
                    _entity->m_sName = _internal;

                    _entity->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);
                }
                else
                {
                    __ast_entity_callback_typedef* _entity = new __ast_entity_callback_typedef();

                    _entity->m_sName = _internal;
                    _entity->m_sType = _type;

                    std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _lexerData.m_sCurrentLine.substr(_openBracketIndex));

                    _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());

                    _entity->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);
                }
            }
            else if(_token == "using")
            {
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ";")
                    {
                        break;
                    }
                }
            }
            else if(_token == "{")
            {
                if(_pAST->m_pPendingScope == nullptr || (_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function))
                {
                    uint32_t _mustacheBrackets = 1;

                    __lexer_data _exp = {};

                    _exp.m_bWasSpace = false;
                    _exp.m_bSpace = false;
                    _exp.m_bEnd = false;
                    _exp.m_bNewLine = false;
                    _exp.m_uiCurrentCharacterIndex = 0;
                    _exp.m_uiCurrentLine = 0;

                    _exp.m_aLines.push_back(_tmpExpression);
                    _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

                    bool _wasType = false;
                    uint32_t _triBrackets = 0;
                    std::string _type;
                    std::string _name;
                    std::string _internalTmp;
                    std::size_t _roundBracketIndex = std::string::npos;

                    while(_pLexer->next_token(&_exp, &_token))
                    {
                        if(_token == "")
                        {
                            if(_exp.m_bSpace && _triBrackets == 0)
                            {
                                _type = _internalTmp;
                                _wasType = true;

                                _internalTmp.clear();
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
                        else if(_token == "(" && _triBrackets == 0)
                        {
                            _roundBracketIndex = _exp.m_uiCurrentCharacterIndex - 1;
                            _name = _internalTmp;

                            break;
                        }
                        else if(_token == "inline")
                        {

                        }
                        else if(_token == "static")
                        {

                        }
                        else
                        {
                            _internalTmp += _token;
                        }
                    }

                    __ast_entity_function* _entity = nullptr;

                    if(_pAST->m_pPendingScope != nullptr)
                    {
                        _entity = (__ast_entity_function*)_pAST->m_pPendingScope;
                        _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
                        _pAST->m_pPendingScope = nullptr;
                    }
                    else
                    {
                        _entity = new __ast_entity_function();
                    }

                    _entity->m_accessLevel = _pAST->m_currentAccess;
                    _entity->m_sReturnType = _type;
                    _entity->m_sName = _name;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                    _entity->m_pParentScope = _pAST->m_pCurrentScope;

                    if(_roundBracketIndex != std::string::npos)
                    {
                        std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_roundBracketIndex));

                        _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());
                    }

                    _pAST->m_pPendingScope = nullptr;

                    _tmpExpression.clear();

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
            }
            else if(_token == "~")
            {
                uint32_t _mustacheBrackets = 0;
                bool _end = false;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_end || (_mustacheBrackets == 0 && _token == ";"))
                    {
                        break;
                    }
                    else if(_token == "{")
                    {
                        _mustacheBrackets++;

                        while(_pLexer->next_token(&_lexerData, &_token))
                        {
                            if(_mustacheBrackets == 0)
                            {
                                break;
                            }
                            else if(_token == "{")
                            {
                                _mustacheBrackets++;
                            }
                            else if(_token == "}")
                            {
                                _mustacheBrackets--;
                            }
                        }
                    }
                }
            }
            else if(_token == "#")
            {
                bool _wasNewLine = false;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == "\\")
                    {
                        _wasNewLine = true;

                        continue;
                    }
                    else if(_token == "" && _lexerData.m_bNewLine)
                    {
                        if(_wasNewLine)
                        {
                            _wasNewLine = false;
                        }
                        else
                        {
                            break;
                        }
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

                _oneSlash = true;
            }
            else if(_token == ":" && _pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
            {
                __ast_entity_structure* _structure = (__ast_entity_structure*)_pAST->m_pPendingScope;
                __ast_access _access;
                std::string _name;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == "public")
                    {
                        _access = __ast_access::__ast_access_public;
                    }
                    else if(_token == "protected")
                    {
                        _access = __ast_access::__ast_access_public;
                    }
                    else if(_token == "private")
                    {
                        _access = __ast_access::__ast_access_public;
                    }
                    else if(_token == ",")
                    {
                        __ast_inheritance _inheritance;

                        _inheritance.m_protection = _access;
                        _inheritance.m_sName = _name;

                        _structure->m_aInheritance.push_back(_inheritance);

                        _name.clear();
                    }
                    else if(_token == "{")
                    {
                        __ast_inheritance _inheritance;

                        _inheritance.m_protection = _access;
                        _inheritance.m_sName = _name;

                        _structure->m_aInheritance.push_back(_inheritance);

                        _continue = true;

                        break;
                    }
                    else if(_token != "")
                    {
                        _name += _token;
                    }
                }
            }
            else if(_token == ";")
            {
                if(_tmpExpression.size() <= 0)
                {
                    continue;
                }

                __lexer_data _exp = {};

                _exp.m_bWasSpace = false;
                _exp.m_bSpace = false;
                _exp.m_bEnd = false;
                _exp.m_bNewLine = false;
                _exp.m_uiCurrentCharacterIndex = 0;
                _exp.m_uiCurrentLine = 0;

                _exp.m_aLines.push_back(_tmpExpression);
                _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

                bool _wasType = false;
                std::string _internalTmp;
                std::string _type;
                uint32_t _triBrackets = 0;
                uint32_t _roundBrackets = 0;
                std::size_t _roundBracketBeginIndex = std::string::npos;
                bool _callback = false;
                bool _wasOpenBracket = false;
                bool _end = false;
                bool _wasKeyword = false;

                while(!_end && _pLexer->next_token(&_exp, &_token))
                {
                    if(_token == "")
                    {
                        if(_exp.m_bSpace && _triBrackets == 0 && !_wasType && _internalTmp.size() > 0)
                        {
                            if(!_wasKeyword)
                            {
                                _type = _internalTmp;
                                _wasType = true;

                                _internalTmp.clear();
                            }
                            else
                            {
                                _wasKeyword = false;

                                _internalTmp += " ";
                            }
                        }
                    }
                    else if(_token == "<")
                    {
                        _triBrackets++;

                        _internalTmp += _token;
                    }
                    else if(_token == ">")
                    {
                        _triBrackets--;

                        _internalTmp += _token;
                    }
                    else if(_token == "(" && _triBrackets == 0)
                    {
                        _roundBrackets++;
                        _wasOpenBracket = true;
                        _roundBracketBeginIndex = _exp.m_uiCurrentCharacterIndex - 1;

                        while(_pLexer->next_token(&_exp, &_token))
                        {
                            if(_token == "*")
                            {
                                _callback = true;
                                _end = true;

                                _pLexer->next_token(&_exp, &_token);
                                _internalTmp += _token;

                                break;
                            }
                            else if(_token == "")
                            {
                                continue;
                            }
                            else
                            {
                                _end = true;

                                break;
                            }
                        }
                    }
                    else if(_token == ")" && _triBrackets == 0)
                    {
                        _roundBrackets--;
                    }
                    else if(_token == "inline")
                    {

                    }
                    else if(_token == "static")
                    {

                    }
                    else if(_token == "const")
                    {
                        _internalTmp += _token;

                        _wasKeyword = true;
                    }
                    else if(_token == "=")
                    {
                        break;
                    }
                    else
                    {
                        _internalTmp += _token;
                    }
                }

                if(!_callback && !_wasOpenBracket)
                {
                    // Variable

                    __ast_entity_variable* _entity = nullptr;

                    if(_pAST->m_pPendingScope != nullptr)
                    {
                        _entity = (__ast_entity_variable*)_pAST->m_pPendingScope;
                    }
                    else
                    {
                        _entity = new __ast_entity_variable();
                    }

                    _entity->m_sName = _internalTmp;
                    _entity->m_sType = _type;
                    _entity->m_accessLevel = _pAST->m_currentAccess;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                    _entity->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pPendingScope = nullptr;
                }
                else if(_callback)
                {
                    // Callback

                    __ast_entity_callback* _entity = nullptr;

                    if(_pAST->m_pPendingScope != nullptr)
                    {
                        _entity = (__ast_entity_callback*)_pAST->m_pPendingScope;
                    }
                    else
                    {
                        _entity = new __ast_entity_callback();
                    }

                    _entity->m_sReturnType = _type;
                    _entity->m_accessLevel = _pAST->m_currentAccess;
                    _entity->m_sName = _internalTmp;

                    std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex + 1));

                    _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                    _entity->m_pParentScope = _pAST->m_pCurrentScope;

                    _pAST->m_pPendingScope = nullptr;
                }
                else
                {
                    // Function

                    __ast_entity_function* _entity = nullptr;

                    if(_pAST->m_pPendingScope != nullptr)
                    {
                        _entity = (__ast_entity_function*)_pAST->m_pPendingScope;
                    }
                    else
                    {
                        _entity = new __ast_entity_function();
                    }

                    _entity->m_accessLevel = _pAST->m_currentAccess;
                    _entity->m_sReturnType = _type;
                    _entity->m_sName = _internalTmp;

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                    _entity->m_pParentScope = _pAST->m_pCurrentScope;

                    std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _exp.m_sCurrentLine.substr(_roundBracketBeginIndex));

                    _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());

                    _pAST->m_pPendingScope = nullptr;
                }

                _tmpExpression.clear();
            }
            else
            {
                bool _skip = false;

                for(std::string& _define : _pAST->m_aUserDefines)
                {
                    if(_token == _define)
                    {
                        uint32_t _roundBrackets = 0;

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
                            else if(_token == ";" && _roundBrackets == 0)
                            {
                                break;
                            }
                        }

                        _skip = true;

                        break;
                    }
                }

                if(_skip)
                {
                    continue;
                }

                if(_token == "")
                {
                    if(_lexerData.m_bSpace)
                    {
                        if(_tmpExpression.size() > 0 && _tmpExpression[_tmpExpression.size() - 1] != ' ')
                        {
                            _tmpExpression += " ";
                        }
                    }
                }
                else
                {
                    if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_namespace)
                    {
                        ((__ast_entity_namespace*)_pAST->m_pPendingScope)->m_sName = _token;
                    }
                    else if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                    {
                        ((__ast_entity_structure*)_pAST->m_pPendingScope)->m_sName = _token;
                    }
                    else if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                    {
                        ((__ast_entity_enum*)_pAST->m_pPendingScope)->m_sName = _token;
                    }
                    else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure && ((__ast_entity_structure*)_pAST->m_pCurrentScope)->m_sName == _token)
                    {
                        std::string _copy = _token;

                        _pLexer->next_token(&_lexerData, &_token);

                        if(_token == "(")
                        {
                            std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _lexerData.m_sCurrentLine.substr(_lexerData.m_uiCurrentCharacterIndex - 1));
                            uint32_t _mustacheBrackets = 0;
                            uint32_t _roundBrackets = 1;
                            bool _end = false;

                            __ast_entity_constructor* _scope = new __ast_entity_constructor();

                            _scope->m_pParentScope = _pAST->m_pCurrentScope;
                            _scope->m_accessLevel = _pAST->m_currentAccess;

                            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                            _scope->m_aArguments.insert(_scope->m_aArguments.begin(), _args.begin(), _args.end());

                            while(_pLexer->next_token(&_lexerData, &_token))
                            {
                                if((_token == ";" && _mustacheBrackets == 0) || _end)
                                {
                                    break;
                                }
                                else if(_token == "(")
                                {
                                    _roundBrackets++;
                                }
                                else if(_token == ")")
                                {
                                    _roundBrackets--;
                                }
                                else if(_token == "{" && _roundBrackets == 0)
                                {
                                    _mustacheBrackets++;

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
                                                _end = true;

                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            _tmpExpression += _copy;
                            _tmpExpression += _token;

                            continue;
                        }
                        
                    }
                    else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                    {
                        __ast_entity_enum* _enum = (__ast_entity_enum*)_pAST->m_pCurrentScope;

                        if(_token == ",")
                        {

                        }
                        else if(_token != "")
                        {
                            _enum->m_aElements.push_back(_token);
                        }
                    }
                    else
                    {
                        _tmpExpression += _token;
                    }
                }
            }
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