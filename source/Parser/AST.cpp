#include "Parser/AST.h"

#include <iostream>
#include <stack>
#include <queue>

#include "cppast/cpp_enum.hpp"
#include "cppast/cpp_member_function.hpp"
#include "cppast/cpp_member_variable.hpp"
#include "cppast/cpp_namespace.hpp"
#include "cppast/cpp_variable.hpp"
#include "cppast/cpp_preprocessor.hpp"
#include "cppast/cpp_type_alias.hpp"

#include "Utils/Utils.h"

#include "Parser/parser.h"

#include "cppast/visitor.hpp"
#include "cppast/cpp_template.hpp"

#define DUCKVIL_SPACES \
    for(uint32_t i = 0; i < _uiScope; ++i) \
    { \
        printf(" "); \
    }

namespace Duckvil { namespace Parser {

    void add_scope(__ast* _pAST, __ast_entity* _pScope)
    {
        _pAST->m_pCurrentScope->m_aScopes.push_back(_pScope);

        _pScope->m_pParentScope = _pAST->m_pCurrentScope;

        _pAST->m_pCurrentScope = _pScope;
    }

    void process_attrs(const cppast::cpp_attribute_list& _attrs, __ast_entity* _pScope)
    {
        for(auto _attr : _attrs)
        {
            auto args = _attr.arguments().value();
            uint32_t _bracketsCount = 0;
            std::string _curr;
            __ast_meta _meta;

            for(auto arg : args)
            {
                if(arg.spelling == "(")
                {
                    _bracketsCount++;
                }
                else if(arg.spelling == ")")
                {
                    _bracketsCount--;
                }
                else if(arg.spelling == "=")
                {
                    _meta.m_sKey = _curr;

                    _curr.clear();

                    continue;
                }
                else if(_bracketsCount == 0 && arg.spelling == ",")
                {
                    if(_meta.m_sKey.empty())
                    {
                        _meta.m_sKey = _curr;

                        _curr.clear();
                    }

                    if(_curr.empty())
                    {
                        _curr = "true";
                    }

                    _meta.m_sValue = _curr;

                    _curr.clear();

                    _pScope->m_aMeta.push_back(_meta);

                    _meta = {};

                    continue;
                }

                _curr += arg.spelling;
            }

            if(_meta.m_sKey.empty() && _curr.empty())
            {
                continue;
            }
            else if(!_meta.m_sKey.empty() && _curr.empty())
            {
                _meta.m_sValue = "true";
            }
            else if(_meta.m_sKey.empty() && !_curr.empty())
            {
                _meta.m_sKey = _curr;

                _curr.clear();

                _meta.m_sValue = "true";
            }
            else if(!_meta.m_sKey.empty() && !_curr.empty())
            {
                _meta.m_sValue = _curr;

                _curr.clear();
            }

            /*if(_meta.m_sKey.empty())
            {
                continue;
            }

            if(!_curr.empty())
            {
                _meta.m_sValue = _curr;

                _curr.clear();
            }

            if(_meta.m_sValue.empty())
            {
                _meta.m_sValue = "true";
            }*/

            _pScope->m_aMeta.push_back(_meta);
        }
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_namespace* _pScope, const cppast::cpp_namespace& _entity, cppast::cpp_access_specifier_kind _access)
    {
        _pScope->m_sName = _entity.name();

        return _pScope;
    }

    std::string resolve_namespaces(const cppast::cpp_member_variable& _entity, const cppast::cpp_entity_index& _index)
    {
        std::string _namespaces = "";

        if(_entity.type().kind() == cppast::cpp_type_kind::user_defined_t)
        {
            const auto& _ut = static_cast<const cppast::cpp_user_defined_type&>(_entity.type());
            type_safe::optional_ref<const cppast::cpp_entity> _e;
            std::vector<std::string> _scopes;

            if (_ut.entity().id().size() > 1)
            {
                printf("AAA\n");
            }

            for(auto _id : _ut.entity().id())
            {
                auto a = _index.lookup(_id);

                if(a.has_value())
                {
                    _scopes.push_back(a.value().name());

                    _e = a.value().parent();

                    break;
                }
            }

            while(_e.has_value())
            {
                if(_e.value().kind() == cppast::cpp_entity_kind::file_t)
                {
                    break;
                }

                _scopes.push_back(_e.value().name());

                _e = _e.value().parent();
            }

            while(!_scopes.empty())
            {
                _namespaces += _scopes.back();

                _scopes.pop_back();

                if(!_scopes.empty())
                {
                    _namespaces += "::";
                }
            }
        }

        return _namespaces;
    }

