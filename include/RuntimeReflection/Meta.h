#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#undef max
#endif

namespace Duckvil { namespace RuntimeReflection {

    template <typename KeyType>
    static DUCKVIL_RESOURCE(variant_t) get_meta_value_handle(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const KeyType& _key)
    {
        return _pReflection->m_fnGetTypeMetaHandle(_pData, _typeHandle, &_key, sizeof(KeyType), typeid(KeyType).hash_code());
    }

    template <typename ValueType, typename KeyType>
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const KeyType& _key)
    {
        return *(ValueType*)_pReflection->m_fnGetTypeMetaValue(_pData, _handle, &_key, sizeof(KeyType), typeid(KeyType).hash_code());
    }

    template <typename KeyType>
    static inline __variant get_meta(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const KeyType& _key)
    {
        return _pReflection->m_fnGetTypeMetaVariant(_pData, _handle, &_key, sizeof(KeyType), typeid(KeyType).hash_code());
    }

    template <typename ValueType, std::size_t Length>
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const char (&_key)[Length])
    {
        return *(ValueType*)_pReflection->m_fnGetTypeMetaValue(_pData, _handle, _key, Length, CONST_CHAR_POINTER_ID);
    }

    template <typename ValueType, typename KeyType>
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle, const KeyType& _key)
    {
        return *(ValueType*)_pReflection->m_fnGetPropertyMetaValue(_pData, _typeHandle, _handle, &_key, sizeof(KeyType), typeid(KeyType).hash_code());
    }

    template <typename ValueType, std::size_t Length>
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle, const char (&_key)[Length])
    {
        return *(ValueType*)_pReflection->m_fnGetPropertyMetaValue(_pData, _typeHandle, _handle, _key, Length, CONST_CHAR_POINTER_ID);
    }

    template <typename ValueType, typename KeyType>
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle, const KeyType& _key)
    {
        return *(ValueType*)_pReflection->m_fnGetConstructorMetaValue(_pData, _typeHandle, _handle, &_key, sizeof(KeyType), typeid(KeyType).hash_code());
    }

    template <typename ValueType, std::size_t Length>
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle, const char (&_key)[Length])
    {
        return *(ValueType*)_pReflection->m_fnGetConstructorMetaValue(_pData, _typeHandle, _handle, _key, Length, CONST_CHAR_POINTER_ID);
    }

}}