#pragma once

#include "RuntimeReflectionStatic/local/enum.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(enum_t) get_enum_handle(DUCKVIL_RESOURCE(ntype_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum_handle(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    DUCKVIL_RESOURCE(enum_t) get_enum_handle(DUCKVIL_RESOURCE(ntype_t) _typeHandle, std::size_t _ullTypeID);

    template <typename Type>
    static inline DUCKVIL_RESOURCE(enum_t) get_enum_handle(DUCKVIL_RESOURCE(ntype_t) _typeHandle)
    {
        return get_enum_handle(_typeHandle, typeid(Type).hash_code());
    }

    const __enum_t& get_enum(DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle);

    // TODO: Not sure
    const __enum_t& get_enum(size_t _ullTypeID, DUCKVIL_RESOURCE(ntype_t)* _pTypeHandle = nullptr, DUCKVIL_RESOURCE(enum_t)* _pHandle = nullptr);

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(enum_element_t) get_enum_element_handle(DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum_element_handle(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _handle, _sName);
    }

    template <typename Type>
    static inline DUCKVIL_RESOURCE(enum_element_t) get_enum_element_handle(DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle, const Type& _value)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum_element_handle(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _handle, _value);
    }

    const __enum_element_t& get_enum_element(DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _enumHandle, DUCKVIL_RESOURCE(enum_element_t) _handle);

}}