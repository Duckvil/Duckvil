#pragma once

#include "Utils/Macro.h"

#include "Memory/Memory.h"

#include "Parser/Lexer.h"

namespace Duckvil { namespace Parser {

    enum __ast_entity_type
    {
        __ast_entity_type_function,
        __ast_entity_type_constructor,
        __ast_entity_type_callback,
        __ast_entity_type_namespace,
        __ast_entity_type_structure,
        __ast_entity_type_variable,
        __ast_entity_type_argument,
        __ast_entity_type_main
    };

    enum __ast_structure_type
    {
        __ast_structure_type_class,
        __ast_structure_type_struct
    };

    enum __ast_access
    {
        __ast_access_public,
        __ast_access_protected,
        __ast_access_private,
        __ast_access_not_specified
    };

    struct __ast_entity
    {
        __ast_entity(__ast_entity_type _scopeType = __ast_entity_type::__ast_entity_type_main) :
            m_scopeType(_scopeType)
        {

        }

        __ast_entity_type m_scopeType;
        std::vector<__ast_entity*> m_aScopes;
        __ast_entity* m_pParentScope;
    };

    struct __ast_entity_argument : public __ast_entity
    {
        __ast_entity_argument(__ast_entity_type _entityType = __ast_entity_type::__ast_entity_type_argument) :
            __ast_entity(_entityType)
        {

        }

        std::string m_sType;
        std::string m_sName;
    };

    struct __ast_entity_variable : public __ast_entity_argument
    {
        __ast_entity_variable(__ast_entity_type _entityType = __ast_entity_type::__ast_entity_type_variable) :
            __ast_entity_argument(_entityType)
        {

        }

        __ast_access m_accessLevel;
    };

    struct __ast_entity_structure : public __ast_entity
    {
        __ast_entity_structure(__ast_structure_type _structureType) :
            __ast_entity(__ast_entity_type::__ast_entity_type_structure),
            m_structureType(_structureType)
        {

        }

        std::string m_sName;
        __ast_structure_type m_structureType;
    };

    struct __ast_entity_namespace : public __ast_entity
    {
        __ast_entity_namespace() :
            __ast_entity(__ast_entity_type::__ast_entity_type_namespace)
        {

        }

        std::string m_sName;
    };

    struct __ast_entity_callable : public __ast_entity
    {
        __ast_entity_callable(__ast_entity_type _entityType) :
            __ast_entity(_entityType)
        {

        }

        std::vector<__ast_entity_argument> m_aArguments;
        __ast_access m_accessLevel;
    };

    struct __ast_entity_callback : public __ast_entity_callable
    {
        __ast_entity_callback() :
            __ast_entity_callable(__ast_entity_type::__ast_entity_type_callback)
        {

        }

        std::string m_sName;
        std::string m_sReturnType;
    };

    struct __ast_entity_constructor : public __ast_entity_callable
    {
        __ast_entity_constructor() :
            __ast_entity_callable(__ast_entity_type::__ast_entity_type_constructor)
        {

        }
    };

    struct __ast_entity_function : public __ast_entity_callable
    {
        __ast_entity_function() :
            __ast_entity_callable(__ast_entity_type::__ast_entity_type_function)
        {
            
        }

        std::string m_sName;
        std::string m_sReturnType;
    };

    struct __ast
    {
        __ast_entity m_main;
        __ast_entity* m_pCurrentScope;
        __ast_entity* m_pPendingScope;
        __ast_access m_currentAccess;
        std::vector<std::string> m_aUserDefines;
    };

    struct __ast_ftable
    {
        void (*generate_ast)(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData);
    };

    void generate_ast(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData);

}}

DUCKVIL_EXPORT Duckvil::Parser::__ast_ftable* duckvil_ast_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);