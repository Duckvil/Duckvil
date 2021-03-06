#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#undef max
#endif

#define DUCKVIL_META_CAT(...) __VA_ARGS__

#define DUCKVIL_META_UTIL(name, type, type2) \
    template <typename KeyType> \
    static inline DUCKVIL_RESOURCE(variant_t) get_meta_value_handle(__ftable* _pReflection, __data* _pData, type, const KeyType& _key) \
    { \
        return _pReflection->m_fnGet ## name ## MetaHandle(_pData, type2, &_key, sizeof(KeyType), typeid(KeyType).hash_code()); \
    } \
 \
    template <typename ValueType, typename KeyType> \
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, type, const KeyType& _key) \
    { \
        return *(ValueType*)_pReflection->m_fnGet ## name ## MetaValue(_pData, type2, &_key, sizeof(KeyType), typeid(KeyType).hash_code()); \
    } \
 \
    template <typename KeyType> \
    static inline void* get_meta_value_ptr(__ftable* _pReflection, __data* _pData, type, const KeyType& _key) \
    { \
        return _pReflection->m_fnGet ## name ## MetaValue(_pData, type2, &_key, sizeof(KeyType), typeid(KeyType).hash_code()); \
    } \
 \
    template <typename KeyType> \
    static inline const __variant& get_meta(__ftable* _pReflection, __data* _pData, type, const KeyType& _key) \
    { \
        return _pReflection->m_fnGet ## name ## MetaVariant(_pData, type2, &_key, sizeof(KeyType), typeid(KeyType).hash_code()); \
    } \
 \
    template <typename ValueType, std::size_t Length> \
    static inline const ValueType& get_meta_value(__ftable* _pReflection, __data* _pData, type, const char (&_key)[Length]) \
    { \
        return *(ValueType*)_pReflection->m_fnGet ## name ## MetaValue(_pData, type2, _key, Length, CONST_CHAR_POINTER_ID); \
    } \
 \
    template <std::size_t Length> \
    static inline void* get_meta_value_ptr(__ftable* _pReflection, __data* _pData, type, const char (&_key)[Length]) \
    { \
        return _pReflection->m_fnGet ## name ## MetaValue(_pData, type2, _key, Length, CONST_CHAR_POINTER_ID); \
    } \
 \
    template <typename KeyType> \
    static inline DUCKVIL_RESOURCE(variant_t) get_meta_value_handle(type, const KeyType& _key) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta_value_handle<KeyType>(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    } \
 \
    template <typename ValueType, typename KeyType> \
    static inline const ValueType& get_meta_value(type, const KeyType& _key) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta_value<KeyType, ValueType>(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    } \
 \
    template <typename KeyType> \
    static inline void* get_meta_value_ptr(type, const KeyType& _key) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta_value_ptr<KeyType>(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    } \
 \
    template <typename KeyType> \
    static inline const __variant& get_meta(type, const KeyType& _key) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta<KeyType>(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    } \
 \
    template <typename ValueType, std::size_t Length> \
    static inline const ValueType& get_meta_value(type, const char (&_key)[Length]) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta_value<ValueType>(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    } \
 \
    template <std::size_t Length> \
    static inline void* get_meta_value_ptr(type, const char (&_key)[Length]) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta_value_ptr(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    }

namespace Duckvil { namespace RuntimeReflection {


    DUCKVIL_META_UTIL(Type, DUCKVIL_RESOURCE(type_t) _typeHandle, _typeHandle);
    DUCKVIL_META_UTIL(Property, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle), DUCKVIL_META_CAT(_typeHandle, _propertyHandle));
    DUCKVIL_META_UTIL(Constructor, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle), DUCKVIL_META_CAT(_typeHandle, _constructorHandle));

}}