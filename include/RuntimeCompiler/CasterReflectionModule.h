#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"

#include "Duckvil/ReflectionFlags.h"

#include "Parser/AST.h"

#include <fstream>

namespace Duckvil {

    class CasterReflectionModule;

    void recursive(CasterReflectionModule* _pData, Parser::__ast_entity* _entity);

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class CasterReflectionModule
    {
    private:

    public:
        CasterReflectionModule()
        {
            m_bHasGeneratedBody = false;
        }

        ~CasterReflectionModule()
        {

        }

        std::string m_sTypeName;
        bool m_bHasGeneratedBody;

        void ProcessAST(Parser::__ast* _ast)
        {
            recursive(this, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _file)
        {
            _file << " \\\n";
            _file << "public: \\\n";

            _file << "void* Cast(void* _pObject) { return static_cast<" << m_sTypeName << "*>(_pObject); }";
        }

        void Clear()
        {

        }
    };

    void recursive(CasterReflectionModule* _pData, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            Parser::__ast_entity_structure* _struct = (Parser::__ast_entity_structure*)_entity;

            for(Parser::__ast_entity* _ent : _struct->m_aScopes)
            {
                if(_ent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_define)
                {
                    Parser::__ast_entity_define* _define = (Parser::__ast_entity_define*)_ent;

                    if(_define->m_sName == "DUCKVIL_GENERATED_BODY")
                    {
                        _pData->m_bHasGeneratedBody = true;
                    }
                }
            }

            if(_pData->m_bHasGeneratedBody)
            {
                Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                _func->m_sReturnType = "void*";
                _func->m_sName = "Cast";
                _func->m_pParentScope = _struct;
                _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                _func->m_flags = (Parser::__ast_flags)0;

                {
                    Parser::__ast_entity_argument _arg;
                    
                    _arg.m_sName = "_pObject";
                    _arg.m_sType = "void*";
                    _arg.m_pParentScope = _func;

                    _func->m_aArguments.push_back(_arg);
                }

                _struct->m_aScopes.push_back(_func);
            }

            _pData->m_sTypeName = _struct->m_sName;
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ent);
        }
    }

}