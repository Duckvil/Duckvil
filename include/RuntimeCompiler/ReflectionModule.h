#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"

#include "Duckvil/ReflectionFlags.h"

#include "Parser/AST.h"

#include <fstream>

namespace Duckvil { namespace RuntimeCompiler {

    class RuntimeCompilerReflectionModule;

    void recursive(RuntimeCompilerReflectionModule* _pData, Parser::__ast_entity* _entity);

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class RuntimeCompilerReflectionModule
    {
    private:

    public:
        RuntimeCompilerReflectionModule()
        {
            m_bGenerate = false;
            m_bHasGeneratedBody = false;
        }

        ~RuntimeCompilerReflectionModule()
        {

        }

        bool m_bGenerate;
        bool m_bHasGeneratedBody;
        std::vector<std::string> m_aVars;

        void ProcessAST(Parser::__ast* _ast)
        {
            recursive(this, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _file)
        {
            if(m_bGenerate)
            {
                _file << " \\\n";
                _file << "public: \\\n";
                _file << "void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \\\n";
                _file << "{\\\n";

                for(const auto& _var : m_aVars)
                {
                    _file << "_pSerializer->SerializeProperty(\"" << _var << "\", " << _var << "); \\\n";
                }

                _file << "}";
            }
        }

        void Clear()
        {
            m_bGenerate = false;
            m_aVars.clear();
        }
    };

    void recursive(RuntimeCompilerReflectionModule* _pData, Parser::__ast_entity* _entity)
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

            for(const Parser::__ast_inheritance& _inh : _struct->m_aInheritance)
            {
                if(_inh.m_sName.find("HotObject") != std::string::npos)
                {
                    _pData->m_bGenerate = true;

                    if(_pData->m_bHasGeneratedBody)
                    {
                        Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                        _func->m_sReturnType = "void";
                        _func->m_sName = "Serialize";
                        _func->m_pParentScope = _struct;
                        _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                        _func->m_flags = (Parser::__ast_flags)0;

                        {
                            Parser::__ast_entity_argument _arg;
                            
                            _arg.m_sName = "_pSerializer";
                            _arg.m_sType = "Duckvil::RuntimeSerializer::ISerializer*";
                            _arg.m_pParentScope = _func;

                            _func->m_aArguments.push_back(_arg);
                        }

                        _struct->m_aScopes.push_back(_func);
                    }
                }
            }

            for(Parser::__ast_entity* _child : _struct->m_aScopes)
            {
                if(_child->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
                {
                    if(_pData->m_bGenerate)
                    {
                        Parser::__ast_entity_variable* _var = (Parser::__ast_entity_variable*)_child;
                        
                        _pData->m_aVars.push_back(_var->m_sName);
                    }
                }
            }
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ent);
        }
    }

}}