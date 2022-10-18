#pragma once

#include "RuntimeReflectionStatic/local/type.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) get_type(const std::size_t& _ullTypeID);

    template <typename Type>
    static inline DUCKVIL_RESOURCE(type_t) get_type()
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type<Type>(_context.m_pReflection, _context.m_pReflectionData);
    }

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(type_t) get_type(const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _sName);
    }

    template <std::size_t Length, std::size_t... Lengths>
    static DUCKVIL_RESOURCE(type_t) get_type(const char (&_sName)[Length], const char (&..._sNamespaces)[Lengths])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type<Length, Lengths...>(_context.m_pReflectionData, _sName, _sNamespaces...);
    }

    DUCKVIL_RESOURCE(type_t) get_type(const char* _sName, const std::vector<const char*>& _aNamespaces);

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(const Memory::FreeList& _heap);

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types();

    const __type_t& get_type(DUCKVIL_RESOURCE(type_t) _typeHandle);

    template <typename Type>
    static inline bool inherits(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return inherits<Type>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle);
    }

    DUCKVIL_RESOURCE(ntype_t) get_type(const std::vector<const char*>& _aNamespaces);

    const __ntype_t& get_type(DUCKVIL_RESOURCE(ntype_t) _typeHandle);

}}