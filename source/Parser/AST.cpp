#include "Parser/AST.h"

#include <stack>
#include <queue>

#include "Utils/Utils.h"

#define DUCKVIL_SPACES \
    for(uint32_t i = 0; i < _uiScope; ++i) \
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
            printf("Structure %s\n", (static_cast<const __ast_entity_structure*>(_entity))->m_sName.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_namespace:
            printf("Namespace %s\n", (static_cast<const __ast_entity_namespace*>(_entity))->m_sName.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_enum:
            printf("Enum %s\n", (static_cast<const __ast_entity_enum*>(_entity))->m_sName.c_str());
            for(const std::string& _element : (static_cast<const __ast_entity_enum*>(_entity))->m_aElements)
            {
                DUCKVIL_SPACES
                printf(" Element %s\n", _element.c_str());
            }
            break;
        case __ast_entity_type::__ast_entity_type_variable:
            printf("Variable\n");
            DUCKVIL_SPACES
            printf(" Name: %s\n", (static_cast<const __ast_entity_variable*>(_entity))->m_sName.c_str());
            DUCKVIL_SPACES
            printf(" Type: %s\n", (static_cast<const __ast_entity_variable*>(_entity))->m_sType.c_str());
            break;
        case __ast_entity_type::__ast_entity_type_constructor:
            printf("Constructor\n");
            break;
        case __ast_entity_type::__ast_entity_type_callback:
            printf("Callback\n");
            break;
        case __ast_entity_type::__ast_entity_type_function:
            printf("Function\n");
            DUCKVIL_SPACES
            printf(" Name: %s\n", (static_cast<const __ast_entity_function*>(_entity))->m_sName.c_str());
            DUCKVIL_SPACES
            printf(" Return: %s\n", (static_cast<const __ast_entity_function*>(_entity))->m_sReturnType.c_str());
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

    std::vector<__ast_meta> parse_meta(__lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        std::vector<__ast_meta> _result;
        std::string _token;
        uint32_t _roundBrackets = 0;
        std::string _tmp_expression;
        std::string _key;
        bool _wasEqual = false;

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

                    if(_wasEqual)
                    {
                        _meta.m_sKey = _key;
                        _meta.m_sValue = _tmp_expression;
                    }
                    else
                    {
                        if(_tmp_expression == "")
                        {
                            break;
                        }

                        _meta.m_sKey = _tmp_expression;
                        _meta.m_sValue = "true";
                    }

                    _result.push_back(_meta);

                    break;
                }
            }
            else if(_token == ",")
            {
                __ast_meta _meta = {};

                if(_wasEqual)
                {
                    _meta.m_sKey = _key;
                    _meta.m_sValue = _tmp_expression;
                }
                else
                {
                    _meta.m_sKey = _tmp_expression;
                    _meta.m_sValue = "true";
                }

                _result.push_back(_meta);

                _tmp_expression.clear();
                _wasEqual = false;
            }
            else if(_token == "=")
            {
                _key = _tmp_expression;
                _wasEqual = true;

                _tmp_expression.clear();
            }
            else
            {
                _tmp_expression += _token;
            }
        }

        return _result;
    }

    std::vector<__ast_entity_argument> process_arguments(__lexer_ftable* _pLexer, __lexer_data& _lexerData, __ast* _pAST, const std::string& _sArgs)
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
        std::string _callbackMemberType;
        bool _keyword = false;
        bool _continue = false;
        bool _wasType = false;
        std::vector<__ast_meta> _argumentMetas;
        std::string _value;

        while(_continue || _pLexer->next_token(&_exp, &_token))
        {
            _continue = false;

            if(_token == "DUCKVIL_ARGUMENT")
            {
                _argumentMetas = parse_meta(_pLexer, _exp);

                _pLexer->next_token(&_exp, &_token);
                _pLexer->next_token(&_exp, &_token);
            }
            else if(_token == "(")
            {
                _roundBrackets++;

                // if(_triBrackets == 0 && _wasType)
                // {
                //     if(_tmp == "")
                //     {
                //         _pLexer->next_token(&_exp, &_token);

                //         if(_token == "*")
                //         {
                //             printf("POSSIBLE CALLBACK!\n");

                //             _tmp += _token;
                //             _callbackMemberType = _tmp;

                //             _pLexer->next_token(&_exp, &_token); // name

                //             _tmp = _token;

                //             _pLexer->next_token(&_exp, &_token); // )

                //             std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, _lexerData, _pLexerData->m_sCurrentLine.substr(_openBracketIndex));
                //         }
                //         else
                //         {
                //             _continue = true;
                //         }
                //     }
                //     else
                //     {

                //     }
                // }
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

                    _arg.m_aMeta = _argumentMetas;
                    _arg.m_sValue = _value;

                    _argumentMetas.clear();

                    if(_type == "")
                    {
                        _arg.m_sType = _tmp;
                    }
                    else
                    {
                        _arg.m_sType = _type;
                        _arg.m_sName = _tmp;
                    }

                    _res.push_back(_arg);

                    _tmp.clear();
                    _value.clear();
                }
            }
            else if(_token == "<")
            {
                _triBrackets++;

                _tmp += _token;
            }
            else if(_token == ">")
            {
                _triBrackets--;

                _tmp += _token;
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
            else if(_token == "*" && _wasType)
            {
                std::string _tmpName;

                _tmp += _token;
                _callbackMemberType = _tmp;
                _tmp.clear();

                _pLexer->next_token(&_exp, &_token); // name

                _tmpName = _token;

                _pLexer->next_token(&_exp, &_token); // )

                uint32_t _roundBracketsCallback = 0;
                std::string _callbackArguments;

                while(_pLexer->next_token(&_exp, &_token))
                {
                    if(_token == "(")
                    {
                        _roundBracketsCallback++;
                    }
                    else if(_token == ")")
                    {
                        _roundBracketsCallback--;

                        if(_roundBracketsCallback == 0)
                        {
                            break;
                        }
                    }
                    else
                    {
                        _callbackArguments += _token;

                        if(_exp.m_bWasSpace)
                        {
                            _callbackArguments += " ";
                        }
                    }
                }

                _type = _type + "(" + _callbackMemberType + ")(" + _callbackArguments + ")";

                _tmp = _tmpName;

                _wasType = false;
            }
            else if(_triBrackets == 0 && _token == ",")
            {
                if(_type.size() <= 0 && _tmp.size() <= 0)
                {
                    continue;
                }

                __ast_entity_argument _arg = {};

                _arg.m_aMeta = _argumentMetas;
                _arg.m_sValue = _value;

                _argumentMetas.clear();

                if(_type == "")
                {
                    _arg.m_sType = _tmp;
                }
                else
                {
                    _arg.m_sType = _type;
                    _arg.m_sName = _tmp;
                }

                _res.push_back(_arg);

                _tmp.clear();
                _value.clear();

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

                    if(_token != "")
                    {
                        _value += _token;
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
                bool _internalContinue = false;

                for(const user_define& _define : _pAST->m_aUserDefines)
                {
                    if(_internalContinue)
                    {
                        break;
                    }

                    if(_token == _define.m_sUserDefine)
                    {
                        _internalContinue = true;

                        _tmp += _token;

                        bool _end = false;
                        uint32_t _internalRoundBrackets = 0;

                        while(!_end && _pLexer->next_token(&_exp, &_token))
                        {
                            if(_token == "(")
                            {
                                _internalRoundBrackets++;
                            }
                            else if(_token == ")")
                            {
                                _internalRoundBrackets--;

                                if(_internalRoundBrackets == 0)
                                {
                                    _end = true;
                                }
                            }
                            else if((_exp.m_bNewLine || _token == ";") && _internalRoundBrackets == 0)
                            {
                                break;
                            }

                            _tmp += _token;
                        }
                    }
                }

                if(!_internalContinue)
                {
                    _tmp += _token;
                }
            }
        }

        return _res;
    }

    __behavior check_define(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data* _pLexerData, std::string& _sToken, std::string* _spCurrent)
    {
        if(_sToken == "DUCKVIL_RUNTIME_REFLECTION_SKIP")
        {
            return __behavior::__behavior_Skip;
        }
        else if(_sToken == "DUCKVIL_RUNTIME_REFLECTION_PAUSE")
        {
            return __behavior::__behavior_Pause;
        }

        for(const user_define& _define : _pAST->m_aUserDefines)
        {
            if(_sToken == _define.m_sUserDefine)
            {
                // uint32_t _roundBrackets = 0;

                // while(_pLexer->next_token(_pLexerData, &_sToken))
                // {
                //     if(_sToken == "(")
                //     {
                //         _roundBrackets++;
                //     }
                //     else if(_sToken == ")")
                //     {
                //         _roundBrackets--;

                //         if(_roundBrackets == 0)
                //         {
                //             break;
                //         }
                //     }
                //     else if((_pLexerData->m_bNewLine || _sToken == ";") && _roundBrackets == 0)
                //     {
                //         break;
                //     }
                // }

                // __ast_entity_define* _defineEntity = new __ast_entity_define();

                // _defineEntity->m_sName = _define.m_sUserDefine;
                // _defineEntity->m_pParentScope = _pAST->m_pCurrentScope;

                // _pAST->m_pCurrentScope->m_aScopes.push_back(_defineEntity);

                return _define.m_fnBehavior(_pAST, _pLexer, _pLexerData, _sToken, _spCurrent);
            }
        }

        return __behavior::__behavior_Nothing;
    }

    void skip_body(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, std::string& _sToken)
    {
        uint32_t _mustacheBrackets = 0;
        uint32_t _roundBrackets = 1;
        bool _end = false;

        while(_pLexer->next_token(_pLexerData, &_sToken))
        {
            if((_sToken == ";" && _mustacheBrackets == 0) || _end)
            {
                break;
            }
            else if(_sToken == "(")
            {
                _roundBrackets++;
            }
            else if(_sToken == ")")
            {
                _roundBrackets--;
            }
            else if(_sToken == "{" && _roundBrackets == 0)
            {
                _mustacheBrackets++;

                while(_pLexer->next_token(_pLexerData, &_sToken))
                {
                    if(_sToken == "{")
                    {
                        _mustacheBrackets++;
                    }
                    else if(_sToken == "}")
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

    void parse_template(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, __ast* _pAST, std::string& _sToken)
    {
        uint32_t _triBrackets = 0;
        std::string _type;
        std::string _internal;
        std::vector<__ast_template_element> _elements;

        while(_pLexer->next_token(_pLexerData, &_sToken))
        {
            if(_sToken == "<")
            {
                _triBrackets++;
            }
            else if(_sToken == ">")
            {
                _triBrackets--;

                if(_triBrackets == 0)
                {
                    __ast_template_element _element;

                    _element.m_sType = _type;
                    _element.m_sName = _internal;

                    _elements.push_back(_element);

                    __ast_template _template;

                    _template.m_aElements.insert(_template.m_aElements.begin(), _elements.begin(), _elements.end());

                    _pAST->m_aTemplates.push_back(_template);

                    break;
                }
            }
            else if(_sToken == ",")
            {
                __ast_template_element _element;

                _element.m_sType = _type;
                _element.m_sName = _internal;

                _elements.push_back(_element);

                _internal.clear();
            }
            else if(_sToken == "" && _pLexerData->m_bSpace && _internal.size() > 0)
            {
                _type = _internal;

                _internal.clear();
            }
            else
            {
                _internal += _sToken;
            }
        }
    }

    void parse_typedefs(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, __ast* _pAST, std::string& _sToken)
    {
        std::string _internal;
        std::size_t _openBracketIndex = std::string::npos;
        uint32_t _roundBrackets = 0;
        std::string _type;
        bool _wasOpenBracket = false;
        std::string _name;
        bool _skip = false;

        while(_pLexer->next_token(_pLexerData, &_sToken))
        {
            if(_sToken == ";")
            {
                break;
            }
            else if(_sToken == "(")
            {
                _roundBrackets++;
                _openBracketIndex = _pLexerData->m_uiCurrentCharacterIndex - 1;
                _wasOpenBracket = true;

                if(_skip)
                {
                    continue;
                }

                while(_pLexer->next_token(_pLexerData, &_sToken))
                {
                    if(_sToken == "*")
                    {

                    }
                    else if(_sToken == ")")
                    {
                        _name = _internal;
                        _skip = true;

                        break;
                    }
                    else
                    {
                        _internal += _sToken;
                    }
                }
            }
            else if(_sToken == ")")
            {
                _roundBrackets--;
            }
            else if(_sToken == "" && _pLexerData->m_bSpace && _internal.size() > 0 && !_wasOpenBracket)
            {
                _type = _internal;

                _internal.clear();
            }
            else
            {
                if(!_skip)
                {
                    _internal += _sToken;
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

            std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, *_pLexerData, _pAST, _pLexerData->m_sCurrentLine.substr(_openBracketIndex));

            _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());

            _entity->m_pParentScope = _pAST->m_pCurrentScope;

            _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);
        }
    }

    void process_open_mustache_bracket(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, __ast* _pAST, std::string& _sToken, std::string& _sTmpExpression, bool& _bContinue)
    {
        if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType != __ast_entity_type::__ast_entity_type_function)
        {
            _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
            _pAST->m_pPendingScope = nullptr;

            return;
        }

        uint32_t _mustacheBrackets = 1;
        __lexer_data _exp = {};

        _exp.m_bWasSpace = false;
        _exp.m_bSpace = false;
        _exp.m_bEnd = false;
        _exp.m_bNewLine = false;
        _exp.m_uiCurrentCharacterIndex = 0;
        _exp.m_uiCurrentLine = 0;

        _exp.m_aLines.push_back(_sTmpExpression);
        _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

        bool _wasType = false;
        bool _wasName = false;
        uint32_t _triBrackets = 0;
        std::string _type;
        std::string _name;
        std::string _internalTmp;
        std::size_t _roundBracketIndex = std::string::npos;
        std::size_t _quadBrackets = 0;
        std::size_t _roundBrackets = 0;
        bool _variable = false;
        bool _skipped = false;
        __ast_flags _flags = {};

        while(_pLexer->next_token(&_exp, &_sToken))
        {
            if(_sToken == "")
            {
                if(_exp.m_bSpace && _triBrackets == 0 && !_wasType && !_skipped)
                {
                    _type = _internalTmp;
                    _wasType = true;

                    _internalTmp.clear();
                }
                else if(_exp.m_bSpace && _triBrackets == 0 && _wasType && !_wasName)
                {
                    _name = _internalTmp;
                    _wasName = true;

                    _internalTmp.clear();
                }
                else if(_skipped)
                {
                    _skipped = false;

                    if(_internalTmp != "" && _internalTmp[_internalTmp.size() - 1] != ' ')
                    {
                        _internalTmp += " ";
                    }
                }
            }
            else if(_sToken == "<")
            {
                _triBrackets++;

                _internalTmp += _sToken;
            }
            else if(_sToken == ">")
            {
                _triBrackets--;

                _internalTmp += _sToken;
            }
            else if(_sToken == "[")
            {
                _quadBrackets++;

                while(_pLexer->next_token(&_exp, &_sToken))
                {
                    if(_sToken == "]")
                    {
                        _quadBrackets--;

                        if(_quadBrackets == 0)
                        {
                            break;
                        }
                    }
                }
            }
            else if(_sToken == "(" && _triBrackets == 0)
            {
                _roundBracketIndex = _exp.m_uiCurrentCharacterIndex - 1;
                // _name = _internalTmp;

                _roundBrackets++;
            }
            else if(_sToken == ")" && _triBrackets == 0)
            {
                _roundBrackets--;
            }
            else if (_roundBrackets != 0)
            {
                continue;
            }
            else if(_sToken == "{" && _roundBrackets == 0)
            {
                break;
            }
            else if(_sToken == "=" && _wasType && _wasName)
            {
                _variable = true;

                break;
            }
            else if(_sToken == "inline")
            {
                _skipped = true;
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_inline));
            }
            else if(_sToken == "static")
            {
                _skipped = true;
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_static));
            }
            else if(_sToken == "const")
            {
                if(_wasType && _wasName && !_variable)
                {
                    _skipped = true;
                    _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_const));
                }
                else
                {
                    _internalTmp += _sToken;
                    _skipped = true;
                }
            }
            else if(_sToken == "virtual")
            {
                _skipped = true;
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_virtual));
            }
            else if(_sToken == "override")
            {
                _skipped = true;
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_override));
            }
            else
            {
                if(_sToken.find("operator") != std::string::npos)
                {
                    _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_operator));
                }

                _internalTmp += _sToken;
            }
        }

        if(!_variable)
        {
            __ast_entity_function* _entity = nullptr;

            if(_pAST->m_pPendingScope != nullptr)
            {
                _entity = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);
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
            _entity->m_flags = _flags;
            _entity->m_aTemplates.insert(_entity->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

            _pAST->m_aTemplates.clear();

            _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

            _entity->m_pParentScope = _pAST->m_pCurrentScope;

            if(_roundBracketIndex != std::string::npos)
            {
                std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, *_pLexerData, _pAST, _exp.m_sCurrentLine.substr(_roundBracketIndex));

                _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());
            }

            _pAST->m_pPendingScope = nullptr;

            _sTmpExpression.clear();

            while(_pLexer->next_token(_pLexerData, &_sToken))
            {
                if(_sToken == "{")
                {
                    _mustacheBrackets++;
                }
                else if(_sToken == "}")
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
            __ast_entity_variable* _entity = nullptr;

            if(_pAST->m_pPendingScope != nullptr)
            {
                _entity = static_cast<__ast_entity_variable*>(_pAST->m_pPendingScope);
                _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
                _pAST->m_pPendingScope = nullptr;
            }
            else
            {
                _entity = new __ast_entity_variable();
            }

            _entity->m_accessLevel = _pAST->m_currentAccess;
            _entity->m_sType = _type;
            _entity->m_sName = _name;

            if(_pAST->m_bPendingIfdef)
            {
                _entity->m_aNeededDefines.push_back(_pAST->m_sCurrentDefineNeeded);
            }

            _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

            _entity->m_pParentScope = _pAST->m_pCurrentScope;

            _pAST->m_pPendingScope = nullptr;

            _sTmpExpression.clear();

            while(_pLexer->next_token(_pLexerData, &_sToken))
            {
                if(_sToken == "{")
                {
                    _mustacheBrackets++;
                }
                else if(_sToken == "}")
                {
                    _mustacheBrackets--;

                    if(_mustacheBrackets == 0)
                    {
                        break;
                    }
                }
            }

            while(_pLexer->next_token(_pLexerData, &_sToken))
            {
                if(_sToken == ";")
                {
                    break;
                }
                else if(_sToken != "")
                {
                    _bContinue = true;

                    break;
                }
            }
        }
    }

    void process_semicolon(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, __ast* _pAST, std::string& _sToken, std::string& _sTmpExpression)
    {
        if(_sTmpExpression.size() <= 0)
        {
            return;
        }

        __lexer_data _exp = {};

        _exp.m_bWasSpace = false;
        _exp.m_bSpace = false;
        _exp.m_bEnd = false;
        _exp.m_bNewLine = false;
        _exp.m_uiCurrentCharacterIndex = 0;
        _exp.m_uiCurrentLine = 0;

        _exp.m_aLines.push_back(_sTmpExpression);
        _exp.m_sCurrentLine = _exp.m_aLines[_exp.m_uiCurrentLine];

        bool _wasType = false;
        std::string _internalTmp;
        std::string _type;
        std::string _callbackMemberType;
        uint32_t _triBrackets = 0;
        uint32_t _roundBrackets = 0;
        uint32_t _quadBrackets = 0;
        std::size_t _roundBracketBeginIndex = std::string::npos;
        bool _callback = false;
        bool _wasOpenBracket = false;
        bool _end = false;
        bool _wasKeyword = false;
        __ast_flags _flags = {};

        while(!_end && _pLexer->next_token(&_exp, &_sToken))
        {
            if(_sToken == "")
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
            else if(_sToken == "<")
            {
                _triBrackets++;

                _internalTmp += _sToken;
            }
            else if(_sToken == ">")
            {
                _triBrackets--;

                _internalTmp += _sToken;
            }
            else if(_sToken == "[" && _wasType)
            {
                _quadBrackets++;

                while(_pLexer->next_token(&_exp, &_sToken))
                {
                    if(_sToken == "]")
                    {
                        _quadBrackets--;

                        if(_quadBrackets == 0)
                        {
                            break;
                        }
                    }
                }
            }
            else if(_sToken == "(" && _triBrackets == 0 && _wasType)
            {
                _roundBrackets++;
                _wasOpenBracket = true;
                _roundBracketBeginIndex = _exp.m_uiCurrentCharacterIndex - 1;

                if(_internalTmp == "")
                {
                    // callback

                    _callback = true;

                    while(_pLexer->next_token(&_exp, &_sToken))
                    {
                        if(_sToken == "*")
                        {
                            _end = true;

                            _internalTmp += _sToken;
                            _callbackMemberType = _internalTmp;

                            _pLexer->next_token(&_exp, &_sToken);

                            _internalTmp = _sToken;

                            break;
                        }
                        else
                        {
                            _internalTmp += _sToken;
                        }
                    }
                }
                else
                {
                    _end = true;
                }
            }
            else if(_sToken == ")" && _triBrackets == 0)
            {
                _roundBrackets--;

                if(!_wasType)
                {
                    _internalTmp += _sToken;
                }
            }
            else if(_sToken == "inline")
            {
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_inline));
            }
            else if(_sToken == "static")
            {
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_static));
            }
            else if(_sToken == "const")
            {
                _internalTmp += _sToken;
                _wasKeyword = true;
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_const));
            }
            else if(_sToken == "virtual")
            {
                _flags = static_cast<__ast_flags>(static_cast<uint8_t>(_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_virtual));
            }
            else if(_sToken == "=")
            {
                break;
            }
            else
            {
                _internalTmp += _sToken;
            }
        }

        if(!_callback)
        {
            if(_wasOpenBracket)
            {
                // Function

                __ast_entity_function* _entity = nullptr;

                if(_pAST->m_pPendingScope != nullptr)
                {
                    _entity = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);
                }
                else
                {
                    _entity = new __ast_entity_function();
                }

                _entity->m_aTemplates.insert(_entity->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

                _pAST->m_aTemplates.clear();

                _entity->m_accessLevel = _pAST->m_currentAccess;
                _entity->m_sReturnType = _type;
                _entity->m_sName = _internalTmp;
                _entity->m_flags = _flags;

                if(_pAST->m_bPendingIfdef)
                {
                    _entity->m_aNeededDefines.push_back(_pAST->m_sCurrentDefineNeeded);
                }

                _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                _entity->m_pParentScope = _pAST->m_pCurrentScope;

                std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, *_pLexerData, _pAST, _exp.m_sCurrentLine.substr(_roundBracketBeginIndex));

                _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());

                _pAST->m_pPendingScope = nullptr;
            }
            else
            {
                // Variable

                __ast_entity_variable* _entity = nullptr;

                if(_pAST->m_pPendingScope != nullptr)
                {
                    _entity = static_cast<__ast_entity_variable*>(_pAST->m_pPendingScope);
                }
                else
                {
                    _entity = new __ast_entity_variable();
                }

                _entity->m_sName = _internalTmp;
                _entity->m_sType = _type;
                _entity->m_accessLevel = _pAST->m_currentAccess;

                if(_pAST->m_bPendingIfdef)
                {
                    _entity->m_aNeededDefines.push_back(_pAST->m_sCurrentDefineNeeded);
                }

                _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

                _entity->m_pParentScope = _pAST->m_pCurrentScope;

                _pAST->m_pPendingScope = nullptr;
            }
        }
        else
        {
            // Callback

            __ast_entity_callback* _entity = nullptr;

            if(_pAST->m_pPendingScope != nullptr)
            {
                _entity = static_cast<__ast_entity_callback*>(_pAST->m_pPendingScope);
            }
            else
            {
                _entity = new __ast_entity_callback();
            }

            _entity->m_sReturnType = _type;
            _entity->m_accessLevel = _pAST->m_currentAccess;
            _entity->m_sName = _internalTmp;
            _entity->m_sMemberType = _callbackMemberType;

            if(_pAST->m_bPendingIfdef)
            {
                _entity->m_aNeededDefines.push_back(_pAST->m_sCurrentDefineNeeded);
            }

            std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, *_pLexerData, _pAST, _exp.m_sCurrentLine.substr(_exp.m_uiCurrentCharacterIndex + 1));

            _entity->m_aArguments.insert(_entity->m_aArguments.begin(), _args.begin(), _args.end());

            _pAST->m_pCurrentScope->m_aScopes.push_back(_entity);

            _entity->m_pParentScope = _pAST->m_pCurrentScope;

            _pAST->m_pPendingScope = nullptr;
        }

        _sTmpExpression.clear();
    }

    void process_inheritance(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, __ast* _pAST, std::string& _sToken, bool& _bContinue)
    {
        __ast_entity_structure* _structure = static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope);
        __ast_access _access = __ast_access::__ast_access_not_specified;
        std::string _name;

        while(_pLexer->next_token(_pLexerData, &_sToken))
        {
            if(_sToken == "public")
            {
                _access = __ast_access::__ast_access_public;
            }
            else if(_sToken == "protected")
            {
                _access = __ast_access::__ast_access_protected;
            }
            else if(_sToken == "private")
            {
                _access = __ast_access::__ast_access_private;
            }
            else if(_sToken == ",")
            {
                __ast_inheritance _inheritance;

                _inheritance.m_protection = _access;
                _inheritance.m_sName = _name;

                _structure->m_aInheritance.push_back(_inheritance);

                _name.clear();
            }
            else if(_sToken == "{")
            {
                __ast_inheritance _inheritance;

                _inheritance.m_protection = _access;
                _inheritance.m_sName = _name;

                _structure->m_aInheritance.push_back(_inheritance);

                _bContinue = true;

                break;
            }
            else if(_sToken != "")
            {
                _name += _sToken;
            }
        }
    }

    bool g_bPause = false;

    void process_pending(__lexer_ftable* _pLexer, __lexer_data* _pLexerData, __ast* _pAST, std::string& _sToken, std::string& _sTmpExpression, bool& _bContinue)
    {
        if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_namespace)
        {
            (static_cast<__ast_entity_namespace*>(_pAST->m_pPendingScope))->m_sName = _sToken;
        }
        else if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
        {
            (static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_sName = _sToken;
            uint32_t _triBrackets = 0;

            while(_pLexer->next_token(_pLexerData, &_sToken))
            {
                if(_sToken == "<")
                {
                    _triBrackets++;
                }
                else if(_sToken == ">")
                {
                    _triBrackets--;

                    if(_triBrackets == 0)
                    {
                        break;
                    }
                }
                else if(_sToken == ";" && _triBrackets == 0)
                {
                    _pAST->m_pPendingScope = nullptr;

                    break;
                }
                else if(_sToken != "" && _triBrackets == 0)
                {
                    _bContinue = true;

                    break;
                }
            }
        }
        else if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
        {
            (static_cast<__ast_entity_enum*>(_pAST->m_pPendingScope))->m_sName = _sToken;
        }
        else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure && (static_cast<__ast_entity_structure*>(_pAST->m_pCurrentScope))->m_sName == _sToken)
        {
            std::string _copy = _sToken;

            _pLexer->next_token(_pLexerData, &_sToken);

            if(_sToken == "(")
            {
                __ast_entity_constructor* _scope = nullptr;

                if(_pAST->m_pPendingScope != nullptr)
                {
                    _scope = static_cast<__ast_entity_constructor*>(_pAST->m_pPendingScope);
                }
                else
                {
                    _scope = new __ast_entity_constructor();
                }

                _scope->m_aTemplates.insert(_scope->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

                _pAST->m_aTemplates.clear();

                _scope->m_pParentScope = _pAST->m_pCurrentScope;
                _scope->m_accessLevel = _pAST->m_currentAccess;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                std::vector<__ast_entity_argument> _args = process_arguments(_pLexer, *_pLexerData, _pAST, _pLexerData->m_sCurrentLine.substr(_pLexerData->m_uiCurrentCharacterIndex - 1));

                _scope->m_aArguments.insert(_scope->m_aArguments.begin(), _args.begin(), _args.end());

                skip_body(_pLexer, _pLexerData, _sToken);
            }
            else
            {
                _sTmpExpression += _copy;
                _sTmpExpression += _sToken;

                return;
            }

            _pAST->m_pPendingScope = nullptr;
        }
        else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
        {
            __ast_entity_enum* _enum = static_cast<__ast_entity_enum*>(_pAST->m_pCurrentScope);

            if(_sToken == ",")
            {
                g_bPause = false;
            }
            else if(_sToken == "=")
            {
                g_bPause = true;
            }
            else if(!g_bPause && _sToken != "")
            {
                _enum->m_aElements.push_back(_sToken);
            }
        }
        else
        {
            _sTmpExpression += _sToken;
        }
    }

    void ast_generate(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        _pAST->m_pCurrentScope = &_pAST->m_main;
        _pAST->m_pPendingScope = _pAST->m_pCurrentScope;
        _pAST->m_currentAccess = __ast_access::__ast_access_not_specified;

        std::string _token;
        std::string _tmpExpression;
        uint32_t _ifdefCount = 0;
        uint32_t _roundBrackets = 0;

        bool _oneSlash = false;
        bool _continue = false;
        bool _wasContinue = false;
        bool _skipRest = false;
        bool _paused = false;

        while(_continue || _pLexer->next_token(&_lexerData, &_token))
        {
            if(_paused)
            {
                if(_token == "DUCKVIL_RUNTIME_REFLECTION_RESUME")
                {
                    _paused = false;
                }

                continue;
            }

            if(_skipRest)
            {
                printf("Skipping: %s\n", _pAST->m_sFile.string().c_str());

                break;
            }

            _wasContinue = _continue;
            _continue = false;

            if(_token == "DUCKVIL_GENERATED_BODY")
            {
                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    __ast_entity_structure* _scope = static_cast<__ast_entity_structure*>(_pAST->m_pCurrentScope);

                    _scope->m_uiGeneratedBodyMacroLine = _lexerData.m_uiCurrentLine;
                }
            }
            else if(_token == "DUCKVIL_STRUCT")
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
            else if(_token == "DUCKVIL_CONSTRUCTOR")
            {
                __ast_entity_constructor* _scope = new __ast_entity_constructor();

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
            else if(_token == "friend")
            {
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == ";")
                    {
                        break;
                    }
                }
            }
            else if(_token == "class")
            {
                if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    _pAST->m_pPendingScope->m_pParentScope = _pAST->m_pCurrentScope;

                    (static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_accessLevel = _pAST->m_currentAccess;
                    (static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_aTemplates.insert((static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

                    _pAST->m_aTemplates.clear();

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_pAST->m_pPendingScope);
                }
                else if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                {
                    continue;
                }
                else
                {
                    __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);

                    _scope->m_accessLevel = _pAST->m_currentAccess;
                    _scope->m_aTemplates.insert(_scope->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

                    _pAST->m_aTemplates.clear();

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

                    (static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_accessLevel = _pAST->m_currentAccess;
                    (static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_aTemplates.insert((static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope))->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

                    _pAST->m_aTemplates.clear();

                    _pAST->m_pCurrentScope->m_aScopes.push_back(_pAST->m_pPendingScope);
                }
                else
                {
                    __ast_entity_structure* _scope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_struct);

                    _scope->m_accessLevel = _pAST->m_currentAccess;
                    _scope->m_aTemplates.insert(_scope->m_aTemplates.begin(), _pAST->m_aTemplates.begin(), _pAST->m_aTemplates.end());

                    _pAST->m_aTemplates.clear();

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
                parse_template(_pLexer, &_lexerData, _pAST, _token);
            }
            else if(_token == "typedef")
            {
                parse_typedefs(_pLexer, &_lexerData, _pAST, _token);
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
            else if(!_wasContinue && _token == "(")
            {
                _roundBrackets++;

                _continue = true;
            }
            else if(!_wasContinue && _token == ")")
            {
                _roundBrackets--;

                _continue = true;
            }
            else if(_roundBrackets == 0 && _token == "{")
            {
                process_open_mustache_bracket(_pLexer, &_lexerData, _pAST, _token, _tmpExpression, _continue);
            }
            else if(_roundBrackets == 0 && _token == "}")
            {
                if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
                {
                    _pLexer->next_token(&_lexerData, &_token); // Skip ;
                }
                else if(_pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
                {
                    _pLexer->next_token(&_lexerData, &_token); // Skip ;
                }

                if(_pAST->m_bPendingIfdef)
                {
                    _pAST->m_pCurrentScope->m_aNeededDefines.push_back(_pAST->m_sCurrentDefineNeeded);
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
                __ast_entity_destructor* _scope = nullptr;

                if(_pAST->m_pPendingScope != nullptr)
                {
                    _scope = static_cast<__ast_entity_destructor*>(_pAST->m_pPendingScope);
                }
                else
                {
                    _scope = new __ast_entity_destructor();
                }

                _scope->m_pParentScope = _pAST->m_pCurrentScope;
                _scope->m_accessLevel = _pAST->m_currentAccess;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                // skip_body(_pLexer, &_lexerData, _token);

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
                                _end = true;

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
            else if(_pAST->m_bPendingIfdef && _token == "ifdef")
            {
                _ifdefCount++;

                _pLexer->next_token(&_lexerData, &_token);
                _pLexer->next_token(&_lexerData, &_pAST->m_sCurrentDefineNeeded); // ifdef name
            }
            else if(_pAST->m_bPendingIfdef && _token == "if")
            {
                uint32_t _ifdefCountInternal = 1;

                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token == "if")
                    {
                        _ifdefCountInternal++;
                    }
                    else if(_token == "endif")
                    {
                        _ifdefCountInternal--;

                        if(_ifdefCountInternal == 0)
                        {
                            _pAST->m_bPendingIfdef = false;

                            break;
                        }
                    }
                }
            }
            else if(_pAST->m_bPendingIfdef && _token == "endif")
            {
                _ifdefCount--;

                if(_ifdefCount == 0)
                {
                    _pAST->m_bPendingIfdef = false;
                    _pAST->m_sCurrentDefineNeeded.clear();
                }
            }
            else if(_pAST->m_bPendingIfdef && _token == "undef")
            {
                _pLexer->next_token(&_lexerData, &_token);
                _pLexer->next_token(&_lexerData, &_pAST->m_sCurrentDefineNeeded); // undef name

                _pAST->m_bPendingIfdef = false;
            }
            else if(_pAST->m_bPendingIfdef && _token == "else")
            {
                continue;
            }
            else if(_pAST->m_bPendingIfdef && _token == "define")
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

                _pAST->m_bPendingIfdef = false;
            }
            else if(_pAST->m_bPendingIfdef && _token == "include")
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

                _pAST->m_bPendingIfdef = false;
            }
            else if(_pAST->m_bPendingIfdef && _token == "pragma")
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

                _pAST->m_bPendingIfdef = false;
            }
            else if(_token == "#")
            {
                _pAST->m_bPendingIfdef = true;
                // bool _wasNewLine = false;
                // uint32_t _ifdefCount = 0;
                // bool _break = false;

                // while(!_break && _pLexer->next_token(&_lexerData, &_token))
                // {
                //     if(_token == "\\")
                //     {
                //         _wasNewLine = true;

                //         continue;
                //     }
                //     else if(_token == "ifdef")
                //     {
                //         _ifdefCount++;

                //         _pLexer->next_token(&_lexerData, &_token);
                //         _pLexer->next_token(&_lexerData, &_token);

                //         for(const auto& _define : _pAST->m_aUserDefines)
                //         {
                //             if(_token == _define.m_sUserDefine)
                //             {
                //                 printf("AAAAA\n");
                //             }
                //         }

                //         while(_pLexer->next_token(&_lexerData, &_token))
                //         {
                //             if(_token == "ifdef")
                //             {
                //                 _ifdefCount++;
                //             }
                //             else if(_token == "endif")
                //             {
                //                 _ifdefCount--;

                //                 if(_ifdefCount == 0)
                //                 {
                //                     _break = true;

                //                     break;
                //                 }
                //             }
                //         }
                //     }
                //     else if(_token == "" && _lexerData.m_bNewLine)
                //     {
                //         if(_wasNewLine)
                //         {
                //             _wasNewLine = false;
                //         }
                //         else
                //         {
                //             break;
                //         }
                //     }
                // }
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
                process_inheritance(_pLexer, &_lexerData, _pAST, _token, _continue);
            }
            else if(_token == ":" && _pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_enum)
            {
                while(_pLexer->next_token(&_lexerData, &_token))
                {
                    if(_token != "")
                    {
                        break;
                    }
                }
            }
            else if(_token == ";")
            {
                process_semicolon(_pLexer, &_lexerData, _pAST, _token, _tmpExpression);
            }
            else
            {
                // if(check_define(_pAST, _pLexer, &_lexerData, _token, &_tmpExpression))
                // {
                //     continue;
                // }

                switch (check_define(_pAST, _pLexer, &_lexerData, _token, &_tmpExpression))
                {
                case __behavior::__behavior_Nothing:
                    break;
                case __behavior::__behavior_Pause:
                    _paused = true;

                    continue;
                    break;
                case __behavior::__behavior_Proceed:
                    continue;
                    break;
                case __behavior::__behavior_Resume:
                    break;
                case __behavior::__behavior_Skip:
                    _skipRest = true;
                    continue;
                    break;
                default:
                    break;
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
                    process_pending(_pLexer, &_lexerData, _pAST, _token, _tmpExpression, _continue);
                }
            }

            _wasContinue = false;
        }
    }

    void ast_print(const __ast& _AST)
    {
        uint32_t _scope = 0;

        recursive(&(_AST.m_main), _scope);
    }

}}

Duckvil::Parser::__ast_ftable* duckvil_ast_init()
{
    static Duckvil::Parser::__ast_ftable _ftable = { 0 };

    _ftable.ast_generate = &Duckvil::Parser::ast_generate;
    _ftable.ast_print = &Duckvil::Parser::ast_print;

    return &_ftable;
}