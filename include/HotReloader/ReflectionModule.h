#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Engine/ReflectionFlags.h"

#include "Parser/AST.h"

#include "Memory/FreeList.h"

#include <fstream>
#include <filesystem>

#include "Serializer/Runtime/ISerializer.h"

#include "Engine/ISystem.h"

namespace Duckvil { namespace HotReloader {

    class RuntimeCompilerReflectionModule;

    void recursive(RuntimeCompilerReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);
    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated);

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule)
    class RuntimeCompilerReflectionModule
    {
    public:
        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            std::vector<std::string> m_aVars;
            std::vector<std::string> m_aChildSerializers;
            std::vector<RuntimeCompilerReflectionModule::Context*> m_aChildren;
            RuntimeCompilerReflectionModule::Context* m_pParent;
        };

        const char* m_sReflectionModuleName = "RUNTIME_COMPILER";

    private:

    public:
        RuntimeCompilerReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_pRuntimeReflection(_pReflection),
            m_heap(_heap)
        {
            m_pCurrentContext = nullptr;
        }

        ~RuntimeCompilerReflectionModule()
        {

        }

        RuntimeReflection::__data* m_pRuntimeReflectionData;
        RuntimeReflection::__ftable* m_pRuntimeReflection;
        Memory::FreeList m_heap;

        std::filesystem::path m_sFile;

        std::vector<RuntimeCompilerReflectionModule::Context*> m_aContexts;
        RuntimeCompilerReflectionModule::Context* m_pCurrentContext;

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

    void recursive(RuntimeCompilerReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            if(_pData->m_pCurrentContext != nullptr)
            {
                RuntimeCompilerReflectionModule::Context* _context = new RuntimeCompilerReflectionModule::Context();

                _context->m_pParent = _pData->m_pCurrentContext;

                _pData->m_pCurrentContext->m_aChildren.push_back(_context);

                _pData->m_pCurrentContext = _context;
            }
            else
            {
                RuntimeCompilerReflectionModule::Context* _context = new RuntimeCompilerReflectionModule::Context();

                _context->m_pParent = _pData->m_pCurrentContext;

                _pData->m_aContexts.push_back(_context);

                _pData->m_pCurrentContext = _pData->m_aContexts.back();
            }

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

            if(_typeHandle.m_ID != -1)
            {
                const Memory::Vector<RuntimeReflection::__duckvil_resource_inheritance_t>& _inhs = _pData->m_pRuntimeReflection->m_fnGetInheritances(_pData->m_pRuntimeReflectionData, _pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), _typeHandle);

                for(const auto& _inh : _inhs)
                {
                    const RuntimeReflection::__inheritance_t& _inhData = _pData->m_pRuntimeReflection->m_fnGetInheritance(_pData->m_pRuntimeReflectionData, _typeHandle, _inh);
                    RuntimeReflection::__duckvil_resource_type_t _inhTypeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData, _inhData.m_ullInheritanceTypeID);

                    if(_inhTypeHandle.m_ID != -1)
                    {
                        RuntimeReflection::__duckvil_resource_function_t _funcHandle = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(_pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData, _inhTypeHandle, "Serialize");

                        if(_funcHandle.m_ID != -1)
                        {
                            const RuntimeReflection::__type_t& _inhType = _pData->m_pRuntimeReflection->m_fnGetType(_pData->m_pRuntimeReflectionData, _inhTypeHandle);

                            _pData->m_pCurrentContext->m_aChildSerializers.push_back(_inhType.m_sTypeName);
                        }
                    }
                }
            }

            Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

            _func->m_sReturnType = "void";
            _func->m_sName = "Serialize";
            _func->m_pParentScope = _struct;
            _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
            _func->m_flags = (Parser::__ast_flags)0;
            _func->m_aMeta.push_back(Parser::__ast_meta{ DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated), "true" });

            {
                Parser::__ast_entity_argument _arg;

                _arg.m_sName = "_pSerializer";
                _arg.m_sType = "Duckvil::RuntimeSerializer::ISerializer*";
                _arg.m_pParentScope = _func;

                _func->m_aArguments.push_back(_arg);
            }

            _struct->m_aScopes.push_back(_func);

            for(Parser::__ast_entity* _child : _struct->m_aScopes)
            {
                if(_child->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
                {
                    Parser::__ast_entity_variable* _var = (Parser::__ast_entity_variable*)_child;
                    bool _skip = false;

                    for(const auto& _needed : _var->m_aNeededDefines)
                    {
                        bool _contain = false;

                        for(const auto& _declared : _ast.m_aUserDefines)
                        {
                            if(_needed == _declared.m_sUserDefine)
                            {
                                _contain = true;

                                break;
                            }
                        }

                        if(!_contain)
                        {
                            _skip = true;

                            break;
                        }
                    }

                    if(!_skip && _var->m_sType.find("const") == std::string::npos)
                    {
                        _pData->m_pCurrentContext->m_aVars.push_back(_var->m_sName);
                    }
                }
            }

            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }

            _pData->m_pCurrentContext = _pData->m_pCurrentContext ? _pData->m_pCurrentContext->m_pParent : nullptr;
        }
        else
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }
        }
    }

    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
    {
        const std::vector<RuntimeCompilerReflectionModule::Context*>& _contexts = *static_cast<std::vector<RuntimeCompilerReflectionModule::Context*>*>(_pContext);

        for(RuntimeCompilerReflectionModule::Context* _context : _contexts)
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
            _file << "void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \\\n";
            _file << "{\\\n";

            for(const auto& _var : _context->m_aVars)
            {
                _file << "_pSerializer->SerializeProperty(\"" << _var << "\", " << _var << "); \\\n";
            }

            for(const std::string& _child : _context->m_aChildSerializers)
            {
                _file << _child << "::Serialize(_pSerializer); \\\n";
            }

            _file << "}\n";

            recursive_generate(&_context->m_aChildren, _sFileID, _file, _sModuleName, _aGenerated);
        }
    }

}}