#pragma once

#include "RuntimeReflectionStatic/local/argument.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type, typename... Args, std::size_t Length>
    static inline void invoke(const char (&_sName)[Length], Type* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        invoke<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _sName, _pObject, _vArgs...);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline ReturnType invoke(const char (&_sName)[Length], Type* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return invoke<ReturnType, Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _sName, _pObject, _vArgs...);
    }

    template <typename... Args>
    static inline void invoke_member(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        invoke_member<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _pObject, _vArgs...);
    }

    template <typename ReturnType, typename... Args>
    static inline ReturnType invoke_member_result(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return invoke_member_result<ReturnType, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _pObject, _vArgs...);
    }

}}