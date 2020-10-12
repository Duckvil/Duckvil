#include "RuntimeReflection/Generator.h"

#include <fstream>

namespace Duckvil { namespace RuntimeReflection {

    void recursive(const Parser::__ast_entity* _entity, std::ofstream& _file)
    {
        for(Parser::__ast_meta _meta : _entity->m_aMeta)
        {

        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_ent, _file);
        }
    }

    void generate(const char _sPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast)
    {
        std::ofstream _file(_sPath);

        _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD(0)\n";
        _file << "{\n";

        recursive(&_ast.m_main, _file);

        _file << "}\n";

        _file.close();
    }

}}

Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__generator_ftable* _ftable = (Duckvil::RuntimeReflection::__generator_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__generator_ftable), alignof(Duckvil::RuntimeReflection::__generator_ftable));

    _ftable->generate = Duckvil::RuntimeReflection::generate;

    return _ftable;
}