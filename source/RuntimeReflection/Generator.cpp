#include "RuntimeReflection/Generator.h"

#include <fstream>

namespace Duckvil { namespace RuntimeReflection {

    std::string combine_namespace(std::queue<__generator_namespace> _namespaces)
    {
        std::string _res;

        while(!_namespaces.empty())
        {
            _res += std::string(_namespaces.front().m_sName);

            if(_namespaces.size() > 1)
            {
                _res += "::";
            }

            _namespaces.pop();
        }

        return _res;
    }

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

            _pData->m_aNamespaces.push(_namespace);
        }
        else if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            const Parser::__ast_entity_structure* _casted = (const Parser::__ast_entity_structure*)_entity;

            if(!_pData->m_bWasNamespaces)
            {
                _pData->m_bWasNamespaces = true;

                _file << "using namespace " + combine_namespace(_pData->m_aNamespaces) + ";\n";
            }

            _file << "_type = record_type<" + _casted->m_sName + ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, \"" + _casted->m_sName + "\");\n";

            for(const Parser::__ast_inheritance& _inheritance : _casted->m_aInheritance)
            {
                _file << "record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<" + _inheritance.m_sName + ">(_pData), ";

                if(_inheritance.m_protection == Parser::__ast_access::__ast_access_public)
                {
                    _file << "__protection::__protection_public);\n";
                }
                else if(_inheritance.m_protection == Parser::__ast_access::__ast_access_protected)
                {
                    _file << "__protection::__protection_protected);\n";
                }
                else if(_inheritance.m_protection == Parser::__ast_access::__ast_access_private)
                {
                    _file << "__protection::__protection_private);\n";
                }
            }

            _pData->m_pCurrent = _casted;
        }
        else if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
        {
            if(_pData->m_pCurrent != nullptr &&_pData->m_pCurrent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
            {
                const Parser::__ast_entity_variable* _casted = (const Parser::__ast_entity_variable*)_entity;

                _file << "record_property<" + _casted->m_sType + ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(" + _pData->m_pCurrent->m_sName + ", " + _casted->m_sName + "), \"" + _casted->m_sName + "\");\n";
            }
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

        _data->m_bWasNamespaces = false;

        return _data;
    }

    void generate(__generator_data* _pData, const char _sPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast)
    {
        std::ofstream _file(_sPath);

        _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD(0)\n";
        _file << "{\n";

        _file << "using namespace Duckvil::RuntimeReflection;\n";
        _file << "DUCKVIL_RESOURCE(type_t) _type;\n";

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