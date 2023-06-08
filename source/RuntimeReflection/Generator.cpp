#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/GeneratedMeta.h"

#include <cstring>
#include <algorithm>

namespace Duckvil { namespace RuntimeReflection {

    // Returns combined namespace from deque
    // Result example: A::B::C
    std::string combine_namespace(std::deque<__generator_namespace> _namespaces)
    {
        std::string _res;

        while(!_namespaces.empty())
        {
            _res += std::string(_namespaces.front().m_sName);

            if(_namespaces.size() > 1)
            {
                _res += "::";
            }

            _namespaces.pop_front();
        }

        return _res;
    }

    bool has_ignore_flag(const Parser::__ast_entity* _pEntity)
    {
        for (auto _meta : _pEntity->m_aMeta)
        {
            if (_meta.m_sKey == "Ignore" && _meta.m_sValue == "true")
            {
                return true;
            }
        }

        return false;
    }

    void generate_inheritance(const Parser::__ast& _ast, const Parser::__ast_entity_structure* _pEntity, std::ofstream& _file)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        for(const Parser::__ast_inheritance& _inheritance : _pEntity->m_aInheritance)
        {
            _file << "record_inheritance<" << _inheritance.m_sName << ">(_data, _type, ";

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

    void generate_constructor(const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_constructor* _castedConstructor = static_cast<const Parser::__ast_entity_constructor*>(_pEntity);
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_class && _castedConstructor->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_struct && _castedConstructor->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedConstructor->m_accessLevel != Parser::__ast_access::__ast_access_public)
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

            for(uint32_t i = 0; i < _castedConstructor->m_aArguments.size(); ++i)
            {
                _file << _castedConstructor->m_aArguments[i].m_sType;

                if(i < _castedConstructor->m_aArguments.size() - 1)
                {
                    _file << ", ";
                }
            }
        }

        _file << ">(_data, _type);\n";

        for(const Parser::__ast_meta& _meta : _castedConstructor->m_aMeta)
        {
            if(!_meta.m_sValue.empty())
            {
                _file << "record_meta(_data, _type, _constructor, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
            }
            else
            {
                _file << "record_meta(_data, _type, _constructor, " + _meta.m_sKey + ", true);\n";
            }
        }

        uint32_t _index = 0;

        for(const Parser::__ast_entity_argument& _argument : _castedConstructor->m_aArguments)
        {
            for(const Parser::__ast_meta& _meta : _argument.m_aMeta)
            {
                if(!_meta.m_sValue.empty())
                {
                    _file << "record_meta(_data, _type, _constructor, " << _index << ", " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
                }
                else
                {
                    _file << "record_meta(_data, _type, _constructor, " << _index << ", " + _meta.m_sKey + ", true);\n";
                }
            }

            ++_index;
        }
    }

    void generate_destructor(const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_destructor* _castedDestructor = static_cast<const Parser::__ast_entity_destructor*>(_pEntity);

        _file << "_destructor = record_destructor<" + _sNamespace + _pParentEntity->m_sName;
        _file << ">(_data, _type);\n";

        for(const Parser::__ast_meta& _meta : _castedDestructor->m_aMeta)
        {
            _file << "record_meta(_data, _type, _destructor, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
        }
    }

    void generate_variable(const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_variable* _castedVariable = static_cast<const Parser::__ast_entity_variable*>(_pEntity);
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_class && _castedVariable->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_struct && _castedVariable->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedVariable->m_accessLevel != Parser::__ast_access::__ast_access_public)
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
                Parser::__ast_entity_callback_typedef* _castedTypedefEntity = static_cast<Parser::__ast_entity_callback_typedef*>(_typedefEntity);

