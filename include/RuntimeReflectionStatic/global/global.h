#pragma once

#include "RuntimeReflection/Resources.h"

inline duckvil_frontend_reflection_context g_duckvilFrontendReflectionContext;

namespace Duckvil { namespace RuntimeReflection {

    static inline void make_current(const duckvil_frontend_reflection_context& _context)
    {
        g_duckvilFrontendReflectionContext = _context;
    }

    static inline const duckvil_frontend_reflection_context& get_current()
    {
        return g_duckvilFrontendReflectionContext;
    }

}}