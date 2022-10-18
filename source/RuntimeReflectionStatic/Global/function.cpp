#pragma once

#include "RuntimeReflectionStatic/global/function.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetFunctions(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_functions(_heapContext.m_heap, _typeHandle);
    }

    const __function_t& get_function(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle);
    }

}}