    std::string resolve_namespaces(const cppast::cpp_variable& _entity, const cppast::cpp_entity_index& _index)
    {
        std::string _namespaces;

        if(_entity.type().kind() == cppast::cpp_type_kind::user_defined_t)
        {
            const auto& _ut = static_cast<const cppast::cpp_user_defined_type&>(_entity.type());
            type_safe::optional_ref<const cppast::cpp_entity> _e;
            std::vector<std::string> _scopes;

            for(auto _id : _ut.entity().id())
            {
                auto a = _index.lookup(_id);

                if(a.has_value())
                {
                    _e = a.value().parent();

                    break;
                }
            }

            while(_e.has_value())
            {
                if(_e.value().kind() == cppast::cpp_entity_kind::file_t)
                {
                    break;
                }

                _scopes.push_back(_e.value().name());

                _e = _e.value().parent();
            }

            while(!_scopes.empty())
            {
                _namespaces += _scopes.back() + "::";

                _scopes.pop_back();
            }
        }

        return _namespaces;
    }

    std::string resolve_namespaces(const cppast::cpp_type& _type, const cppast::cpp_entity_index& _index)
    {
        std::string _namespaces;

        const auto& _ut = static_cast<const cppast::cpp_user_defined_type&>(_type);
        type_safe::optional_ref<const cppast::cpp_entity> _e;
        std::vector<std::string> _scopes;

        for (auto _id : _ut.entity().id())
        {
            auto a = _index.lookup(_id);

            if (a.has_value())
            {
                _e = a.value().parent();

                break;
            }
        }

        while (_e.has_value())
        {
            if (_e.value().kind() == cppast::cpp_entity_kind::file_t)
            {
                break;
            }

            _scopes.push_back(_e.value().name());

            _e = _e.value().parent();
        }

        while (!_scopes.empty())
        {
            _namespaces += _scopes.back() + "::";

            _scopes.pop_back();
        }

        return _namespaces;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_structure* _pScope, const cppast::cpp_class& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(_entity.name() == "__duckvil_generated_body__")
        {
            _pAST->m_heap.Free(_pScope);

            auto _scope = _pAST->m_heap.Allocate<__ast_entity_define>();

            //_pAST->m_pPendingScope = _scope;

            _scope->m_sName = "DUCKVIL_GENERATED_BODY";

            static_cast<__ast_entity_structure*>(_pAST->m_pCurrentScope)->m_uiGeneratedBodyMacroLine = _entity.get_line();

            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

            return nullptr;
        }
        else if(_entity.name().find("__duckvil_csharp_function__") != std::string::npos)
        {
            _pAST->m_heap.Free(_pScope);

            auto _scope = _pAST->m_heap.Allocate<__ast_entity_define>();

            //_pAST->m_pPendingScope = _scope;

            _scope->m_sName = "DUCKVIL_CS_FUNCTION";

            auto _attrs = _entity.attributes();

            for (auto _attr : _attrs)
            {
                auto args = _attr.arguments().value();
                uint32_t _bracketsCount = 0;
                uint32_t _sharpBracketsCount = 0;
                std::string _curr;

                for (auto arg : args)
                {
                    if (arg.spelling == "(")
                    {
                        _bracketsCount++;
                    }
                    else if (arg.spelling == ")")
                    {
                        _bracketsCount--;
                    }
                    if (arg.spelling == "<")
                    {
                        _sharpBracketsCount++;
                    }
                    else if (arg.spelling == ">")
                    {
                        _sharpBracketsCount--;
                    }
                    else if (_bracketsCount == 0 && _sharpBracketsCount == 0 && arg.spelling == ",")
                    {
                        _scope->m_aArguments.push_back(_curr);

                        _curr.clear();

                        continue;
                    }

                    _curr += arg.spelling;
                }

                if (!_curr.empty())
                {
                    _curr = std::string(++_curr.begin(), --_curr.end()) ;

                    _scope->m_aArguments.push_back(_curr);

                    _curr.clear();
                }
            }


        	//_scope->m_aArguments

            //static_cast<__ast_entity_structure*>(_pAST->m_pCurrentScope)->m_uiGeneratedBodyMacroLine = _entity.get_line();

            _pAST->m_pCurrentScope->m_aScopes.push_back(_scope);

            return nullptr;
        }

    	if(_pAST->m_pPendingScope == nullptr && _entity.name().starts_with("__duckvil_class_"))
        {
            _pAST->m_heap.Free(_pScope);
            
            _pAST->m_pPendingScope = _pAST->m_heap.Allocate<__ast_entity_structure>(__ast_structure_type::__ast_structure_type_class);
            
            process_attrs(_entity.attributes(), _pAST->m_pPendingScope);

            return nullptr;
        }
        else if (_pAST->m_pPendingScope == nullptr && _entity.name().starts_with("__duckvil_struct_"))
        {
            _pAST->m_heap.Free(_pScope);

            _pAST->m_pPendingScope = _pAST->m_heap.Allocate<__ast_entity_structure>(__ast_structure_type::__ast_structure_type_struct);

            process_attrs(_entity.attributes(), _pAST->m_pPendingScope);

            return nullptr;
        }

        if(_pAST->m_pPendingScope != nullptr)
        {
            _pAST->m_heap.Free(_pScope);

            _pScope = static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        _pScope->m_sName = _entity.name();

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        if (!cppast::is_definition(_entity))
        {
            _pScope->m_bIsForwardDeclaration = true;

            _pAST->m_pCurrentScope->m_aScopes.push_back(_pScope);

            return nullptr;
        }
        else
        {
            _pScope->m_bIsForwardDeclaration = false;
        }

        for(const auto& _base : _entity.bases())
        {
            __ast_inheritance _inh;

            _inh.m_sName = _base.name();
            
            switch(_base.access_specifier())
            {
            case cppast::cpp_public:
                _inh.m_protection = __ast_access::__ast_access_public;
                break;
            case cppast::cpp_protected:
                _inh.m_protection = __ast_access::__ast_access_protected;
                break;
            case cppast::cpp_private:
                _inh.m_protection = __ast_access::__ast_access_private;
                break;
            default:
                break;
            }

            _pScope->m_aInheritance.push_back(_inh);
        }

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_function* _pScope, const cppast::cpp_member_function& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        if(_pAST->m_pPendingScope != nullptr)
        {
            _pAST->m_heap.Free(_pScope);

            _pScope = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        _pScope->m_sName = _entity.name();
        _pScope->m_sReturnType = to_string(_entity.return_type());

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_function* _pScope, const cppast::cpp_function& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope != nullptr)
        {
            _pAST->m_heap.Free(_pScope);

            _pScope = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        _pScope->m_sName = _entity.name();
        _pScope->m_sReturnType = to_string(_entity.return_type());
        _pScope->m_flags = __ast_flags::__ast_flags_static;

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());
            // TODO:
            //_eArg.m_sValue = _arg.default_value().has_value() ? _arg.default_value() : "";

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_variable* _pScope, const cppast::cpp_member_variable& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        _pScope->m_sName = _entity.name();
        _pScope->m_flags = !_entity.is_mutable() ? static_cast<__ast_flags>(static_cast<uint8_t>(_pScope->m_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_const)) : _pScope->m_flags;

        std::string _namespaces = resolve_namespaces(_entity, _index);

        if(_namespaces.empty())
        {
            _namespaces = to_string(_entity.type());
        }

        _pScope->m_sType = _namespaces/* + to_string(_entity.type())*/;

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_variable* _pScope, const cppast::cpp_variable& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        _pScope->m_sName = _entity.name();
        _pScope->m_flags = !_entity.is_constexpr() ? static_cast<__ast_flags>(static_cast<uint8_t>(_pScope->m_flags) | static_cast<uint8_t>(__ast_flags::__ast_flags_const)) : _pScope->m_flags;
        
        std::string _namespaces = resolve_namespaces(_entity, _index);

        if(_namespaces.empty())
        {
            _namespaces = to_string(_entity.type());
        }

        _pScope->m_sType = _namespaces/* + to_string(_entity.type())*/;

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_enum* _pScope, const cppast::cpp_enum& _entity, cppast::cpp_access_specifier_kind _access)
    {
        _pScope->m_sName = _entity.name();

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_enum_value& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(!_pAST->m_pCurrentScope || _pAST->m_pCurrentScope->m_scopeType != __ast_entity_type::__ast_entity_type_enum)
        {
            throw std::exception();
        }

        auto _scope = static_cast<__ast_entity_enum*>(_pAST->m_pCurrentScope);

        _scope->m_aElements.push_back(_entity.name());

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_template_specialization& _entity, cppast::cpp_entity_kind _kind, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope == nullptr)
        {
            if(_kind == cppast::cpp_entity_kind::class_template_t || _kind == cppast::cpp_entity_kind::class_template_specialization_t)
            {
                _pAST->m_pPendingScope = _pAST->m_heap.Allocate<__ast_entity_structure>(__ast_structure_type::__ast_structure_type_class);
            }
            else if(_kind == cppast::cpp_entity_kind::function_template_t || _kind == cppast::cpp_entity_kind::function_template_specialization_t)
            {
                _pAST->m_pPendingScope = _pAST->m_heap.Allocate<__ast_entity_function>();
            }
            else
            {
                throw std::exception();
            }
        }

        if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
        {
            static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(__ast_template());
        }
        else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        {
            static_cast<__ast_entity_function*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(__ast_template());
        }

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_template& _entity, cppast::cpp_entity_kind _kind, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope == nullptr)
        {
            if(_kind == cppast::cpp_entity_kind::class_template_t)
            {
                _pAST->m_pPendingScope = _pAST->m_heap.Allocate<__ast_entity_structure>(__ast_structure_type::__ast_structure_type_class);
            }
            else if(_kind == cppast::cpp_entity_kind::function_template_t)
            {
                _pAST->m_pPendingScope = _pAST->m_heap.Allocate<__ast_entity_function>();
            }
            else
            {
                throw std::exception();
            }
        }