                if(_castedTypedefEntity->m_sName == _castedVariable->m_sType)
                {
                    _additionalNamespaceTypedef += _pParentEntity->m_sName + "::";

                    break;
                }
            }
            else if(_typedefEntity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_typedef)
            {
                Parser::__ast_entity_typedef* _castedTypedefEntity = static_cast<Parser::__ast_entity_typedef*>(_typedefEntity);

                if(_castedTypedefEntity->m_sName == _castedVariable->m_sType)
                {
                    _additionalNamespaceTypedef += _pParentEntity->m_sName + "::";

                    break;
                }
            }
        }

        for(const auto& _define : _castedVariable->m_aNeededDefines)
        {
            _file << "#ifdef " << _define << "\n";
        }

        if(_castedVariable->m_flags & Parser::__ast_flags::__ast_flags_static)
        {
            _file << "_property = record_property<" << _castedVariable->m_sType << ">(_data, _type, &" << _sNamespace + _pParentEntity->m_sName + "::" + _castedVariable->m_sName;
        }
        else
        {
            _file << "_property = record_property<" + /*_additionalNamespaceTypedef + */_castedVariable->m_sType + ">(_data, _type, ";

            _file << "offsetof(" + _sNamespace + _pParentEntity->m_sName + ", " + _castedVariable->m_sName + ")";
        }

        _file << ", \"" + _castedVariable->m_sName + "\");\n";

        for(const Parser::__ast_meta& _meta : _castedVariable->m_aMeta)
        {
            _file << "record_meta(_data, _type, _property, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
        }

        for(const auto& _define : _castedVariable->m_aNeededDefines)
        {
            _file << "#endif\n";
        }
    }

    void generate_callback(const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_callback* _castedCallback = static_cast<const Parser::__ast_entity_callback*>(_pEntity);
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_class && _castedCallback->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_struct && _castedCallback->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedCallback->m_accessLevel != Parser::__ast_access::__ast_access_public)
        {
            _skip = true;
        }

        if(_skip)
        {
            return;
        }

        for(const auto& _define : _castedCallback->m_aNeededDefines)
        {
            _file << "#ifdef " << _define << "\n";
        }

        _file << "_property = record_property<" << _castedCallback->m_sReturnType << "(" << _castedCallback->m_sMemberType << ")(";

        for(uint32_t i = 0; i < _castedCallback->m_aArguments.size(); ++i)
        {
            _file << _castedCallback->m_aArguments[i].m_sType;

            if(i < _castedCallback->m_aArguments.size() - 1)
            {
                _file << ", ";
            }
        }

        _file << ")>(_data, _type, offsetof(" << _sNamespace + _pParentEntity->m_sName << ", " << _castedCallback->m_sName << "), \"" << _castedCallback->m_sName << "\");\n";

        for(const auto& _define : _castedCallback->m_aNeededDefines)
        {
            _file << "#endif\n";
        }
    }

    void generate_function(const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_function* _castedFunction = static_cast<const Parser::__ast_entity_function*>(_pEntity);
        bool _skip = false;

        if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_class && _castedFunction->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = true;
        }
        else if (_castedFunction->m_flags & Parser::__ast_flags::__ast_flags_virtual)
        {
            _skip = true;
        }
        else if(_pParentEntity->m_structureType == Parser::__ast_structure_type::__ast_structure_type_struct && _castedFunction->m_accessLevel == Parser::__ast_access::__ast_access_not_specified)
        {
            _skip = false;
        }
        else if(_castedFunction->m_accessLevel != Parser::__ast_access::__ast_access_public)
        {
            _skip = true;
        }
        else if(_castedFunction->m_flags & Parser::__ast_flags::__ast_flags_operator)
        {
            _skip = true;
        }

        if(_skip || !_castedFunction->m_aTemplates.empty())
        {
            return;
        }

        for(const auto& _define : _castedFunction->m_aNeededDefines)
        {
            _file << "#ifdef " << _define << "\n";
        }

        _file << "_function = record_function<";

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

            for(uint32_t i = 0; i < _castedFunction->m_aArguments.size(); ++i)
            {
                _file << _castedFunction->m_aArguments[i].m_sType;

                if(i < _castedFunction->m_aArguments.size() - 1)
                {
                    _file << ", ";
                }
            }
        }

        _file << ">(_data, _type, &" << _sNamespace + _pParentEntity->m_sName + "::" + _castedFunction->m_sName << ", \"" << _castedFunction->m_sName << "\");\n";

        for(const Parser::__ast_meta& _meta : _castedFunction->m_aMeta)
        {
            _file << "record_meta(_data, _type, _function, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
        }

        uint32_t _index = 0;

        for(const Parser::__ast_entity_argument& _argument : _castedFunction->m_aArguments)
        {
            for(const Parser::__ast_meta& _meta : _argument.m_aMeta)
            {
                _file << "record_meta(_data, _type, _function, " << _index << ", " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
            }

            ++_index;
        }

        for(const auto& _define : _castedFunction->m_aNeededDefines)
        {
            _file << "#endif\n";
        }
    }

    void generate_enum(const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, const Parser::__ast_entity_structure* _pParentEntity, std::ofstream& _file, const std::string& _sNamespace)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_enum* _castedEnum = static_cast<const Parser::__ast_entity_enum*>(_pEntity);

        for(const auto& _define : _castedEnum->m_aNeededDefines)
        {
            _file << "#ifdef " << _define << "\n";
        }

        _file << "_enum = record_enum<" << _sNamespace + _pParentEntity->m_sName << "::" << _castedEnum->m_sName << ">(_data, _type, \"" << _castedEnum->m_sName << "\");\n";

        for(const Parser::__ast_meta& _meta : _castedEnum->m_aMeta)
        {
            _file << "record_meta(_data, _type, _enum, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
        }

        for(const auto& _element : _castedEnum->m_aElements)
        {
            _file << "_enumElement = record_enum_element(_data, _type, _enum, " << _sNamespace + _pParentEntity->m_sName << "::" << _castedEnum->m_sName << "::" << _element << ", \"" << _element << "\");\n";
        }
    }

    void generate_structure(__generator_data* _pData, const Parser::__ast& _ast, const Parser::__ast_entity* _pEntity, std::ofstream& _file)
    {
        if (has_ignore_flag(_pEntity))
        {
            return;
        }

        const Parser::__ast_entity_structure* _casted = static_cast<const Parser::__ast_entity_structure*>(_pEntity);

        if (_casted->m_bIsForwardDeclaration)
        {
            return;
        }

        const Parser::__ast_entity_structure* _parent = _casted;
        std::string _additionalNamespace;

        if(!_casted->m_aTemplates.empty() || _casted->m_accessLevel == Parser::__ast_access::__ast_access_private)
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
            _additionalNamespace += (static_cast<Parser::__ast_entity_structure*>(_parent->m_pParentScope))->m_sName + "::";

            _parent = static_cast<Parser::__ast_entity_structure*>(_parent->m_pParentScope);
        }

        for(const auto& _define : _casted->m_aNeededDefines)
        {
            _file << "#ifdef " << _define << "\n";
        }

        _file << "_type = record_type<" + _additionalNamespace + _casted->m_sName + ">(_data, \"" + _casted->m_sName + "\");\n";
        _file << "_recordedTypes.push_back(_type);\n";

        Parser::__ast_entity* _namespace = _casted->m_pParentScope;
        std::stack<Parser::__ast_entity_namespace*> _namespaces;

        while(_namespace->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
        {
            if(_namespace->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace || _namespace->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
            {
                _namespaces.push(static_cast<Parser::__ast_entity_namespace*>(_namespace));
            }

            _namespace = _namespace->m_pParentScope;
        }

        while(!_namespaces.empty())
        {
            Parser::__ast_entity_namespace* _n = _namespaces.top();

            _file << "record_namespace(_data, _type, \"" << _n->m_sName << "\");\n";

            _namespaces.pop();
        }

        _file << "record_meta(_data, _type, " << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID) << ", " << _pData->m_uiRecorderIndex << ");\n";

        for(const Parser::__ast_meta& _meta : _casted->m_aMeta)
        {
            if(!_meta.m_sValue.empty())
            {
                _file << "record_meta(_data, _type, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
            }
            else
            {
                _file << "record_meta(_data, _type, " + _meta.m_sKey + ", true);\n";
            }
        }

        generate_inheritance(_ast, _casted, _file);

        for(const auto& _define : _casted->m_aNeededDefines)
        {
            _file << "#endif\n";
        }

        for(Parser::__ast_entity* _ent : _pEntity->m_aScopes)
        {
            switch(_ent->m_scopeType)
            {
            case Parser::__ast_entity_type::__ast_entity_type_variable:
                generate_variable(_ast, _ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_callback:
                generate_callback(_ast, _ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_constructor:
                generate_constructor(_ast, _ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_destructor:
                generate_destructor(_ast, _ent, _casted, _file, _additionalNamespace);
                break;
            case Parser::__ast_entity_type::__ast_entity_type_function:
                generate_function(_ast, _ent, _casted, _file, _additionalNamespace);
                break;
            default:
                break;
            }
        }

        // _file << "if(_types.Full()) _types.Resize(_types.Size() * 2);\n";
        // _file << "_types.Allocate(_type);\n";
    }

    void recursive(__generator_data* _pData, const Parser::__ast& _ast, const Parser::__ast_entity* _entity, std::ofstream& _file)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
        {
            const Parser::__ast_entity_namespace* _casted = static_cast<const Parser::__ast_entity_namespace*>(_entity);
            __generator_namespace _namespace = {};

            snprintf(_namespace.m_sName, 32, "%s", _casted->m_sName.c_str());

            for(Parser::__ast_meta _meta : _entity->m_aMeta)
            {
                _namespace.m_aMeta.push_back(_meta);
            }

            _pData->m_aNamespaces.push_back(_namespace);

            _file << "{\n";
            _file << "_namespaces.push_back(\"" << _casted->m_sName << "\");\n";
            _file << "_ntype = record_type(_data, _namespaces);\n";
            _file << "_recordedNTypes.push_back(_ntype);\n";
        }
        else if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            const Parser::__ast_entity_structure* _c = static_cast<const Parser::__ast_entity_structure*>(_entity);

            // _file << "{\n";
            _file << "_namespaces.push_back(\"" << _c->m_sName << "\");\n";
            _file << "_ntype = record_type(_data, _namespaces);\n";
            _file << "_recordedNTypes.push_back(_ntype);\n";

            generate_structure(_pData, _ast, _entity, _file);

            _file << "_namespaces.pop_back();\n";
            // _file << "}\n";

            // _pData->m_pCurrent = _casted;
        }
        else if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_enum)
        {
            const Parser::__ast_entity* _parent = _entity;
            std::string _additionalNamespace;

            std::string _combined = combine_namespace(_pData->m_aNamespaces);

            for(const auto& _define : _entity->m_aNeededDefines)
            {
                _file << "#ifdef " << _define << "\n";
            }

            if(_combined.size() > 0)
            {
                _additionalNamespace += _combined + "::";
            }

            std::queue<std::string> _ns;

            while(_parent->m_pParentScope)
            {
                if(_parent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
                {
                    const Parser::__ast_entity_structure* _c = static_cast<const Parser::__ast_entity_structure*>(_parent);

                    _ns.push(_c->m_sName);
                }

                _parent = _parent->m_pParentScope;
            }

            while(!_ns.empty())
            {
                _additionalNamespace += _ns.front() + "::";

                _ns.pop();
            }

            const Parser::__ast_entity_enum* _castedEnum = static_cast<const Parser::__ast_entity_enum*>(_entity);

            _file << "_enum = record_enum<" << _additionalNamespace << _castedEnum->m_sName << ">(_data, _ntype, \"" << _castedEnum->m_sName << "\");\n";

            // for(const Parser::__ast_meta& _meta : _castedEnum->m_aMeta)
            // {
            //     _file << "record_meta(_data, _ntype, _enum, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
            // }

            for(const auto& _element : _castedEnum->m_aElements)
            {
                _file << "_enumElement = record_enum_element(_data, _ntype, _enum, " << _additionalNamespace << _castedEnum->m_sName << "::" << _element << ", \"" << _element << "\");\n";
            }

            for(const auto& _define : _entity->m_aNeededDefines)
            {
                _file << "#endif\n";
            }
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ast, _ent, _file);
        }

        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
        {
            _pData->m_aNamespaces.pop_back();

            _file << "_namespaces.pop_back();\n";
            _file << "}\n";
        }
    }

    __generator_data* init(Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator)
    {
        __generator_data* _data = static_cast<__generator_data*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__generator_data), alignof(__generator_data)));

        // _data->m_aNamespaces = Memory::Vector<const char*>(_pMemory, _pAllocator, 3);

        _data->m_bWasNamespaces = false;

        return _data;
    }

    void add_includes(std::ofstream& _sFile, const Parser::__ast_entity* _pScope)
    {
        for (const auto& _m : _pScope->m_aMeta)
        {
            if (_m.m_sKey == "Include")
            {
                _sFile << "#include " << _m.m_sValue << "\n";
            }
        }

        for (const auto& _scope : _pScope->m_aScopes)
        {
            add_includes(_sFile, _scope);
        }
    }

    void generate(__generator_data* _pData, const char _sSourcePath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const char _sHeaderPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast, void (*_fnGenerate)(std::ofstream& _hFile, std::ofstream& _sFile, void* _pUserData, const Duckvil::RuntimeReflection::__ftable* _ftableReflection), void* _pUserData, const Duckvil::RuntimeReflection::__ftable* _ftableReflection)
    {
        if(!std::filesystem::exists(std::filesystem::path(_sHeaderPath).parent_path()))
        {
            std::filesystem::create_directories(std::filesystem::path(_sHeaderPath).parent_path());
        }

        std::ofstream _sFile(_sSourcePath);

        std::replace(_pData->m_sInclude, _pData->m_sInclude + DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX, '\\', '/');

        _sFile << "#include \"" << _pData->m_sInclude << "\"\n";
        _sFile << "#include \"RuntimeReflection/Recorder.h\"\n";
        _sFile << "#include \"RuntimeReflection/GeneratedMeta.h\"\n\n";

        add_includes(_sFile, &_ast.m_main);

        {
            std::ofstream _hFile(_sHeaderPath);

            _fnGenerate(_hFile, _sFile, _pUserData, _ftableReflection);

            _hFile << "\n";
            _sFile << "\n";

            _hFile.close();
        }

        {
            _sFile << "DUCKVIL_RUNTIME_REFLECTION_RECORD(" << _pData->m_uiRecorderIndex << ")\n";
            _sFile << "{\n";

            _sFile << "using namespace Duckvil::RuntimeReflection;\n";
            _sFile << "using namespace Duckvil;\n";
            _sFile << "DUCKVIL_RESOURCE(type_t) _type;\n";
            _sFile << "DUCKVIL_RESOURCE(property_t) _property;\n";
            _sFile << "DUCKVIL_RESOURCE(constructor_t) _constructor;\n";
            _sFile << "DUCKVIL_RESOURCE(destructor_t) _destructor;\n";
            _sFile << "DUCKVIL_RESOURCE(function_t) _function;\n";
            _sFile << "DUCKVIL_RESOURCE(enum_t) _enum;\n";
            _sFile << "DUCKVIL_RESOURCE(enum_element_t) _enumElement;\n";
            _sFile << "DUCKVIL_RESOURCE(ntype_t) _ntype;\n";
            _sFile << "std::vector<" << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::__duckvil_resource_type_t) << "> _recordedTypes;\n";
            _sFile << "std::vector<" << DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::__duckvil_resource_ntype_t) << "> _recordedNTypes;\n";
            _sFile << "std::vector<const char*> _namespaces;\n";

            recursive(_pData, _ast, &_ast.m_main, _sFile);

            _sFile << "return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, \"" << _pData->m_sInclude << "\", " << _pData->m_uiRecorderIndex << ");\n";
            _sFile << "}\n";

            _sFile << "#ifdef DUCKVIL_RUNTIME_COMPILE\n";
            _sFile << "DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()\n";
            _sFile << "{\n";
            _sFile << "return " << _pData->m_uiRecorderIndex << ";\n";
            _sFile << "}\n";
            _sFile << "#endif\n";
        }

        _sFile.close();
    }

}}

const Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init()
{
    static Duckvil::RuntimeReflection::__generator_ftable _ftable = { 0 };

    _ftable.init = &Duckvil::RuntimeReflection::init;
    _ftable.generate = &Duckvil::RuntimeReflection::generate;

    return &_ftable;
}