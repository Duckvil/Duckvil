#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "Engine/ReflectionFlags.h"

namespace Duckvil { namespace Serializer { namespace ReflectionModule {

    class Component;

    static void recursive(Component* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class Component
    {
    public:
        const char* m_sReflectionModuleName = "COMPONENT";

    private:

    public:
        Component(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData)
        {
            m_bIsValid = false;
            m_uiGeneratedBodyLine = -1;
        }

        ~Component()
        {

        }

        bool m_bIsValid;
        std::filesystem::path m_sFile;
        uint32_t m_uiGeneratedBodyLine;
        std::string m_sTypeName;

        void ProcessAST(Parser::__ast* _ast)
        {
            m_sFile = _ast->m_sFile;

            recursive(this, *_ast, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _hFile, std::ofstream& _sFile, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
        {
            if(!m_bIsValid || m_uiGeneratedBodyLine == -1)
            {
                return;
            }

            std::string _fileID = m_sFile.string();

            _fileID = Utils::replace_all(_fileID, "\\", "_");
            _fileID = Utils::replace_all(_fileID, ".", "_");

            bool _found = false;

            for(auto& _generated : _aGenerated)
            {
                if(_generated.first == m_uiGeneratedBodyLine)
                {
                    _found = true;

                    _generated.second.push_back(m_sReflectionModuleName);

                    break;
                }
            }

            if(!_found)
            {
                std::vector<std::string> _tmp;

                _tmp.push_back(m_sReflectionModuleName);

                _aGenerated.push_back(std::make_pair(m_uiGeneratedBodyLine, _tmp));
            }

            _hFile << "#include \"Utils/EntitySerializer.h\"\n";
            _hFile << "#include \"Utils/ECS.h\"\n";
            _hFile << "#include \"Engine/Entity.h\"\n";
            _hFile << "#define " << _fileID << "_" << m_uiGeneratedBodyLine << "_REFLECTION_MODULE_" << m_sReflectionModuleName << " \\\n";
            _hFile <<
            "static void Serialize(nlohmann::json& _jOut, const Entity& _entity)\\\n"
            "{\\\n"
            "Utils::init_ecs();\\\n"
            "Utils::serialize_component<" + m_sTypeName + ">(_jOut, _entity);\\\n"
            "}\\\n"
            "\\\n"
            "static void Deserialize(const nlohmann::json& _jIn, Entity& _entity)\\\n"
            "{\\\n"
            "Utils::init_ecs();\\\n"
            "Utils::deserialize_component<" + m_sTypeName + ">(_jIn, _entity);\\\n"
            "}\\\n"
            "static bool Has(const Entity& _entity)\\\n"
            "{\\\n"
            "Utils::init_ecs();\\\n"
            "return _entity.Has<" + m_sTypeName + ">();\\\n"
            "}\\\n"
            "\\\n"
            "static uint8_t* Get(const Entity& _entity)\\\n"
            "{\\\n"
            "Utils::init_ecs();\\\n"
            "return reinterpret_cast<uint8_t*>(_entity.m_entity.get_mut<" + m_sTypeName + ">());\\\n"
            "}\n"
            ;
        }

        void Clear()
        {
            m_bIsValid = false;
            m_uiGeneratedBodyLine = -1;
        }
    };

    void recursive(Component* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }

            return;
        }

        Parser::__ast_entity_structure* _struct = (Parser::__ast_entity_structure*)_entity;

        for(const auto& _meta : _entity->m_aMeta)
        {
            if(_meta.m_sKey == "\"Component\"")
            {
                _pData->m_bIsValid = true;
                _pData->m_uiGeneratedBodyLine = _struct->m_uiGeneratedBodyMacroLine;
                _pData->m_sTypeName = _struct->m_sName;

                {
                    Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                    _func->m_sReturnType = "void";
                    _func->m_sName = "Serialize";
                    _func->m_pParentScope = _struct;
                    _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                    _func->m_flags = static_cast<Parser::__ast_flags>(Parser::__ast_flags::__ast_flags_static);
                    _func->m_aMeta.push_back(Parser::__ast_meta{ DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated), "true" });

                    {
                        Parser::__ast_entity_argument _arg;
                        
                        _arg.m_sName = "_jOut";
                        _arg.m_sType = "nlohmann::json&";
                        _arg.m_pParentScope = _func;

                        _func->m_aArguments.push_back(_arg);
                    }

                    {
                        Parser::__ast_entity_argument _arg;
                        
                        _arg.m_sName = "_entity";
                        _arg.m_sType = "const Entity&";
                        _arg.m_pParentScope = _func;

                        _func->m_aArguments.push_back(_arg);
                    }

                    _struct->m_aScopes.push_back(_func);
                }

                {
                    Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                    _func->m_sReturnType = "void";
                    _func->m_sName = "Deserialize";
                    _func->m_pParentScope = _struct;
                    _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                    _func->m_flags = static_cast<Parser::__ast_flags>(Parser::__ast_flags::__ast_flags_static);
                    _func->m_aMeta.push_back(Parser::__ast_meta{ DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated), "true" });

                    {
                        Parser::__ast_entity_argument _arg;
                        
                        _arg.m_sName = "_jIn";
                        _arg.m_sType = "const nlohmann::json&";
                        _arg.m_pParentScope = _func;

                        _func->m_aArguments.push_back(_arg);
                    }

                    {
                        Parser::__ast_entity_argument _arg;
                        
                        _arg.m_sName = "_entity";
                        _arg.m_sType = "Entity&";
                        _arg.m_pParentScope = _func;

                        _func->m_aArguments.push_back(_arg);
                    }

                    _struct->m_aScopes.push_back(_func);
                }

                {
                    Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                    _func->m_sReturnType = "bool";
                    _func->m_sName = "Has";
                    _func->m_pParentScope = _struct;
                    _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                    _func->m_flags = static_cast<Parser::__ast_flags>(Parser::__ast_flags::__ast_flags_static);
                    _func->m_aMeta.push_back(Parser::__ast_meta{ DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated), "true" });

                    {
                        Parser::__ast_entity_argument _arg;
                        
                        _arg.m_sName = "_entity";
                        _arg.m_sType = "const Entity&";
                        _arg.m_pParentScope = _func;

                        _func->m_aArguments.push_back(_arg);
                    }

                    _struct->m_aScopes.push_back(_func);
                }

                {
                    Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                    _func->m_sReturnType = "uint8_t*";
                    _func->m_sName = "Get";
                    _func->m_pParentScope = _struct;
                    _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                    _func->m_flags = static_cast<Parser::__ast_flags>(Parser::__ast_flags::__ast_flags_static);
                    _func->m_aMeta.push_back(Parser::__ast_meta{ DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated), "true" });

                    {
                        Parser::__ast_entity_argument _arg;
                        
                        _arg.m_sName = "_entity";
                        _arg.m_sType = "const Entity&";
                        _arg.m_pParentScope = _func;

                        _func->m_aArguments.push_back(_arg);
                    }

                    _struct->m_aScopes.push_back(_func);
                }

                break;
            }
        }
    }

}}}