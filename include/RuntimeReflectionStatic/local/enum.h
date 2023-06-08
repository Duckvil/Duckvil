#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    template <std::size_t Length>
    DUCKVIL_RESOURCE(enum_t) get_enum_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetNEnumHandleByName(_pData, _typeHandle, _sName, Length);
    }

    DUCKVIL_RESOURCE(enum_t) get_enum_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, std::size_t _ullTypeID);

    template <typename Type>
    DUCKVIL_RESOURCE(enum_t) get_enum_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle)
    {
        // return _pReflection->m_fnGetNEnumHandleByTypeID(_pData, _typeHandle, typeid(Type).hash_code());
        return get_enum_handle(_pReflection, _pData, _typeHandle, typeid(Type).hash_code());
    }

    const __enum_t& get_enum(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle);

    // TODO: Not sure
    const __enum_t& get_enum(const __ftable* _pReflection, __data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(ntype_t)* _pTypeHandle = nullptr, DUCKVIL_RESOURCE(enum_t)* _pHandle = nullptr);

    template <std::size_t Length>
    DUCKVIL_RESOURCE(enum_element_t) get_enum_element_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetNEnumElementHandleByName(_pData, _typeHandle, _handle, _sName, Length);
    }

    template <typename Type>
    DUCKVIL_RESOURCE(enum_element_t) get_enum_element_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle, const Type& _value)
    {
        return _pReflection->m_fnGetNEnumElementHandleByValue(_pData, _typeHandle, _handle, &_value, sizeof(Type));
    }

    const __enum_element_t& get_enum_element(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _enumHandle, DUCKVIL_RESOURCE(enum_element_t) _handle);

}}