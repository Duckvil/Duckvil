#include "Parser/AST.h"

#include <iostream>
#include <stack>
#include <queue>

#include "cppast/cpp_enum.hpp"
#include "cppast/cpp_member_function.hpp"
#include "cppast/cpp_member_variable.hpp"
#include "cppast/cpp_namespace.hpp"
#include "cppast/cpp_variable.hpp"
#include "cppast/cpp_preprocessor.hpp"

#include "Utils/Utils.h"

#include "Parser/parser.h"

#include "cppast/visitor.hpp"
#include "cppast/cpp_template.hpp"

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
            _entity->m_flags = _flags;

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

                if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_variable)
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
                _entity->m_flags = _flags;

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
        _pAST->m_sCurrentDefineNeeded2 = nullptr;

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
            else if(_pAST->m_bPendingIfdef && _token == "elif")
            {
                _pLexer->next_token(&_lexerData, &_token);
                _pLexer->next_token(&_lexerData, &_pAST->m_sCurrentDefineNeeded);

                if(_pAST->m_sCurrentDefineNeeded2 != nullptr)
                {
                    __ast_preprocessor _p;

                    _p.m_token = _pAST->m_sCurrentDefineNeeded;
                    _p.m_type = __ast_preprocessor_type::__ast_preprocessor_type_elif;

                    _pAST->m_sCurrentDefineNeeded2->m_aChildren.push_back(_p);

                    // _pAST->m_sCurrentDefineNeeded2 = &_pAST->m_sCurrentDefineNeeded2->m_aChildren.back();
                }
                else
                {
                    _pAST->m_sCurrentDefineNeeded2 = new __ast_preprocessor();

                    _pAST->m_sCurrentDefineNeeded2->m_token = _pAST->m_sCurrentDefineNeeded;
                    _pAST->m_sCurrentDefineNeeded2->m_type = __ast_preprocessor_type::__ast_preprocessor_type_elif;
                }

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

    void add_scope(__ast* _pAST, __ast_entity* _pScope)
    {
        _pAST->m_pCurrentScope->m_aScopes.push_back(_pScope);

        _pScope->m_pParentScope = _pAST->m_pCurrentScope;

        _pAST->m_pCurrentScope = _pScope;
    }

    void process_attrs(const cppast::cpp_attribute_list& _attrs, __ast_entity* _pScope)
    {
        for(auto _attr : _attrs)
        {
            auto args = _attr.arguments().value();
            uint32_t _bracketsCount = 0;
            std::string _curr;
            __ast_meta _meta;

            for(auto arg : args)
            {
                if(arg.spelling == "(")
                {
                    _bracketsCount++;
                }
                else if(arg.spelling == ")")
                {
                    _bracketsCount--;
                }
                else if(arg.spelling == "=")
                {
                    _meta.m_sKey = _curr;

                    _curr.clear();

                    continue;
                }
                else if(_bracketsCount == 0 && arg.spelling == ",")
                {
                    if(_meta.m_sKey.empty())
                    {
                        _meta.m_sKey = _curr;

                        _curr.clear();
                    }

                    if(_curr.empty())
                    {
                        _curr = "true";
                    }

                    _meta.m_sValue = _curr;

                    _curr.clear();

                    _pScope->m_aMeta.push_back(_meta);

                    _meta = {};

                    continue;
                }

                _curr += arg.spelling;
            }

            if(_meta.m_sKey.empty() && _curr.empty())
            {
                continue;
            }
            else if(!_meta.m_sKey.empty() && _curr.empty())
            {
                _meta.m_sValue = "true";
            }
            else if(_meta.m_sKey.empty() && !_curr.empty())
            {
                _meta.m_sKey = _curr;

                _curr.clear();

                _meta.m_sValue = "true";
            }
            else if(!_meta.m_sKey.empty() && !_curr.empty())
            {
                _meta.m_sValue = _curr;

                _curr.clear();
            }

            /*if(_meta.m_sKey.empty())
            {
                continue;
            }

            if(!_curr.empty())
            {
                _meta.m_sValue = _curr;

                _curr.clear();
            }

            if(_meta.m_sValue.empty())
            {
                _meta.m_sValue = "true";
            }*/

            _pScope->m_aMeta.push_back(_meta);
        }
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_namespace* _pScope, const cppast::cpp_namespace& _entity, cppast::cpp_access_specifier_kind _access)
    {
        _pScope->m_sName = _entity.name();

        return _pScope;
    }

    std::string resolve_namespaces(const cppast::cpp_member_variable& _entity, const cppast::cpp_entity_index& _index)
    {
        std::string _namespaces = "";

        if(_entity.type().kind() == cppast::cpp_type_kind::user_defined_t)
        {
            const auto& _ut = static_cast<const cppast::cpp_user_defined_type&>(_entity.type());
            type_safe::optional_ref<const cppast::cpp_entity> _e;
            std::vector<std::string> _scopes;

            if (_ut.entity().id().size() > 1)
            {
                printf("AAA\n");
            }

            for(auto _id : _ut.entity().id())
            {
                auto a = _index.lookup(_id);

                if(a.has_value())
                {
                    _scopes.push_back(a.value().name());

                    _e = a.value().parent();

                    break;
                }
            }

            while(_e.has_value())
            {
                if(_e.value().kind() == cppast::cpp_entity_kind::file_t)
                {
                    break;
                }

                _scopes.push_back(_e.value().name());

                _e = _e.value().parent();
            }

            while(!_scopes.empty())
            {
                _namespaces += _scopes.back();

                _scopes.pop_back();

                if(!_scopes.empty())
                {
                    _namespaces += "::";
                }
            }
        }

        return _namespaces;
    }

    std::string resolve_namespaces(const cppast::cpp_variable& _entity, const cppast::cpp_entity_index& _index)
    {
        std::string _namespaces;

        if(_entity.type().kind() == cppast::cpp_type_kind::user_defined_t)
        {
            const auto& _ut = static_cast<const cppast::cpp_user_defined_type&>(_entity.type());
            type_safe::optional_ref<const cppast::cpp_entity> _e;
            std::vector<std::string> _scopes;

            for(auto _id : _ut.entity().id())
            {
                auto a = _index.lookup(_id);

                if(a.has_value())
                {
                    _e = a.value().parent();

                    break;
                }
            }

            while(_e.has_value())
            {
                if(_e.value().kind() == cppast::cpp_entity_kind::file_t)
                {
                    break;
                }

                _scopes.push_back(_e.value().name());

                _e = _e.value().parent();
            }

            while(!_scopes.empty())
            {
                _namespaces += _scopes.back() + "::";

                _scopes.pop_back();
            }
        }

        return _namespaces;
    }

    std::string resolve_namespaces(const cppast::cpp_type& _type, const cppast::cpp_entity_index& _index)
    {
        std::string _namespaces;

        const auto& _ut = static_cast<const cppast::cpp_user_defined_type&>(_type);
        type_safe::optional_ref<const cppast::cpp_entity> _e;
        std::vector<std::string> _scopes;

        for (auto _id : _ut.entity().id())
        {
            auto a = _index.lookup(_id);

            if (a.has_value())
            {
                _e = a.value().parent();

                break;
            }
        }

        while (_e.has_value())
        {
            if (_e.value().kind() == cppast::cpp_entity_kind::file_t)
            {
                break;
            }

            _scopes.push_back(_e.value().name());

            _e = _e.value().parent();
        }

        while (!_scopes.empty())
        {
            _namespaces += _scopes.back() + "::";

            _scopes.pop_back();
        }

        return _namespaces;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_structure* _pScope, const cppast::cpp_class& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(_entity.name() == "__duckvil_generated_body__")
        {
            delete _pScope;

            auto _scope = new __ast_entity_define();

            //_pAST->m_pPendingScope = _scope;

            _scope->m_sName = "DUCKVIL_GENERATED_BODY";

            static_cast<__ast_entity_structure*>(_pAST->m_pCurrentScope)->m_uiGeneratedBodyMacroLine = _entity.get_line();

            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

            return nullptr;
        }
        else if(_entity.name().find("__duckvil_csharp_function__") != std::string::npos)
        {
            delete _pScope;

            auto _scope = new __ast_entity_define();

            //_pAST->m_pPendingScope = _scope;

            _scope->m_sName = "DUCKVIL_CS_FUNCTION";

            auto _attrs = _entity.attributes();

            for (auto _attr : _attrs)
            {
                auto args = _attr.arguments().value();
                uint32_t _bracketsCount = 0;
                uint32_t _sharpBracketsCount = 0;
                std::string _curr;

                for (auto arg : args)
                {
                    if (arg.spelling == "(")
                    {
                        _bracketsCount++;
                    }
                    else if (arg.spelling == ")")
                    {
                        _bracketsCount--;
                    }
                    if (arg.spelling == "<")
                    {
                        _sharpBracketsCount++;
                    }
                    else if (arg.spelling == ">")
                    {
                        _sharpBracketsCount--;
                    }
                    else if (_bracketsCount == 0 && _sharpBracketsCount == 0 && arg.spelling == ",")
                    {
                        _scope->m_aArguments.push_back(_curr);

                        _curr.clear();

                        continue;
                    }

                    _curr += arg.spelling;
                }

                if (!_curr.empty())
                {
                    _curr = std::string(++_curr.begin(), --_curr.end()) ;

                    _scope->m_aArguments.push_back(_curr);

                    _curr.clear();
                }
            }


        	//_scope->m_aArguments

            //static_cast<__ast_entity_structure*>(_pAST->m_pCurrentScope)->m_uiGeneratedBodyMacroLine = _entity.get_line();

            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

            return nullptr;
        }

    	if(_pAST->m_pPendingScope == nullptr && _entity.name().starts_with("__duckvil_class_"))
        {
            delete _pScope;
            
            _pAST->m_pPendingScope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);
            
            process_attrs(_entity.attributes(), _pAST->m_pPendingScope);

            return nullptr;
        }

        if(!cppast::is_definition(_entity))
        {
            delete _pScope;

            return nullptr;
        }

        if(_pAST->m_pPendingScope != nullptr)
        {
            delete _pScope;

            _pScope = static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        _pScope->m_sName = _entity.name();

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _base : _entity.bases())
        {
            __ast_inheritance _inh;

            _inh.m_sName = _base.name();
            
            switch(_base.access_specifier())
            {
            case cppast::cpp_public:
                _inh.m_protection = __ast_access::__ast_access_public;
                break;
            case cppast::cpp_protected:
                _inh.m_protection = __ast_access::__ast_access_protected;
                break;
            case cppast::cpp_private:
                _inh.m_protection = __ast_access::__ast_access_private;
                break;
            default:
                break;
            }

            _pScope->m_aInheritance.push_back(_inh);
        }

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_function* _pScope, const cppast::cpp_member_function& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        if(_pAST->m_pPendingScope != nullptr)
        {
            delete _pScope;

            _pScope = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        _pScope->m_sName = _entity.name();
        _pScope->m_sReturnType = to_string(_entity.return_type());

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_function* _pScope, const cppast::cpp_function& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope != nullptr)
        {
            delete _pScope;

            _pScope = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        _pScope->m_sName = _entity.name();
        _pScope->m_sReturnType = to_string(_entity.return_type());
        _pScope->m_flags = __ast_flags::__ast_flags_static;

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());
            // TODO:
            //_eArg.m_sValue = _arg.default_value().has_value() ? _arg.default_value() : "";

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_variable* _pScope, const cppast::cpp_member_variable& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        _pScope->m_sName = _entity.name();
        _pScope->m_flags = !_entity.is_mutable() ? static_cast<__ast_flags>(static_cast<uint8_t>(_pScope->m_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_const)) : _pScope->m_flags;

        std::string _namespaces = resolve_namespaces(_entity, _index);

        if(_namespaces.empty())
        {
            _namespaces = to_string(_entity.type());
        }

        _pScope->m_sType = _namespaces/* + to_string(_entity.type())*/;

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_variable* _pScope, const cppast::cpp_variable& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        _pScope->m_sName = _entity.name();
        _pScope->m_flags = !_entity.is_constexpr() ? static_cast<__ast_flags>(static_cast<uint8_t>(_pScope->m_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_const)) : _pScope->m_flags;
        
        std::string _namespaces = resolve_namespaces(_entity, _index);

        if(_namespaces.empty())
        {
            _namespaces = to_string(_entity.type());
        }

        _pScope->m_sType = _namespaces/* + to_string(_entity.type())*/;

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_enum* _pScope, const cppast::cpp_enum& _entity, cppast::cpp_access_specifier_kind _access)
    {
        _pScope->m_sName = _entity.name();

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_enum_value& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(!_pAST->m_pCurrentScope || _pAST->m_pCurrentScope->m_scopeType != __ast_entity_type::__ast_entity_type_enum)
        {
            throw new std::exception();
        }

        auto _scope = static_cast<__ast_entity_enum*>(_pAST->m_pCurrentScope);

        _scope->m_aElements.push_back(_entity.name());

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_template_specialization& _entity, cppast::cpp_entity_kind _kind, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope == nullptr)
        {
            if(_kind == cppast::cpp_entity_kind::class_template_t || _kind == cppast::cpp_entity_kind::class_template_specialization_t)
            {
                _pAST->m_pPendingScope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);
            }
            else if(_kind == cppast::cpp_entity_kind::function_template_t || _kind == cppast::cpp_entity_kind::function_template_specialization_t)
            {
                _pAST->m_pPendingScope = new __ast_entity_function();
            }
            else
            {
                throw std::exception();
            }
        }

        if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
        {
            static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(__ast_template());
        }
        else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        {
            static_cast<__ast_entity_function*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(__ast_template());
        }

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_template& _entity, cppast::cpp_entity_kind _kind, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope == nullptr)
        {
            if(_kind == cppast::cpp_entity_kind::class_template_t)
            {
                _pAST->m_pPendingScope = new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class);
            }
            else if(_kind == cppast::cpp_entity_kind::function_template_t)
            {
                _pAST->m_pPendingScope = new __ast_entity_function();
            }
            else
            {
                throw std::exception();
            }
        }

        auto _templ = __ast_template();

        if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
        {
            static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(_templ);
        }
        else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        {
            static_cast<__ast_entity_function*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(_templ);
        }

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_constructor* _pScope, const cppast::cpp_constructor& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        {
            auto _f = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);

            _pScope->m_aTemplates.insert(_pScope->m_aTemplates.begin(), _f->m_aTemplates.begin(), _f->m_aTemplates.end());

            delete _pAST->m_pPendingScope;

            _pAST->m_pPendingScope = nullptr;
        }

        if(_entity.body_kind() == cppast::cpp_function_deleted)
        {
            return nullptr;
        }

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());
            // TODO:
            //_eArg.m_sValue = _arg.default_value().has_value() ? _arg.default_value() : "";

            process_attrs(_arg.attributes(), &_eArg);

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_destructor* _pScope, const cppast::cpp_destructor& _entity, cppast::cpp_access_specifier_kind _access)
    {
        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());
            // TODO:
            //_eArg.m_sValue = _arg.default_value().has_value() ? _arg.default_value() : "";

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    void ast_generate_cppast(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        const auto& _config = *static_cast<Parser::compile_config*>(_lexerData.m_pConfig);

        _pAST->m_pCurrentScope = &_pAST->m_main;
        _pAST->m_pPendingScope = nullptr;
        _pAST->m_currentAccess = __ast_access::__ast_access_not_specified;
        _pAST->m_sCurrentDefineNeeded2 = nullptr;

        cppast::cpp_entity_index _index;

        custom<cppast::libclang_parser> _parser(type_safe::ref(_index));

        try
        {
            _parser.parse((_pAST->m_sPath / _pAST->m_sFile).string(), _config);
        }
        catch(const std::exception& ex)
        {
            printf("Exception while parsing: %s\n", ex.what());

            return;
        }

        for(const auto& _file : _parser.files())
        {
            cppast::visit(_file, [&](const cppast::cpp_entity& e, cppast::visitor_info info)
                {
                    if(info.event == cppast::visitor_info::container_entity_exit)
                    {
                        if(e.kind() == cppast::cpp_entity_kind::class_t && !cppast::is_definition(e))
                        {
                            return;
                        }

                        if((e.kind() != cppast::cpp_entity_kind::language_linkage_t &&
                            e.kind() != cppast::cpp_entity_kind::function_template_t &&
                            e.kind() != cppast::cpp_entity_kind::function_template_specialization_t &&
                            _pAST->m_pPendingScope == nullptr &&
                            e.kind() != cppast::cpp_entity_kind::file_t &&
                            e.kind() != cppast::cpp_entity_kind::alias_template_t &&
                            e.kind() != cppast::cpp_entity_kind::class_template_t &&
                            e.kind() != cppast::cpp_entity_kind::class_template_specialization_t &&
                            e.kind() != cppast::cpp_entity_kind::constructor_t &&
                            e.kind() != cppast::cpp_entity_kind::destructor_t))
                        {
                            _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
                        }

                        return;
                    }

                    __ast_entity* _scope = nullptr;

                    switch(e.kind())
                    {
                    case cppast::cpp_entity_kind::class_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_structure(__ast_structure_type::__ast_structure_type_class), reinterpret_cast<const cppast::cpp_class&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::member_function_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_function(), reinterpret_cast<const cppast::cpp_member_function&>(e), info.access, _index);
                    } break;
                    case cppast::cpp_entity_kind::function_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_function(), reinterpret_cast<const cppast::cpp_function&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::namespace_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_namespace(), reinterpret_cast<const cppast::cpp_namespace&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::member_variable_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_variable(), reinterpret_cast<const cppast::cpp_member_variable&>(e), info.access, _index);
                    } break;
                    case cppast::cpp_entity_kind::enum_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_enum(), reinterpret_cast<const cppast::cpp_enum&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::enum_value_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_enum_value&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::class_template_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::class_template_specialization_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template_specialization&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::function_template_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::function_template_specialization_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template_specialization&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::constructor_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_constructor(), reinterpret_cast<const cppast::cpp_constructor&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::destructor_t:
                    {
                        _scope = process_entity(_pAST, new __ast_entity_destructor(), reinterpret_cast<const cppast::cpp_destructor&>(e), info.access);
                    } break;
                    default:
                        printf("%s\n", e.name().c_str());
                        return;
                    }

                    if(_scope == nullptr)
                    {
                        return;
                    }

            		if(info.event == cppast::visitor_info::container_entity_enter)
                    {
                        add_scope(_pAST, _scope);
                    }
                    else if(info.event == cppast::visitor_info::leaf_entity)
                    {
                        add_scope(_pAST, _scope);

                        _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
                    }
                }
            );
        }
    }

}}

Duckvil::Parser::__ast_ftable* duckvil_ast_init()
{
    static Duckvil::Parser::__ast_ftable _ftable = { 0 };

    _ftable.ast_generate = &Duckvil::Parser::ast_generate_cppast;
    _ftable.ast_print = &Duckvil::Parser::ast_print;

    return &_ftable;
}