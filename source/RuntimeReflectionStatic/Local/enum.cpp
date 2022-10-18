#pragma once

#include "RuntimeReflectionStatic/local/enum.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(enum_t) get_enum_handle(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, std::size_t _ullTypeID)
    {
        return _pReflection->m_fnGetNEnumHandleByTypeID(_pData, _typeHandle, _ullTypeID);
    }

    const __enum_t& get_enum(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle)
    {
        return _pReflection->m_fnGetNEnumByHandle(_pData, _typeHandle, _handle);
    }

    // TODO: Not sure
    const __enum_t& get_enum(__ftable* _pReflection, __data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(ntype_t)* _pTypeHandle, DUCKVIL_RESOURCE(enum_t)* _pHandle)
    {
        return _pReflection->m_fnGetNEnumByTypeID(_pData, _ullTypeID, _pTypeHandle, _pHandle);
    }

    const __enum_element_t& get_enum_element(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _enumHandle, DUCKVIL_RESOURCE(enum_element_t) _handle)
    {
        return _pReflection->m_fnGetNEnumElementByHandle(_pData, _typeHandle, _enumHandle, _handle);
    }

}}