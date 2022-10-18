#pragma once

#include "RuntimeReflectionStatic/global/variant.h"

namespace Duckvil { namespace RuntimeReflection {

    const __variant_t& get_variant_key(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_variant_key(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _handle);
    }

    const __variant_t& get_variant_value(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_variant_value(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _handle);
    }

}}