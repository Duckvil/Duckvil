#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Parser/AST.h"

#include "Engine/ReflectionFlags.h"

#include "Memory/FreeList.h"
#include "Utils/Function.h"

namespace Duckvil { namespace CSharp {

    class ReflectionModule;

    void recursive(ReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);
    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated);
    void recursive_generate_source(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName);

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule)
    class ReflectionModule
    {
    public:
        struct CSharpCall
        {
            std::string m_sName;
            std::vector<std::string> m_aArgs;
            std::string m_sReturnType;
        };

        struct InternalCall
        {
            std::string m_sName;
            std::vector<Parser::__ast_entity_argument> m_aArgs;
        };

        struct Variable
        {
            std::string m_sName;
            std::string m_sType;
        };

        struct Context
        {
            uint32_t m_uiGeneratedBodyLine;
            std::vector<ReflectionModule::InternalCall> m_aInternalCalls;
            std::vector<ReflectionModule::CSharpCall> m_aCSharpCalls;
            std::vector<ReflectionModule::Variable> m_aVariables;
            std::vector<ReflectionModule::Context*> m_aChildren;
            ReflectionModule::Context* m_pParent;
            std::vector<std::string> m_aNamespaces;
            std::string m_sClassName;
        };

        const char* m_sReflectionModuleName = "CSHARP";

    private:

    public:
        ReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_pRuntimeReflection(_pReflection),
            m_heap(_heap)
        {

        }

        ~ReflectionModule()
        {

        }

        RuntimeReflection::__data* m_pRuntimeReflectionData;
        RuntimeReflection::__ftable* m_pRuntimeReflection;
        Memory::FreeList m_heap;

        std::filesystem::path m_sFile;

        std::vector<ReflectionModule::Context*> m_aContexts;
        ReflectionModule::Context* m_pCurrentContext;

        void ProcessAST(Parser::__ast* _ast)
        {
            m_sFile = _ast->m_sFile;

            recursive(this, *_ast, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _hFile, std::ofstream& _sFile, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
        {
            if(m_aContexts.empty())
            {
                return;
            }

            std::string _fileID = m_sFile.string();

            _fileID = Utils::replace_all(_fileID, "\\", "_");
            _fileID = Utils::replace_all(_fileID, ".", "_");

            recursive_generate(&m_aContexts, _fileID, _hFile, m_sReflectionModuleName, _aGenerated);
            recursive_generate_source(&m_aContexts, _fileID, _sFile, m_sReflectionModuleName);
        }

        void Clear()
        {
            // ms_pCurrentModule = nullptr;

            m_aContexts.clear();
            m_pCurrentContext = nullptr;
        }
    };

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

        Parser::__ast_entity_structure* _struct = (Parser::__ast_entity_structure*)_entity;

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
        bool _found = false;

        if(_typeHandle.m_ID != -1)
        {
            RuntimeReflection::__duckvil_resource_type_t _sharedScriptTypeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, "SharedScript", { "Duckvil", "CSharp" });

            _found = _pData->m_pRuntimeReflection->m_fnInheritsByHandle(_pData->m_pRuntimeReflectionData, _typeHandle, _sharedScriptTypeHandle);
        }
        else
        {
            for(const auto& _inh : _struct->m_aInheritance)
            {
                if(_inh.m_sName == "SharedScript")
                {
                    _found = true;

                    break;
                }
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

        for(Parser::__ast_entity* _child : _struct->m_aScopes)
        {
            _found = false;

            if(_child->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_function)
            {
                Parser::__ast_entity_function* _func = (Parser::__ast_entity_function*)_child;

                for(const auto& _meta : _func->m_aMeta)
                {
                    if(_meta.m_sKey == "CSharp::ReflectionFlags::EXPOSE_TO_CSHARP")
                    {
                        _found = true;

                        break;
                    }
                }

                if(!_found)
                {
                    continue;
                }

                _pData->m_pCurrentContext->m_aInternalCalls.push_back({ _func->m_sName, _func->m_aArguments });
            }
            else if(_child->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_define)
            {
                Parser::__ast_entity_define* _d = (Parser::__ast_entity_define*)_child;

                if(_d->m_sName != "DUCKVIL_CS_FUNCTION")
                {
                    continue;
                }

                ReflectionModule::CSharpCall _call;

                _call.m_sName = _d->m_aArguments[1];
                _call.m_sReturnType = _d->m_aArguments[0];

                std::string _args = _d->m_aArguments[2];
                std::string _curr;

                for(const auto& _c : _args)
                {
                    if(_c == '{')
                    {

                    }
                    else if(_c == '}')
                    {

                    }
                    else if(_c == ';')
                    {
                        _call.m_aArgs.push_back(_curr);

                        _curr.clear();
                    }
                    else
                    {
                        _curr += _c;
                    }
                }

                _pData->m_pCurrentContext->m_aCSharpCalls.push_back(_call);
            }
            else if(_child->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
            {
                Parser::__ast_entity_variable* _var = (Parser::__ast_entity_variable*)_child;

                for (const auto& _meta : _var->m_aMeta)
                {
                    if (_meta.m_sKey == "CSharp::ReflectionFlags::EXPOSE_TO_CSHARP")
                    {
                        _found = true;

                        break;
                    }
                }

                if (!_found)
                {
                    continue;
                }

                _pData->m_pCurrentContext->m_aVariables.push_back({ _var->m_sName, _var->m_sType });
            }
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ast, _ent);
        }

        _pData->m_pCurrentContext = _pData->m_pCurrentContext ? _pData->m_pCurrentContext->m_pParent : nullptr;
    }

#define DUCKVIL_CS_FUNCTION(rType, name, args) \
    struct duckvil_cs_ ## name args; \
    rType name(duckvil_cs_ ## name _args) { generated_ ## name(_args); }

    void generate_internal_call(std::ofstream& _file, const ReflectionModule::InternalCall& _call, const std::string_view& _sNewLine = "\\\n", void (*_fnBody)(std::ofstream&) = nullptr)
    {
        _file << (_fnBody ? "" : "static ") << "void csharp_internal_" << _call.m_sName << "(size_t _ullSharedScriptID";

        if(!_call.m_aArgs.empty())
        {
            _file << ", ";

            for(uint32_t _i = 0; _i < _call.m_aArgs.size(); ++_i)
            {
                const auto& _arg = _call.m_aArgs[_i];

                _file << _arg.m_sType << " " << _arg.m_sName;

                if(_i < _call.m_aArgs.size() - 1)
                {
                    _file << ", ";
                }
            }
        }
        
        _file << ")";

        if(_fnBody)
        {
            _file << "{" << _sNewLine;
            _fnBody(_file);
            _file << "}" << _sNewLine;
        }
        else
        {
            _file << ";" << _sNewLine;
        }
    }

    void recursive_generate(void* _pContext, const std::string& _sFileID, std::ofstream& _file, const char* _sModuleName, std::vector<std::pair<uint32_t, std::vector<std::string>>>& _aGenerated)
    {
        const std::vector<ReflectionModule::Context*>& _contexts = *static_cast<std::vector<ReflectionModule::Context*>*>(_pContext);

        for(ReflectionModule::Context* _context : _contexts)
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
            _file << "public:\\\n";

            for(const auto& _var : _context->m_aVariables)
            {
                if(_var.m_sName.find_first_of("_an_") == std::string::npos)
                {
	                continue;
                }

                std::string _name = Utils::replace_all(_var.m_sName, "_an_", "");

                _file << "MonoClassField* m_pCSharpField_" << _name << ";\\\n";
                _file << "void " << _name << "(" << _var.m_sType << " _v);\\\n";
                _file << _var.m_sType << " " << _name << "() const;\\\n";
                _file << "DUCKVIL_CSHARP_VARIABLE_CHANGED(" << _context->m_sClassName << ", " << _var.m_sType << ", " << _name << ");\\\n";
            }

            for(const auto& _csCall : _context->m_aCSharpCalls)
            {
                _file << "MonoMethod* m_pCSharpMethod_" << _csCall.m_sName << ";\\\n";
                _file << _csCall.m_sReturnType << " generated_csharp_" << _csCall.m_sName << "(";

                for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                {
                    const auto& _a = _csCall.m_aArgs[_i];

                    _file << _a;

                    if(_i < _csCall.m_aArgs.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << ");\\\n";

                _file << _csCall.m_sReturnType << " generated_csharp_" << _csCall.m_sName << "(duckvil_cs_" << _csCall.m_sName << " _args);\\\n";
                _file << _csCall.m_sReturnType << " " << _csCall.m_sName << "(";

                for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                {
                    const auto& _a = _csCall.m_aArgs[_i];

                    _file << _a;

                    if(_i < _csCall.m_aArgs.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << ");\\\n";
            }

            _file << "private: \\\n";
            _file << "MonoObject* m_pCSharpObject;\\\n";
            _file << "MonoClass* m_pCSharpClass;\\\n";
            _file << "void SetCSharpClass(MonoClass* _pClass) override\\\n";
            _file << "{\\\n";
            _file << "m_pCSharpClass = _pClass;\\\n";
            _file << "}\\\n";
            _file << "void SetCSharpObject(MonoObject* _pObject) override\\\n";
            _file << "{\\\n";
            _file << "m_pCSharpObject = _pObject;\\\n";
            _file << "}\\\n";

            std::string _currNamespace;

            for(uint32_t _i = 0; _i < _context->m_aNamespaces.size(); ++_i)
            {
                const auto& _namespace = _context->m_aNamespaces[_i];

                _currNamespace += _namespace;

                if(_i < _context->m_aNamespaces.size() - 1)
                {
                    _currNamespace += ".";
                }
            }

            for(const auto& _internalCall : _context->m_aInternalCalls)
            {
                generate_internal_call(_file, _internalCall);
            }

            _file << "void InternalInit() override;\n";
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

            _file << "#include \"mono/jit/jit.h\"\n";
        	_file << "#include \"mono/metadata/assembly.h\"\n";

            std::string _namespace;

            for(const auto& _n : _context->m_aNamespaces)
            {
                _namespace += _n + "::";
            }

            std::string _csNamespace;

            for(uint32_t _i = 0; _i < _context->m_aNamespaces.size(); ++_i)
            {
                const auto& _namespace = _context->m_aNamespaces[_i];

                _csNamespace += _namespace;

                if(_i < _context->m_aNamespaces.size() - 1)
                {
                    _csNamespace += ".";
                }
            }

            for(const auto& _var : _context->m_aVariables)
            {
                if(_var.m_sName.find_first_of("_an_") == std::string::npos)
                {
	                continue;
                }

                std::string _name = Utils::replace_all(_var.m_sName, "_an_", "");

                _file << "void " << _namespace << _name << "(" << _var.m_sType << " _v) { " << _var.m_sName << " = _v; mono_field_set_value(m_pCSharpObject, m_pCSharpField_" << _name << ", (void*)&_v); }\n";
                _file << _var.m_sType << " " << _namespace << _name << "() const { return " << _var.m_sName << "; }\n";
            }

            for(const auto& _csCall : _context->m_aCSharpCalls)
            {
                _file << _csCall.m_sReturnType << " " << _namespace << "generated_csharp_" << _csCall.m_sName << "(";

                for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                {
                    const auto& _a = _csCall.m_aArgs[_i];

                    _file << _a;

                    if(_i < _csCall.m_aArgs.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << ")\n";
                _file << "{\n";

                if(_csCall.m_aArgs.size() == 0)
                {
                    _file << "mono_runtime_invoke(m_pCSharpMethod_" << _csCall.m_sName << ", m_pCSharpObject, nullptr, nullptr);\n";
                }
                else
                {
                    _file << "void* _params[" << _csCall.m_aArgs.size() << "];\n";

                    for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                    {
                        const auto& _a = _csCall.m_aArgs[_i];

                        _file << "_params[" << _i << "] = &" << _a.c_str() + _a.find(' ') << ";\n";
                    }

                    _file << "mono_runtime_invoke(m_pCSharpMethod_" << _csCall.m_sName << ", m_pCSharpObject, _params, nullptr);\n";
                }

                _file << "}\n";

                _file << _csCall.m_sReturnType << " " << _namespace << "generated_csharp_" << _csCall.m_sName << "(duckvil_cs_" << _csCall.m_sName << " _args)\n";
                _file << "{\n";
                _file << "generated_csharp_" << _csCall.m_sName << "(";

                for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                {
                    const auto& _a = _csCall.m_aArgs[_i];

                    _file << "_args." << _a.c_str() + _a.find(' ');

                    if(_i < _csCall.m_aArgs.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << ");\n";
                _file << "}\n";

                _file << _csCall.m_sReturnType << " " << _namespace << _csCall.m_sName << "(";

                for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                {
                    const auto& _a = _csCall.m_aArgs[_i];

                    _file << _a;

                    if(_i < _csCall.m_aArgs.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << ")\n";
                _file << "{\n";
                _file << "generated_csharp_" << _csCall.m_sName << "({ ";

                for(uint32_t _i = 0; _i < _csCall.m_aArgs.size(); ++_i)
                {
                    const auto& _a = _csCall.m_aArgs[_i];

                    _file << _a.c_str() + _a.find(' ');

                    if(_i < _csCall.m_aArgs.size() - 1)
                    {
                        _file << ", ";
                    }
                }

                _file << " });\n";
                _file << "}\n";
            }

            for(const auto& _internalCall : _context->m_aInternalCalls)
            {
                generate_internal_call(_file, _internalCall, "\n", Utils::lambda([&](std::ofstream& _b)
                {
                    _b << "auto _csEngine = Duckvil::CSharp::CSharp::GetScriptEngine();\n";
                    _b << _context->m_sClassName << "* _script = static_cast<" << _context->m_sClassName  << "*>(_csEngine->GetSharedScript(_ullSharedScriptID));\n";
                    _b << "_script->" << _internalCall.m_sName << "(";
                    
                    for(uint32_t _i = 0; _i < _internalCall.m_aArgs.size(); ++_i)
                    {
                        const auto& _arg = _internalCall.m_aArgs[_i];

                        _b << _arg.m_sName;

                        if(_i < _internalCall.m_aArgs.size() - 1)
                        {
                            _b << ", ";
                        }
                    }

                    _b << ");\n";
                }));
            }

            _file << "void " << _namespace << "InternalInit()\n";
            _file << "{\n";

            for(const auto& _internalCall : _context->m_aInternalCalls)
            {
                _file << "mono_add_internal_call(\"" << _csNamespace << "::" << _internalCall.m_sName << "\", csharp_internal_" << _internalCall.m_sName << ");\n";
            }

            for(const auto& _csCall : _context->m_aCSharpCalls)
            {
                _file << "m_pCSharpMethod_" << _csCall.m_sName << " = mono_class_get_method_from_name(m_pCSharpClass, \"" << _csCall.m_sName << "\", " << _csCall.m_aArgs.size() << ");\n";
            }

            for(const auto& _varialbe : _context->m_aVariables)
            {
                std::string _name = Utils::replace_all(_varialbe.m_sName, "_an_", "");

                _file << "m_pCSharpField_" << _name << " = mono_class_get_field_from_name(m_pCSharpClass, \"" << _varialbe.m_sName << "\");\n";
                _file << "mono_add_internal_call(\"" << _csNamespace << "::VariableChanged_" << _name << "\", csharp_internal_VariableChanged_" << _name << ");\n";
            }

            _file << "}\n";
        }
    }

}}
