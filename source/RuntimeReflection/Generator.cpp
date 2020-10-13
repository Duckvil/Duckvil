#include "RuntimeReflection/Generator.h"

#include <fstream>

namespace Duckvil { namespace RuntimeReflection {

    void recursive(__generator_data* _pData, const Parser::__ast_entity* _entity, std::ofstream& _file)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
        {
            const Parser::__ast_entity_namespace* _casted = (const Parser::__ast_entity_namespace*)_entity;
            __generator_namespace _namespace = {};

            strcpy(_namespace.m_sName, _casted->m_sName.c_str());

            for(Parser::__ast_meta _meta : _entity->m_aMeta)
            {
                _namespace.m_aMeta.push_back(_meta);
            }

            _pData->m_aNamespaces.push_back(_namespace);
        }
        else if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            const Parser::__ast_entity_structure* _casted = (const Parser::__ast_entity_structure*)_entity;
            __generator_namespace _namespace = {};

            
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ent, _file);
        }
    }

    __generator_data* init(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    {
        __generator_data* _data = (__generator_data*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__generator_data), alignof(__generator_data));

        // _data->m_aNamespaces = Memory::Vector<const char*>(_pMemory, _pAllocator, 3);

        return _data;
    }

    void generate(__generator_data* _pData, const char _sPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast)
    {
        std::ofstream _file(_sPath);

        _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD(0)\n";
        _file << "{\n";

        recursive(_pData, &_ast.m_main, _file);

        _file << "}\n";

        _file.close();
    }

}}

Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__generator_ftable* _ftable = (Duckvil::RuntimeReflection::__generator_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__generator_ftable), alignof(Duckvil::RuntimeReflection::__generator_ftable));

    _ftable->init = &Duckvil::RuntimeReflection::init;
    _ftable->generate = &Duckvil::RuntimeReflection::generate;

    return _ftable;
}