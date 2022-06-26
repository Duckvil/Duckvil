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

// Dummy reflection module will make sure if there is DUCKVIL_GENERATED_BODY defined in structure, *FILE_ID*_*DUCKVIL_GENERATED_BODY_LINE*_GENERATED_BODY macro will be generated to prevent errors(in theory, still needs testing...)

namespace Duckvil {

    class DummyReflectionModule;

    static void recursive(DummyReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);
    static void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated);

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_ReflectionModule)
    class DummyReflectionModule
    {
    public:
        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            bool m_bIsStruct;
            std::string m_sTypeName;
            std::vector<DummyReflectionModule::Context*> m_aChildren;
            DummyReflectionModule::Context* m_pParent;
        };

        const char* m_sReflectionModuleName = "DUMMY";

    private:

    public:
        DummyReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pReflection(_pReflection),
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_heap(_heap)
        {
            RuntimeReflection::make_current(duckvil_frontend_reflection_context{ .m_pReflection = _pReflection, .m_pReflectionData = _pRuntimeReflectionData });
        }

        ~DummyReflectionModule()
        {

        }

        Memory::FreeList m_heap;

        RuntimeReflection::__ftable* m_pReflection;
        RuntimeReflection::__data* m_pRuntimeReflectionData;

        std::filesystem::path m_sFile;

        std::vector<DummyReflectionModule::Context*> m_aContexts;
        DummyReflectionModule::Context* m_pCurrentContext;

        void ProcessAST(Parser::__ast* _ast)
        {
            m_sFile = _ast->m_sFile;

            recursive(this, *_ast, &_ast->m_main);
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

    void recursive(DummyReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity)
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
            DummyReflectionModule::Context* _context = new DummyReflectionModule::Context();

            _context->m_pParent = _pData->m_pCurrentContext;

            _pData->m_pCurrentContext->m_aChildren.push_back(_context);

            _pData->m_pCurrentContext = _context;
        }
        else
        {
            DummyReflectionModule::Context* _context = new DummyReflectionModule::Context();

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
            recursive(_pData, _ast, _ent);
        }

        _pData->m_pCurrentContext = _pData->m_pCurrentContext ? _pData->m_pCurrentContext->m_pParent : nullptr;
    }

    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
    {
        const std::vector<DummyReflectionModule::Context*>& _contexts = *static_cast<std::vector<DummyReflectionModule::Context*>*>(_pContext);

        for(DummyReflectionModule::Context* _context : _contexts)
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

            _file << "#define " << _sFileID << "_" << _context->m_uiGeneratedBodyLine << "_REFLECTION_MODULE_" << _sModuleName << " \n";

            recursive_generate(&_context->m_aChildren, _sFileID, _file, _sModuleName, _aGenerated);
        }
    }

}