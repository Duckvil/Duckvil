#pragma once

#include "RuntimeReflectionStatic/local/function.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle);

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(DUCKVIL_RESOURCE(type_t) _typeHandle);

    template <typename... Args, std::size_t Length>
    static inline DUCKVIL_RESOURCE(function_t) get_function_handle(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_handle<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    template <typename Type, typename... Args>
    static inline DUCKVIL_RESOURCE(function_t) get_function_handle(void (Type::*_fnCallback)(Args...))
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_handle<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _fnCallback);
    }

    template <typename Type, typename... Args>
    static inline DUCKVIL_RESOURCE(function_t) get_function_handle(void (*_fnCallback)(Args...))
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_handle<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _fnCallback);
    }

// TODO: Check it
    template <typename... Args, std::size_t Length>
    static inline __function<void(*)(Args...)>* get_static_function_callback(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_static_function_handle<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    template <typename Type, typename... Args, std::size_t Length>
    static inline __function<void(Type::*)(Args...)>* get_function_callback_m(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_callback_m<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    template <typename Type, typename... Args>
    static inline __function<void(Type::*)(Args...)>* get_function_callback_m(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_callback_m<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline __function<ReturnType(Type::*)(Args...)>* get_function_callback_mr(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_callback_mr<ReturnType, Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    const __function_t& get_function(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);

}}