#pragma once

#include "RuntimeReflectionStatic/global/object.h"

namespace Duckvil { namespace RuntimeReflection {

    void destroy(const Memory::FreeList& _memory, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        destroy(_memory.GetMemoryInterface(), _memory.GetAllocator(), _context.m_pReflection, _context.m_pReflectionData, _typeHandle, _bTracked, _pObject);
    }

}}