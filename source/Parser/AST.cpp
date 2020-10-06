#include "Parser/AST.h"

namespace Duckvil { namespace Parser {

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
            else if(_pAST->m_pCurrentScope != nullptr && _pAST->m_pCurrentScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure && ((__ast_entity_structure*)_pAST->m_pCurrentScope)->m_sName == _token)
            {
                __ast_entity_constructor* _scope = new __ast_entity_constructor();

                _scope->m_pParentScope = _pAST->m_pCurrentScope;

                _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

                _pAST->m_pPendingScope = _scope;
            }
            else if(_token == "{")
            {
                _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
                _pAST->m_pPendingScope = nullptr;
            }
            else if(_token == "}")
            {
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
                printf("AAA\n");
            }
            else if(_token == "\n")
            {
                continue;
            }
            else if(_token != "")
            {
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

                    if(_lexerData.m_bSpace)
                    {
                        _tmp_expression += ' ';
                    }
                }
            }

            // else if(_token != "")
            // {
            //     if(_pAST->m_pPendingScope != nullptr)
            //     {
            //         if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_namespace)
            //         {
            //             ((__ast_entity_namespace*)_pAST->m_pPendingScope)->m_sName = _token;
            //         }
            //         else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
            //         {
            //             ((__ast_entity_structure*)_pAST->m_pPendingScope)->m_sName = _token;
            //         }
            //     }
            //     else
            //     {
            //         _tmp_expression += _token;
            //     }
            // }
            // else if(_lexerData.m_cPendingSymbol != 0)
            // {
            //     if(_lexerData.m_cPendingSymbol == '{' && _pAST->m_pPendingScope)
            //     {
            //         _pAST->m_pCurrentScope = _pAST->m_pPendingScope;
            //         _pAST->m_pPendingScope = nullptr;
            //     }
            //     else if(_lexerData.m_cPendingSymbol == '}')
            //     {
            //         _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
            //         _pAST->m_pPendingScope = nullptr;
            //     }
            //     else if(_lexerData.m_cPendingSymbol == '#')
            //     {
            //         while(_pLexer->next_token(&_lexerData, &_token))
            //         {
            //             if(_lexerData.m_cPendingSymbol == '\n')
            //             {
            //                 break;
            //             }
            //         }
            //     }
            //     else if(_lexerData.m_cPendingSymbol == '\n')
            //     {

            //     }
            //     else
            //     {
            //         _tmp_expression += _lexerData.m_cPendingSymbol;
            //     }
            // }
        }
    }

}}

Duckvil::Parser::__ast_ftable* duckvil_ast_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Parser::__ast_ftable* _table = (Duckvil::Parser::__ast_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Parser::__ast_ftable), alignof(Duckvil::Parser::__ast_ftable));

    _table->generate_ast = &Duckvil::Parser::generate_ast;

    return _table;
}