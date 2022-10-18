#pragma once

#include "RuntimeReflectionStatic/global/enum.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(enum_t) get_enum_handle(DUCKVIL_RESOURCE(ntype_t) _typeHandle, std::size_t _ullTypeID)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum_handle(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _ullTypeID);
    }

    const __enum_t& get_enum(DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _handle);
    }

    // TODO: Not sure
    const __enum_t& get_enum(size_t _ullTypeID, DUCKVIL_RESOURCE(ntype_t)* _pTypeHandle, DUCKVIL_RESOURCE(enum_t)* _pHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum(_context.m_pReflection, _context.m_pReflectionData, _ullTypeID, _pTypeHandle, _pHandle);
    }

    const __enum_element_t& get_enum_element(DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _enumHandle, DUCKVIL_RESOURCE(enum_element_t) _handle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_enum_element(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _enumHandle, _handle);
    }

}}