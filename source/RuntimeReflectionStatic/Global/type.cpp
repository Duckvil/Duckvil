#pragma once

#include "RuntimeReflectionStatic/global/type.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) get_type(const std::size_t& _ullTypeID)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _ullTypeID);
    }

    DUCKVIL_RESOURCE(type_t) get_type(const char* _sName, const std::vector<const char*>& _aNamespaces)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflectionData, _sName, _aNamespaces);
    }

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(const Memory::FreeList& _heap)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_types(_context.m_pReflection, _context.m_pReflectionData, _heap);
    }

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types()
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_types(_heapContext.m_heap);
    }

    const __type_t& get_type(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _typeHandle);
    }

    DUCKVIL_RESOURCE(ntype_t) get_type(const std::vector<const char*>& _aNamespaces)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _aNamespaces);
    }

    const __ntype_t& get_type(DUCKVIL_RESOURCE(ntype_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _typeHandle);
    }

}}