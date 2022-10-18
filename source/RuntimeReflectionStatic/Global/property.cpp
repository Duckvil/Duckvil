#pragma once

#include "RuntimeReflectionStatic/global/property.h"

namespace Duckvil { namespace RuntimeReflection {

    const __property_t& get_property(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetPropertyByHandle(_context.m_pReflectionData, _typeHandle, _propertyHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(property_t)> get_properties(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetProperties(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

}}