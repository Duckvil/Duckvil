#pragma once

#include "RuntimeReflectionStatic/local/property.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type, std::size_t Length>
    static inline void* get_property(const char (&_sName)[Length], const Type* _pObject)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_property<Type>(_context.m_pReflection, _context.m_pReflectionData, _sName, _pObject);
    }

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(property_t) get_property(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_property(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    const __property_t& get_property(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle);

    Memory::Vector<DUCKVIL_RESOURCE(property_t)> get_properties(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle);

}}