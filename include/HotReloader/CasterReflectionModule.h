#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Engine/ReflectionFlags.h"

#include "Parser/AST.h"

#include "Memory/FreeList.h"

#include "Utils/Utils.h"

#include <fstream>
#include <filesystem>
#include <sstream>

namespace Duckvil {

    class CasterReflectionModule;

    void recursive(CasterReflectionModule* _pData, Parser::__ast_entity* _entity);
    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated);

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class CasterReflectionModule
    {
    public:
        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            bool m_bIsStruct;
            std::string m_sTypeName;
            std::vector<CasterReflectionModule::Context*> m_aChildren;
            CasterReflectionModule::Context* m_pParent;
        };

        const char* m_sReflectionModuleName = "CASTER";

    private:

    public:
        CasterReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData)
        {
            m_pCurrentContext = nullptr;
        }

        ~CasterReflectionModule()
        {

        }

        std::filesystem::path m_sFile;

        std::vector<CasterReflectionModule::Context*> m_aContexts;
        CasterReflectionModule::Context* m_pCurrentContext;

        void ProcessAST(Parser::__ast* _ast)
        {
            m_sFile = _ast->m_sFile;

            recursive(this, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _file, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
        {
            if(m_aContexts.empty())
            {
                return;
            }

            std::string _fileID = m_sFile.string();

            _fileID = Utils::replace_all(_fileID, "\\", "_");
            _fileID = Utils::replace_all(_fileID, ".", "_");

            recursive_generate(&m_aContexts, _fileID, _file, m_sReflectionModuleName, _aGenerated);
        }

        void Clear()
        {
            m_aContexts.clear();
            m_pCurrentContext = nullptr;
        }
    };

    void recursive(CasterReflectionModule* _pData, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            if(_pData->m_pCurrentContext != nullptr)
            {
                CasterReflectionModule::Context* _context = new CasterReflectionModule::Context();

                _context->m_pParent = _pData->m_pCurrentContext;

                _pData->m_pCurrentContext->m_aChildren.push_back(_context);

                _pData->m_pCurrentContext = _context;
            }
            else
            {
                CasterReflectionModule::Context* _context = new CasterReflectionModule::Context();

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
                    recursive(_pData, _ent);
                }

                return;
            }

            Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

            _func->m_sReturnType = "void*";
            _func->m_sName = "Cast";
            _func->m_pParentScope = _struct;
            _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
            _func->m_flags = static_cast<Parser::__ast_flags>(Parser::__ast_flags::__ast_flags_static);

            {
                Parser::__ast_entity_argument _arg;
                
                _arg.m_sName = "_pObject";
                _arg.m_sType = "void*";
                _arg.m_pParentScope = _func;

                _func->m_aArguments.push_back(_arg);
            }

            _struct->m_aScopes.push_back(_func);

            _pData->m_pCurrentContext->m_sTypeName = _struct->m_sName;

            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ent);
            }

            _pData->m_pCurrentContext = _pData->m_pCurrentContext ? _pData->m_pCurrentContext->m_pParent : nullptr;
        }
        else
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ent);
            }
        }
    }

    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
    {
        const std::vector<CasterReflectionModule::Context*>& _contexts = *static_cast<std::vector<CasterReflectionModule::Context*>*>(_pContext);

        for(CasterReflectionModule::Context* _context : _contexts)
        {
            if(_context->m_uiGeneratedBodyLine == -1)
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

            _file << "#define " << _sFileID << "_" << _context->m_uiGeneratedBodyLine << "_REFLECTION_MODULE_" << _sModuleName << " \\\n";
            _file << "public: \\\n";

            _file << "static void* Cast(void* _pObject) { return static_cast<" << _context->m_sTypeName << "*>(_pObject); }\n";

            recursive_generate(&_context->m_aChildren, _sFileID, _file, _sModuleName, _aGenerated);
        }
    }

}