#pragma once

#include "RuntimeReflectionStatic/global/argument.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_arguments(_context.m_pReflection, _context.m_pReflectionData, _heap, _typeHandle, _constructorHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_arguments(_heapContext.m_heap, _typeHandle, _constructorHandle);
    }

    __argument_t get_argument(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_argument(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _constructorHandle, _argumentHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_arguments(_context.m_pReflection, _context.m_pReflectionData, _heap, _typeHandle, _functionHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_arguments(_heapContext.m_heap, _typeHandle, _functionHandle);
    }

    const __argument_t& get_argument(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_argument(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _argumentHandle);
    }

}}