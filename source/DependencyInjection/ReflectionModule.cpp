#include "DependencyInjection/ReflectionModule.h"

namespace Duckvil { namespace DependencyInjection {

    void recursive(ReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);
    void recursive_generate_header(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, RuntimeReflection::GeneratedVector& _aGenerated);
    void recursive_generate_source(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName);

    ReflectionModule::ReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
        m_pRuntimeReflection(_pReflection),
        m_pRuntimeReflectionData(_pRuntimeReflectionData),
        m_heap(_heap)
    {

    }

    ReflectionModule::~ReflectionModule()
    {

    }

    void ReflectionModule::ProcessAST(Parser::__ast* _ast)
    {
        m_sFile = _ast->m_sFile;

        recursive(this, *_ast, &_ast->m_main);
    }

    void ReflectionModule::GenerateCustom(std::ofstream& _hFile, std::ofstream& _sFile, RuntimeReflection::GeneratedVector& _aGenerated)
    {
        if(m_aContexts.empty())
        {
            return;
        }

        std::string _fileID = m_sFile.string();

        _fileID = Utils::replace_all(_fileID, "\\", "_");
        _fileID = Utils::replace_all(_fileID, ".", "_");

        recursive_generate_header(&m_aContexts, _fileID, _hFile, m_sReflectionModuleName, _aGenerated);
        recursive_generate_source(&m_aContexts, _fileID, _sFile, m_sReflectionModuleName);
    }

    void ReflectionModule::Clear()
    {
        m_aContexts.clear();
        m_pCurrentContext = nullptr;
    }

    void recursive(ReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }

            return;
        }

        auto _struct = static_cast<Parser::__ast_entity_structure*>(_entity);

        Parser::__ast_entity* _current = _struct->m_pParentScope;
        std::vector<const char*> _namespaces;

        while(_current != nullptr && _current->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
        {
            if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
            {
                auto _entity = static_cast<Parser::__ast_entity_namespace*>(_current);

                _namespaces.push_back(_entity->m_sName.c_str());
            }
            else if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
            {
                auto _entity = static_cast<Parser::__ast_entity_structure*>(_current);

                _namespaces.push_back(_entity->m_sName.c_str());
            }

            _current = _current->m_pParentScope;
        }

        std::reverse(_namespaces.begin(), _namespaces.end());

        bool _found = false;

        for(const auto& _meta : _struct->m_aMeta)
        {
            if(_meta.m_sKey == DUCKVIL_TO_STRING(Duckvil::DependencyInjection::INJECTABLE) && _meta.m_sValue == "true")
            {
                _found = true;

                break;
            }
        }

        if(!_found)
        {
            return;
        }

        ReflectionModule::Context* _context = new ReflectionModule::Context();

        _context->m_pParent = _pData->m_pCurrentContext;
        _context->m_aNamespaces.push_back(_struct->m_sName);
        _context->m_sClassName = _struct->m_sName;

        Parser::__ast_entity* _curr = _struct->m_pParentScope;

        while(_curr != nullptr)
        {
            if(_curr->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
            {
                _context->m_aNamespaces.push_back(static_cast<Parser::__ast_entity_namespace*>(_curr)->m_sName);
            }
            else if(_curr->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
            {
                _context->m_aNamespaces.push_back(static_cast<Parser::__ast_entity_structure*>(_curr)->m_sName);
            }

            _curr = _curr->m_pParentScope;
        }

        std::reverse(_context->m_aNamespaces.begin(), _context->m_aNamespaces.end());

        if(_pData->m_pCurrentContext != nullptr)
        {
            _pData->m_pCurrentContext->m_aChildren.push_back(_context);

            _pData->m_pCurrentContext = _context;
        }
        else
        {
            _pData->m_aContexts.push_back(_context);

            _pData->m_pCurrentContext = _pData->m_aContexts.back();
        }

        _pData->m_pCurrentContext->m_uiGeneratedBodyLine = _struct->m_uiGeneratedBodyMacroLine;

        if(_pData->m_pCurrentContext->m_uiGeneratedBodyLine == -1)
        {
            for(Parser::__ast_entity* _ent : _entity->m_aScopes)
            {
                recursive(_pData, _ast, _ent);
            }

            return;
        }

        Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

        _func->m_sReturnType = "void";
        _func->m_sName = "Push";
        _func->m_pParentScope = _struct;
        _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
        _func->m_flags = static_cast<Parser::__ast_flags>(Parser::__ast_flags::__ast_flags_static);
        _func->m_aMeta.push_back(Parser::__ast_meta{ DUCKVIL_TO_STRING(Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated), "true" });

        {
            Parser::__ast_entity_argument _arg;
            
            _arg.m_sName = "_pFAP";
            _arg.m_sType = DUCKVIL_TO_STRING(Duckvil::DependencyInjection::IFunctionArgumentsInjector*);
            _arg.m_pParentScope = _func;

            _func->m_aArguments.push_back(_arg);
        }

        {
            Parser::__ast_entity_argument _arg;
            
            _arg.m_sName = "_pData";
            _arg.m_sType = "const void*";
            _arg.m_pParentScope = _func;

            _func->m_aArguments.push_back(_arg);
        }

        _struct->m_aScopes.push_back(_func);

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ast, _ent);
        }

        _pData->m_pCurrentContext = _pData->m_pCurrentContext ? _pData->m_pCurrentContext->m_pParent : nullptr;
    }

    void recursive_generate_header(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, RuntimeReflection::GeneratedVector& _aGenerated)
    {
        const std::vector<ReflectionModule::Context*>& _contexts = *static_cast<std::vector<ReflectionModule::Context*>*>(_pContext);

        for(ReflectionModule::Context* _context : _contexts)
        {
            if(_context->m_uiGeneratedBodyLine == -1)
            {
                recursive_generate_header(&_context->m_aChildren, _sFileID, _file, _sModuleName, _aGenerated);

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
            _file << "public:\\\n";
            _file << "static void Push(DependencyInjection::IDependencyInjector* _pFAP, const void* _pData);\n";
        }
    }

    void recursive_generate_source(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName)
    {
        const std::vector<ReflectionModule::Context*>& _contexts = *static_cast<std::vector<ReflectionModule::Context*>*>(_pContext);

        for(ReflectionModule::Context* _context : _contexts)
        {
            if(_context->m_uiGeneratedBodyLine == -1)
            {
                recursive_generate_source(&_context->m_aChildren, _sFileID, _file, _sModuleName);

                continue;
            }

            std::string _namespace;

            for(const auto& _n : _context->m_aNamespaces)
            {
                _namespace += _n + "::";
            }

            _file << "void " << _namespace << "Push(DependencyInjection::IDependencyInjector* _pFAP, const void* _pData)\n";
            _file << "{\n";
            _file << "_pFAP->Push(*static_cast<const " << _context->m_sClassName << "*>(_pData));\n";
            _file << "}\n";
        }
    }

}}