        auto _templ = __ast_template();

        if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_structure)
        {
            static_cast<__ast_entity_structure*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(_templ);
        }
        else if(_pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        {
            static_cast<__ast_entity_function*>(_pAST->m_pPendingScope)->m_aTemplates.push_back(_templ);
        }

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_constructor* _pScope, const cppast::cpp_constructor& _entity, cppast::cpp_access_specifier_kind _access)
    {
        if(_pAST->m_pPendingScope != nullptr && _pAST->m_pPendingScope->m_scopeType == __ast_entity_type::__ast_entity_type_function)
        {
            auto _f = static_cast<__ast_entity_function*>(_pAST->m_pPendingScope);

            _pScope->m_aTemplates.insert(_pScope->m_aTemplates.begin(), _f->m_aTemplates.begin(), _f->m_aTemplates.end());

            _pAST->m_heap.Free(_pAST->m_pPendingScope);

            _pAST->m_pPendingScope = nullptr;
        }

        if(_entity.body_kind() == cppast::cpp_function_deleted)
        {
            return nullptr;
        }

        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());
            // TODO:
            //_eArg.m_sValue = _arg.default_value().has_value() ? _arg.default_value() : "";

            process_attrs(_arg.attributes(), &_eArg);

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, __ast_entity_destructor* _pScope, const cppast::cpp_destructor& _entity, cppast::cpp_access_specifier_kind _access)
    {
        switch(_access)
        {
        case cppast::cpp_public:
            _pScope->m_accessLevel = __ast_access::__ast_access_public;
            break;
        case cppast::cpp_protected:
            _pScope->m_accessLevel = __ast_access::__ast_access_protected;
            break;
        case cppast::cpp_private:
            _pScope->m_accessLevel = __ast_access::__ast_access_private;
            break;
        default:
            break;
        }

        for(const auto& _arg : _entity.parameters())
        {
            __ast_entity_argument _eArg;

            _eArg.m_sName = _arg.name();
            _eArg.m_sType = to_string(_arg.type());
            // TODO:
            //_eArg.m_sValue = _arg.default_value().has_value() ? _arg.default_value() : "";

            _pScope->m_aArguments.push_back(_eArg);
        }

        process_attrs(_entity.attributes(), _pScope);

        return _pScope;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_namespace_alias& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        auto _scope = _pAST->m_heap.Allocate<__ast_entity_namespace_alias>();

        _scope->m_sName = _entity.name();
        _scope->m_sTarget = _entity.target().name();
        _scope->m_pParentScope = _pAST->m_pCurrentScope;

        _pAST->m_pCurrentScope->m_aNamepaceAliases.push_back(_scope);

        return nullptr;
    }

    __ast_entity* process_entity(__ast* _pAST, const cppast::cpp_type_alias& _entity, cppast::cpp_access_specifier_kind _access, const cppast::cpp_entity_index& _index)
    {
        auto _a = _entity.underlying_type().kind();

        if (_a != cppast::cpp_type_kind::user_defined_t)
        {
            return nullptr;
        }

        auto _scope = _pAST->m_heap.Allocate<__ast_entity_type_alias>();

        _scope->m_sName = _entity.name();
        auto& x = (cppast::cpp_user_defined_type&)_entity.underlying_type();

        _scope->m_sTarget = x.entity().name();

        _pAST->m_pCurrentScope->m_aTypeAliases.push_back(_scope);

        return nullptr;
    }

    void ast_generate_cppast(__ast* _pAST, const __lexer_ftable* _pLexer, __lexer_data& _lexerData)
    {
        const auto& _config = *static_cast<Parser::compile_config*>(_lexerData.m_pConfig);

        _pAST->m_pCurrentScope = &_pAST->m_main;
        _pAST->m_pPendingScope = nullptr;
        _pAST->m_currentAccess = __ast_access::__ast_access_not_specified;
        _pAST->m_sCurrentDefineNeeded2 = nullptr;

        cppast::cpp_entity_index _index;

        custom<cppast::libclang_parser> _parser(type_safe::ref(_index));

        try
        {
            _parser.parse((_pAST->m_sPath / _pAST->m_sFile).string(), _config);
        }
        catch(const std::exception& ex)
        {
            printf("Exception while parsing: %s\n", ex.what());

            return;
        }

        for(const auto& _file : _parser.files())
        {
            cppast::visit(_file, [&](const cppast::cpp_entity& e, cppast::visitor_info info)
                {
                    if(info.event == cppast::visitor_info::container_entity_exit)
                    {
                        if(e.kind() == cppast::cpp_entity_kind::class_t && !cppast::is_definition(e))
                        {
                            return;
                        }

                        if((e.kind() != cppast::cpp_entity_kind::language_linkage_t &&
                            e.kind() != cppast::cpp_entity_kind::function_template_t &&
                            e.kind() != cppast::cpp_entity_kind::function_template_specialization_t &&
                            _pAST->m_pPendingScope == nullptr &&
                            e.kind() != cppast::cpp_entity_kind::file_t &&
                            e.kind() != cppast::cpp_entity_kind::alias_template_t &&
                            e.kind() != cppast::cpp_entity_kind::class_template_t &&
                            e.kind() != cppast::cpp_entity_kind::class_template_specialization_t &&
                            e.kind() != cppast::cpp_entity_kind::constructor_t &&
                            e.kind() != cppast::cpp_entity_kind::destructor_t))
                        {
                            _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
                        }

                        return;
                    }

                    __ast_entity* _scope = nullptr;

                    switch(e.kind())
                    {
                    case cppast::cpp_entity_kind::class_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_structure>(__ast_structure_type::__ast_structure_type_class), reinterpret_cast<const cppast::cpp_class&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::member_function_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_function>(), reinterpret_cast<const cppast::cpp_member_function&>(e), info.access, _index);
                    } break;
                    case cppast::cpp_entity_kind::function_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_function>(), reinterpret_cast<const cppast::cpp_function&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::namespace_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_namespace>(), reinterpret_cast<const cppast::cpp_namespace&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::member_variable_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_variable>(), reinterpret_cast<const cppast::cpp_member_variable&>(e), info.access, _index);
                    } break;
                    case cppast::cpp_entity_kind::enum_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_enum>(), reinterpret_cast<const cppast::cpp_enum&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::enum_value_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_enum_value&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::class_template_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::class_template_specialization_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template_specialization&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::function_template_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::function_template_specialization_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_template_specialization&>(e), e.kind(), info.access);
                    } break;
                    case cppast::cpp_entity_kind::constructor_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_constructor>(), reinterpret_cast<const cppast::cpp_constructor&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::destructor_t:
                    {
                        _scope = process_entity(_pAST, _pAST->m_heap.Allocate<__ast_entity_destructor>(), reinterpret_cast<const cppast::cpp_destructor&>(e), info.access);
                    } break;
                    case cppast::cpp_entity_kind::namespace_alias_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_namespace_alias&>(e), info.access, _index);
                    } break;
                    case cppast::cpp_entity_kind::type_alias_t:
                    {
                        _scope = process_entity(_pAST, reinterpret_cast<const cppast::cpp_type_alias&>(e), info.access, _index);
                    } break;
                    default:
                        printf("%s\n", e.name().c_str());
                        return;
                    }

                    if(_scope == nullptr)
                    {
                        return;
                    }

            		if(info.event == cppast::visitor_info::container_entity_enter)
                    {
                        add_scope(_pAST, _scope);
                    }
                    else if(info.event == cppast::visitor_info::leaf_entity)
                    {
                        add_scope(_pAST, _scope);

                        _pAST->m_pCurrentScope = _pAST->m_pCurrentScope->m_pParentScope;
                    }
                }
            );
        }
    }

}}

const Duckvil::Parser::__ast_ftable* duckvil_ast_init()
{
    static Duckvil::Parser::__ast_ftable _ftable = { 0 };

    _ftable.ast_generate = &Duckvil::Parser::ast_generate_cppast;

    return &_ftable;
}