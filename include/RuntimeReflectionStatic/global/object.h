#pragma once

#include "RuntimeReflectionStatic/local/object.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    void destroy(const Memory::FreeList& _memory, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject);

    template <size_t Length, typename... Args>
    static inline void* create(const Memory::FreeList& _memory, const char _sTypeName[Length], bool _bTracked, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _context.m_pReflection, _context.m_pReflectionData, _sTypeName, _bTracked, _vArgs...);
    }

    template <typename... Args>
    static inline void* create(const Memory::FreeList& _memory, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _context.m_pReflection, _context.m_pReflectionData, _typeHandle, _bTracked, _vArgs...);
    }

}}