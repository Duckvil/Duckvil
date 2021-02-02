#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/GeneratedMeta.h"

#include <cstring>
#include <algorithm>

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

    void generate_inheritance(const Parser::__ast_entity_structure* _pEntity, std::ofstream& _file)
    {
        for(const Parser::__ast_inheritance& _inheritance : _pEntity->m_aInheritance)
        {
            _file << "record_inheritance<" << _inheritance.m_sName << ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, ";

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
    }

    void generate_constructor(const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        const Parser::__ast_entity_constructor* _castedConstructor = (const Parser::__ast_entity_constructor*)_pEntity;
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type_class && _castedConstructor->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type_struct && _castedConstructor->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedConstructor->m_accessLevel != Parser::__ast_access_public)
        {
            _skip = true;
        }

        if(_skip || !_castedConstructor->m_aTemplates.empty())
        {
            return;
        }

        _file << "_constructor = record_constructor<" + _sNamespace + _pParentEntity->m_sName;

        if(!_castedConstructor->m_aArguments.empty())
        {
            _file << ", ";

            for(uint32_t i = 0; i < _castedConstructor->m_aArguments.size(); i++)
            {
                _file << _castedConstructor->m_aArguments[i].m_sType;

                if(i < _castedConstructor->m_aArguments.size() - 1)
                {
                    _file << ", ";
                }
            }
        }

        _file << ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);\n";

        for(const Parser::__ast_meta& _meta : _castedConstructor->m_aMeta)
        {
            _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, _constructor, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
        }
    }

    void generate_variable(const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        const Parser::__ast_entity_variable* _castedVariable = (const Parser::__ast_entity_variable*)_pEntity;
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type_class && _castedVariable->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type_struct && _castedVariable->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedVariable->m_accessLevel != Parser::__ast_access_public)
        {
            _skip = true;
        }

        if(_skip)
        {
            return;
        }

        std::string _additionalNamespaceTypedef;

        for(Parser::__ast_entity* _typedefEntity : _pParentEntity->m_aScopes)
        {
            if(_typedefEntity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_callback_typedef)
            {
                Parser::__ast_entity_callback_typedef* _castedTypedefEntity = (Parser::__ast_entity_callback_typedef*)_typedefEntity;

                if(_castedTypedefEntity->m_sName == _castedVariable->m_sType)
                {
                    _additionalNamespaceTypedef += _pParentEntity->m_sName + "::";

                    break;
                }
            }
            else if(_typedefEntity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_typedef)
            {
                Parser::__ast_entity_typedef* _castedTypedefEntity = (Parser::__ast_entity_typedef*)_typedefEntity;

                if(_castedTypedefEntity->m_sName == _castedVariable->m_sType)
                {
                    _additionalNamespaceTypedef += _pParentEntity->m_sName + "::";

                    break;
                }
            }
        }

        _file << "_property = record_property<" + _additionalNamespaceTypedef + _castedVariable->m_sType + ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(" + _sNamespace + _pParentEntity->m_sName + ", " + _castedVariable->m_sName + "), \"" + _castedVariable->m_sName + "\");\n";

        for(const Parser::__ast_meta& _meta : _castedVariable->m_aMeta)
        {
            _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, _property, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
        }
    }

    void generate_callback(const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        const Parser::__ast_entity_callback* _castedCallback = (const Parser::__ast_entity_callback*)_pEntity;
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type_class && _castedCallback->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type_struct && _castedCallback->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedCallback->m_accessLevel != Parser::__ast_access_public)
        {
            _skip = true;
        }

        if(_skip)
        {
            return;
        }

        _file << "_property = record_property<" << _castedCallback->m_sReturnType << "(" << _castedCallback->m_sMemberType << ")(";

        for(uint32_t i = 0; i < _castedCallback->m_aArguments.size(); i++)
        {
            _file << _castedCallback->m_aArguments[i].m_sType;

            if(i < _castedCallback->m_aArguments.size() - 1)
            {
                _file << ", ";
            }
        }

        _file << ")>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(" << _sNamespace + _pParentEntity->m_sName << ", " << _castedCallback->m_sName << "), \"" << _castedCallback->m_sName << "\");\n";
    }

    void generate_function(const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        const Parser::__ast_entity_function* _castedFunction = (const Parser::__ast_entity_function*)_pEntity;
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type_class && _castedFunction->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type_struct && _castedFunction->m_accessLevel == Parser::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedFunction->m_accessLevel != Parser::__ast_access_public)
        {
            _skip = true;
        }

        if(_skip || !_castedFunction->m_aTemplates.empty())
        {
            return;
        }

        _file << "record_function<";

        if(_castedFunction->m_flags & Parser::__ast_flags::__ast_flags_static)
        {
            _file << _castedFunction->m_sReturnType;
        }
        else
        {
            _file << _sNamespace + _pParentEntity->m_sName + ", ";
            _file << _castedFunction->m_sReturnType;
        }

        if(!_castedFunction->m_aArguments.empty())
        {
            _file << ", ";

            for(uint32_t i = 0; i < _castedFunction->m_aArguments.size(); i++)
            {
                _file << _castedFunction->m_aArguments[i].m_sType;

                if(i < _castedFunction->m_aArguments.size() - 1)
                {
                    _file << ", ";
                }
            }
        }

        _file << ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &" << _sNamespace + _pParentEntity->m_sName + "::" + _castedFunction->m_sName << ", \"" << _castedFunction->m_sName << "\");\n";
    }

    void generate_structure(__generator_data* _pData, const Parser::__ast_entity* _pEntity, std::ofstream& _file)
    {
        const Parser::__ast_entity_structure* _casted = (const Parser::__ast_entity_structure*)_pEntity;
        const Parser::__ast_entity_structure* _parent = _casted;
        std::string _additionalNamespace;

        if(!_casted->m_aTemplates.empty())
        {
            return;
        }

        std::string _combined = combine_namespace(_pData->m_aNamespaces);

        if(_combined.size() > 0)
        {
            if(!_pData->m_bWasNamespaces)
            {
                _pData->m_bWasNamespaces = true;

                _file << "using namespace " + _combined + ";\n";
            }

            _additionalNamespace += _combined + "::";
        }

        while(_parent->m_pParentScope->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            _additionalNamespace += ((Parser::__ast_entity_structure*)_parent->m_pParentScope)->m_sName + "::";

            _parent = (Parser::__ast_entity_structure*)_parent->m_pParentScope;
        }

        _file << "_type = record_type<" + _additionalNamespace + _casted->m_sName + ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, \"" + _casted->m_sName + "\");\n";
        _file << "_recordedTypes.push_back(_type);\n";

        Parser::__ast_entity* _namespace = _casted->m_pParentScope;
        std::stack<Parser::__ast_entity_namespace*> _namespaces;

        while(_namespace->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
        {
            if(_namespace->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
            {
                _namespaces.push((Parser::__ast_entity_namespace*)_namespace);
            }

            _namespace = _namespace->m_pParentScope;
        }

        while(!_namespaces.empty())
        {
            Parser::__ast_entity_namespace* _n = _namespaces.top();

            _file << "record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, \"" << _n->m_sName << "\");\n";

            _namespaces.pop();
        }

        _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, " << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID) << ", " << _pData->m_uiRecorderIndex << ");\n";

        for(const Parser::__ast_meta& _meta : _casted->m_aMeta)
        {
            if(!_meta.m_sValue.empty())
            {
                _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
            }
            else
            {
                _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, " + _meta.m_sKey + ", true);\n";
            }
        }

        generate_inheritance(_casted, _file);

        for(Parser::__ast_entity* _ent : _pEntity->m_aScopes)
        {
            switch(_ent->m_scopeType)
            {
            case Parser::__ast_entity_type::__ast_entity_type_variable:
                generate_variable(_ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_callback:
                generate_callback(_ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_constructor:
                generate_constructor(_ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_function:
                generate_function(_ent, _casted, _file, _additionalNamespace);
                break;
            default:
                break;
            }
        }

        // _file << "if(_types.Full()) _types.Resize(_types.Size() * 2);\n";
        // _file << "_types.Allocate(_type);\n";
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
            generate_structure(_pData, _entity, _file);

            // _pData->m_pCurrent = _casted;
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ent, _file);
        }

        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
        {
            _pData->m_aNamespaces.pop();
        }
    }

    __generator_data* init(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    {
        __generator_data* _data = (__generator_data*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__generator_data), alignof(__generator_data));

        // _data->m_aNamespaces = Memory::Vector<const char*>(_pMemory, _pAllocator, 3);

        _data->m_bWasNamespaces = false;

        return _data;
    }

    void generate(__generator_data* _pData, const char _sSourcePath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const char _sHeaderPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast, void (*_fnGenerate)(std::ofstream& _file, void* _pUserData), void* _pUserData)
    {
        {
            std::ofstream _file(_sHeaderPath);

            _fnGenerate(_file, _pUserData);

            _file << "\n";

            _file.close();
        }

        {
            std::ofstream _file(_sSourcePath);

            std::replace(_pData->m_sInclude, _pData->m_sInclude + DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX, '\\', '/');

            _file << "#include \"" << _pData->m_sInclude << "\"\n";
            _file << "#include \"RuntimeReflection/Recorder.h\"\n";
            _file << "#include \"RuntimeReflection/GeneratedMeta.h\"\n\n";

            _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD(" << _pData->m_uiRecorderIndex << ")\n";
            _file << "{\n";

            _file << "using namespace Duckvil::RuntimeReflection;\n";
            _file << "using namespace Duckvil;\n";
            _file << "DUCKVIL_RESOURCE(type_t) _type;\n";
            _file << "DUCKVIL_RESOURCE(property_t) _property;\n";
            _file << "DUCKVIL_RESOURCE(constructor_t) _constructor;\n";
            _file << "std::vector<" << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::__duckvil_resource_type_t) << "> _recordedTypes;\n";

            recursive(_pData, &_ast.m_main, _file);

            _file << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::__duckvil_resource_type_t) << "* _types = new " << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::__duckvil_resource_type_t) <<"[_recordedTypes.size()];\n";
            _file << "for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }\n";
            _file << "return duckvil_recorderd_types { _types, _recordedTypes.size(), \"" << _pData->m_sInclude << "\", " << _pData->m_uiRecorderIndex << " };\n";
            _file << "}\n";

            _file << "#ifdef DUCKVIL_RUNTIME_COMPILE\n";
            _file << "DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()\n";
            _file << "{\n";
            _file << "return " << _pData->m_uiRecorderIndex << ";\n";
            _file << "}\n";
            _file << "#endif\n";

            _file.close();
        }
    }

}}

Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__generator_ftable* _ftable = (Duckvil::RuntimeReflection::__generator_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__generator_ftable), alignof(Duckvil::RuntimeReflection::__generator_ftable));

    _ftable->init = &Duckvil::RuntimeReflection::init;
    _ftable->generate = &Duckvil::RuntimeReflection::generate;

    return _ftable;
}