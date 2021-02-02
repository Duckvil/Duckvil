#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Engine/ReflectionFlags.h"

#include "Parser/AST.h"

#include "Memory/FreeList.h"

#include <fstream>

#include "Serializer/Runtime/ISerializer.h"

#include "Engine/ISystem.h"

namespace Duckvil { namespace HotReloader {

    class RuntimeCompilerReflectionModule;

    void recursive(RuntimeCompilerReflectionModule* _pData, Parser::__ast_entity* _entity);

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule)
    class RuntimeCompilerReflectionModule
    {
    private:

    public:
        RuntimeCompilerReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_heap(_heap)
        {
            m_bGenerate = false;
            m_bHasGeneratedBody = false;
        }

        ~RuntimeCompilerReflectionModule()
        {

        }

        RuntimeReflection::__data* m_pRuntimeReflectionData;
        Memory::FreeList m_heap;

        bool m_bGenerate;
        bool m_bHasGeneratedBody;
        std::vector<std::string> m_aVars;
        std::vector<std::string> m_aChildSerializers;

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

                for(const std::string& _child : m_aChildSerializers)
                {
                    _file << _child << "::Serialize(_pSerializer); \\\n";
                }

                _file << "}";
            }
        }

        void Clear()
        {
            m_bGenerate = false;
            m_aVars.clear();
            m_aChildSerializers.clear();
        }
    };

    void recursive(RuntimeCompilerReflectionModule* _pData, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            Parser::__ast_entity_structure* _struct = (Parser::__ast_entity_structure*)_entity;
            Parser::__ast_entity* _current = _struct->m_pParentScope;
            std::vector<const char*> _namespaces;

            while(_current != nullptr && _current->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
            {
                if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
                {
                    Parser::__ast_entity_namespace* _entity = (Parser::__ast_entity_namespace*)_current;

                    _namespaces.push_back(_entity->m_sName.c_str());
                }
                else if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
                {
                    Parser::__ast_entity_structure* _entity = (Parser::__ast_entity_structure*)_current;

                    _namespaces.push_back(_entity->m_sName.c_str());
                }

                _current = _current->m_pParentScope;
            }

            std::reverse(_namespaces.begin(), _namespaces.end());

            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, _struct->m_sName.c_str(), _namespaces);

            if(_typeHandle.m_ID != -1)
            {
                const Memory::Vector<RuntimeReflection::__duckvil_resource_inheritance_t>& _inhs = RuntimeReflection::get_inheritances(_pData->m_pRuntimeReflectionData, _pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), _typeHandle);

                for(const auto& _inh : _inhs)
                {
                    const RuntimeReflection::__inheritance_t& _inhData = RuntimeReflection::get_inheritance(_pData->m_pRuntimeReflectionData, _typeHandle, _inh);
                    RuntimeReflection::__duckvil_resource_type_t _inhTypeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, _inhData.m_ullInheritanceTypeID);

                    if(_inhTypeHandle.m_ID != -1)
                    {
                        RuntimeReflection::__duckvil_resource_function_t _funcHandle = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(_pData->m_pRuntimeReflectionData, _inhTypeHandle, "Serialize");

                        if(_funcHandle.m_ID != -1)
                        {
                            const RuntimeReflection::__type_t& _inhType = RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, _inhTypeHandle);

                            _pData->m_aChildSerializers.push_back(_inhType.m_sTypeName);
                        }
                    }
                }
            }

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

            for(const auto& _meta : _struct->m_aMeta)
            {
                if(_meta.m_sKey.find("HotReloader::ReflectionFlags_Hot") != std::string::npos)
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

            // for(const Parser::__ast_inheritance& _inh : _struct->m_aInheritance)
            // {
            //     if(_inh.m_sName.find("HotObject") != std::string::npos)
            //     {
            //         _pData->m_bGenerate = true;

            //         if(_pData->m_bHasGeneratedBody)
            //         {
            //             Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

            //             _func->m_sReturnType = "void";
            //             _func->m_sName = "Serialize";
            //             _func->m_pParentScope = _struct;
            //             _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
            //             _func->m_flags = (Parser::__ast_flags)0;

            //             {
            //                 Parser::__ast_entity_argument _arg;
                            
            //                 _arg.m_sName = "_pSerializer";
            //                 _arg.m_sType = "Duckvil::RuntimeSerializer::ISerializer*";
            //                 _arg.m_pParentScope = _func;

            //                 _func->m_aArguments.push_back(_arg);
            //             }

            //             _struct->m_aScopes.push_back(_func);
            //         }
            //     }
            // }

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