#pragma once

#include "RuntimeReflectionStatic/local/constructor.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename... Args>
    static inline DUCKVIL_RESOURCE(constructor_t) get_constructor_handle(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_constructor_handle<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle);
    }

    const __constructor_t& get_constructor(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);
    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle);
    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(DUCKVIL_RESOURCE(type_t) _typeHandle);

}}