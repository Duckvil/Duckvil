#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "Engine/ReflectionFlags.h"

#include "Parser/AST.h"

#include "Memory/FreeList.h"

#include "Utils/Utils.h"

#include <fstream>
#include <filesystem>
#include <sstream>

// Intellisense isn't clever enough to prompt about macro's structure variables 😥

namespace Duckvil {

    class NamedArgumentsReflectionModule;

    static void recursive(NamedArgumentsReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);
    static void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, RuntimeReflection::GeneratedVector& _aGenerated);
    static void find(Parser::__ast_entity* _pEntity, Parser::__ast_entity_type _type, bool (*_fnCmp)(Parser::__ast_entity* _pEntity), Parser::__ast_entity** _ppFoundEntity);

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class NamedArgumentsReflectionModule
    {
    public:
        struct argument
        {
            std::string m_sName;
            std::string m_sType;
            std::string m_sValue;
        };

        struct function
        {
            std::string m_sName;
            std::string m_sReturnType;
            std::vector<NamedArgumentsReflectionModule::argument> m_aArguments;
        };

        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            bool m_bIsStruct;
            std::string m_sTypeName;
            std::vector<NamedArgumentsReflectionModule::Context*> m_aChildren;
            NamedArgumentsReflectionModule::Context* m_pParent;
            std::vector<NamedArgumentsReflectionModule::function> m_aFunctions;
        };

        const char* m_sReflectionModuleName = "NAMED_ARGUMENTS";

    private:

    public:
        NamedArgumentsReflectionModule(const Memory::FreeList& _heap, const RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pReflection(_pReflection),
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_heap(_heap)
        {
            RuntimeReflection::make_current(duckvil_frontend_reflection_context{ .m_pReflection = _pReflection, .m_pReflectionData = _pRuntimeReflectionData });
        }

        ~NamedArgumentsReflectionModule()
        {

        }

        Memory::FreeList m_heap;

        const RuntimeReflection::__ftable* m_pReflection;
        RuntimeReflection::__data* m_pRuntimeReflectionData;

        std::filesystem::path m_sFile;

        std::vector<NamedArgumentsReflectionModule::Context*> m_aContexts;
        NamedArgumentsReflectionModule::Context* m_pCurrentContext;

        void ProcessAST(Parser::__ast* _ast)
        {
            m_sFile = _ast->m_sFile;

            recursive(this, *_ast, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _hFile, std::ofstream& _sFile, RuntimeReflection::GeneratedVector& _aGenerated)
        {
            if(m_aContexts.empty())
            {
                return;
            }

            std::string _fileID = m_sFile.string();

            _fileID = Utils::replace_all(_fileID, "\\", "_");
            _fileID = Utils::replace_all(_fileID, ".", "_");

            recursive_generate(&m_aContexts, _fileID, _hFile, m_sReflectionModuleName, _aGenerated);
        }

        void Clear()
        {
            m_aContexts.clear();
            m_pCurrentContext = nullptr;
        }
    };

    void recursive(NamedArgumentsReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }

            return;
        }

        if(_pData->m_pCurrentContext != nullptr)
        {
            NamedArgumentsReflectionModule::Context* _context = new NamedArgumentsReflectionModule::Context();

            _context->m_pParent = _pData->m_pCurrentContext;

            _pData->m_pCurrentContext->m_aChildren.push_back(_context);

            _pData->m_pCurrentContext = _context;
        }
        else
        {
            NamedArgumentsReflectionModule::Context* _context = new NamedArgumentsReflectionModule::Context();

            _context->m_pParent = _pData->m_pCurrentContext;

            _pData->m_aContexts.push_back(_context);

            _pData->m_pCurrentContext = _pData->m_aContexts.back();
        }

        _pData->m_pCurrentContext->m_bIsStruct = true;

        Parser::__ast_entity_structure* _struct = (Parser::__ast_entity_structure*)_entity;

        _pData->m_pCurrentContext->m_uiGeneratedBodyLine = _struct->m_uiGeneratedBodyMacroLine;

        if(_pData->m_pCurrentContext->m_uiGeneratedBodyLine == -1)
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }

            return;
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            if(_ent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_function)
            {
                auto _func = static_cast<Parser::__ast_entity_function*>(_ent);
                bool _found = false;

                for(const auto& _meta : _func->m_aMeta)
                {
                    if(_meta.m_sKey == "\"NamedArguments\"")
                    {
                        _found = true;

                        break;
                    }
                }

                if(!_found)
                {
                    continue;
                }

                NamedArgumentsReflectionModule::function _f;

                _f.m_sName = _func->m_sName.c_str();
                _f.m_sReturnType = _func->m_sReturnType;

                for(const auto& _arg : _func->m_aArguments)
                {
                    NamedArgumentsReflectionModule::argument _a;

                    _a.m_sName = _arg.m_sName;
                    _a.m_sType = _arg.m_sType;
                    _a.m_sValue = _arg.m_sValue;

                    _f.m_aArguments.push_back(_a);
                }

                _pData->m_pCurrentContext->m_aFunctions.push_back(_f);
            }
            else
            {
                recursive(_pData, _ast, _ent);
            }
        }

        _pData->m_pCurrentContext = _pData->m_pCurrentContext ? _pData->m_pCurrentContext->m_pParent : nullptr;
    }

    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, RuntimeReflection::GeneratedVector& _aGenerated)
    {
        const std::vector<NamedArgumentsReflectionModule::Context*>& _contexts = *static_cast<std::vector<NamedArgumentsReflectionModule::Context*>*>(_pContext);

        for(NamedArgumentsReflectionModule::Context* _context : _contexts)
        {
            if(_context->m_uiGeneratedBodyLine == -1 || _context->m_aFunctions.empty())
            {
                recursive_generate(&_context->m_aChildren, _sFileID, _file, _sModuleName, _aGenerated);

                continue;
            }

            bool _found = false;

            for(auto& _generated : _aGenerated)
            {
                if(_generated.first == _context->m_uiGeneratedBodyLine)
                {
                    _found = true;

                    _generated.second.push_back(_sModuleName);

                    break;
                }
            }

            if(!_found)
            {
                std::vector<std::string> _tmp;

                _tmp.push_back(_sModuleName);

                _aGenerated.push_back(std::make_pair(_context->m_uiGeneratedBodyLine, _tmp));
            }

            for(const auto& _func : _context->m_aFunctions)
            {
                _file << "struct " << _sFileID << "_" << _context->m_uiGeneratedBodyLine << "_" << _func.m_sName << "_args\n";
                _file << "{\n";

                for(const auto& _arg : _func.m_aArguments)
                {
                    _file << _arg.m_sType << " " << _arg.m_sName << (_arg.m_sValue != "" ? " = " : "") << _arg.m_sValue << ";\n";
                }

                _file << "};\n";
            }

            _file << "#define " << _sFileID << "_" << _context->m_uiGeneratedBodyLine << "_REFLECTION_MODULE_" << _sModuleName << " \\\n";
            _file << "public: \\\n";

            for(uint32_t _i = 0; _i < _context->m_aFunctions.size(); ++_i)
            {
                const auto& _func = _context->m_aFunctions[_i];

                _file << "inline " << _func.m_sReturnType << " " << _func.m_sName << "(const " << _sFileID << "_" << _context->m_uiGeneratedBodyLine << "_" << _func.m_sName << "_args& _args)\\\n";
                _file << "{\\\n";

                if(_func.m_sReturnType != "void")
                {
                    _file << "return ";
                }

                _file << _func.m_sName << "(";

                for(uint32_t _i = 0; _i < _func.m_aArguments.size(); ++_i)
                {
                    _file << "_args." << _func.m_aArguments[_i].m_sName;

                    if(_i < _func.m_aArguments.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << ");\\\n";
                _file << "}" << (_i < _context->m_aFunctions.size() - 1 ? "\\" : "") << "\n";
            }

            recursive_generate(&_context->m_aChildren, _sFileID, _file, _sModuleName, _aGenerated);
        }
    }

    void find(Parser::__ast_entity* _pEntity, Parser::__ast_entity_type _type, bool (*_fnCmp)(Parser::__ast_entity* _pEntity), Parser::__ast_entity** _ppFoundEntity)
    {
        if(*_ppFoundEntity != nullptr)
        {
            return;
        }

        for(Parser::__ast_entity* _entity : _pEntity->m_aScopes)
        {
            if(_entity->m_scopeType == _type && _fnCmp(_entity))
            {
                *_ppFoundEntity = _entity;

                break;
            }

            find(_entity, _type, _fnCmp, _ppFoundEntity);
        }
    }

}