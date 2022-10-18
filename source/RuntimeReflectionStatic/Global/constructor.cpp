#pragma once

#include "RuntimeReflectionStatic/global/constructor.h"

namespace Duckvil { namespace RuntimeReflection {

    const __constructor_t& get_constructor(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_constructor(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _constructorHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructors(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_constructors(_heapContext.m_heap, _typeHandle);
    }

}}