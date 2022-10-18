#pragma once

#include "RuntimeReflection/Resources.h"

#include <vector>

namespace Duckvil { namespace RuntimeReflection {

    static inline Utils::string combine(const __type_t& _type)
    {
        Utils::string _res("");

        for(uint32_t _i = 0; _i < DUCKVIL_SLOT_ARRAY_SIZE(_type.m_namespaces); _i++)
        {
            _res = _res + DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, _i).m_sNamespaceName;
            _res = _res + "::";
        }

        _res = _res + _type.m_sTypeName;

        return _res;
    }

    template <std::size_t Length>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const char(&_sName)[Length])
    {
        if(!_bRes)
        {
            return;
        }

        const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_typeNamespaces, _uiIndex);

        if(strcmp(_namespace.m_sNamespaceName, _sName) != 0)
        {
            _bRes = false;
        }

        _uiIndex++;
    }

    template <typename Arg>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const Arg& _arg)
    {
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _arg);
    }

    template <typename Arg, typename... Args>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const Arg& _arg, const Args&... _args)
    {
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _arg);
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _args...);
    }

}}