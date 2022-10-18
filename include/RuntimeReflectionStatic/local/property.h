#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type, std::size_t Length>
    const void* get_property(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Type* _pObject)
    {
        return _pReflection->m_fnGetPropertyValue(_pData, _sName, Length, typeid(Type).hash_code(), _pObject);
    }

    template <std::size_t Length>
    DUCKVIL_RESOURCE(property_t) get_property(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetPropertyHandleByName(_pData, _typeHandle, _sName, Length);
    }

    const __property_t& get_property(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle);

    Memory::Vector<DUCKVIL_RESOURCE(property_t)> get_properties(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle);

}}