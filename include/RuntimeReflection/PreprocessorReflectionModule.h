#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "Engine/ReflectionFlags.h"

#include "Parser/AST.h"

#include "Memory/FreeList.h"

#include <fstream>

namespace Duckvil {

    class PreprocessorReflectionModule;

    static void recursive2(PreprocessorReflectionModule* _pData, Parser::__ast_entity* _entity);

    static void replaceAll(std::string& str, const std::string& from, const std::string& to)
    {
        if(from.empty())
            return;

        size_t start_pos = 0;

        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class PreprocessorReflectionModule
    {
    public:
        enum Flag
        {
            Flag_Ifdef
        };

    private:

    public:
        PreprocessorReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_pRuntimeReflection(_pReflection),
            m_heap(_heap)
        {

        }

        ~PreprocessorReflectionModule()
        {

        }

        RuntimeReflection::__data* m_pRuntimeReflectionData;
        RuntimeReflection::__ftable* m_pRuntimeReflection;
        Memory::FreeList m_heap;

        void ProcessAST(Parser::__ast* _ast)
        {
            recursive2(this, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _file)
        {

        }

        void Clear()
        {

        }
    };

    static void recursive2(PreprocessorReflectionModule* _pData, Parser::__ast_entity* _entity)
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
            const auto& _properties = _pData->m_pRuntimeReflection->m_fnGetProperties(_pData->m_pRuntimeReflectionData, _pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), _typeHandle);

            for(const auto& _property : _properties)
            {
                const auto& _metas = _pData->m_pRuntimeReflection->m_fnGetPropertyMetas(_pData->m_heap, _pData->m_pRuntimeReflectionData, _typeHandle, _property);

                for(const auto& _meta : _metas)
                {
                    // _pData->m_pRuntimeReflection->m_fnGetPropertyMetaVariant(_pData->m_pRuntimeReflectionData, _typeHandle, _property, )
                    const auto& _metaVariant = RuntimeReflection::get_meta(_typeHandle, PreprocessorReflectionModule::Flag_Ifdef);

                    if(_metaVariant.m_ullTypeID != -1)
                    {
                        printf("aaaa\n");
                    }
                }
            }
        }

        // if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
        // {
        //     Parser::__ast_entity_variable* _variable = (Parser::__ast_entity_variable*)_entity;
        //     Parser::__ast_entity* _current = _variable->m_pParentScope;
        //     std::vector<const char*> _namespaces;

        //     while(_current != nullptr && _current->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
        //     {
        //         if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
        //         {
        //             Parser::__ast_entity_namespace* _entity = (Parser::__ast_entity_namespace*)_current;

        //             _namespaces.push_back(_entity->m_sName.c_str());
        //         }
        //         else if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        //         {
        //             Parser::__ast_entity_structure* _entity = (Parser::__ast_entity_structure*)_current;

        //             _namespaces.push_back(_entity->m_sName.c_str());
        //         }

        //         _current = _current->m_pParentScope;
        //     }

        //     std::reverse(_namespaces.begin(), _namespaces.end());

        //     std::string _variableName = _variable->m_sType;

        //     replaceAll(_variableName, "::", " ");

        //     std::vector<std::string> _splitted = Utils::split(_variableName, ' ');

        //     // for(uint32_t i = 0; i < _splitted.size() - 1; ++i)
        //     // {
        //     //     _namespaces.push_back(_splitted[i].c_str());
        //     // }

        //     RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, _variableName.c_str(), _namespaces);

        //     if(_typeHandle.m_ID != -1)
        //     {
        //         RuntimeReflection::__duckvil_resource_property_t _propertyHandle = _pData->m_pRuntimeReflection->m_fnGetPropertyHandleByName(_pData->m_pRuntimeReflectionData, _typeHandle, _variable->m_sName.c_str(), _variable->m_sName.length());

        //         const auto& _metas = _pData->m_pRuntimeReflection->m_fnGetPropertyMetas(_pData->m_heap, _pData->m_pRuntimeReflectionData, _typeHandle, _propertyHandle);

        //         for(const auto& _meta : _metas)
        //         {
        //             printf("AAAA\n");
        //         }
        //     }

        //     // for(const auto& _meta : _variable->m_aMeta)
        //     // {
        //     //     if(_meta.m_sKey.)
        //     // }
        // }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive2(_pData, _ent);
        }
    }

}