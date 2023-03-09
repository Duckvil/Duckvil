#pragma once

#include "RuntimeReflection/Global.h"

#include "Memory/FreeList.h"

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
	template <typename KeyType> \
    static inline const __variant& get_meta(__ftable* _pReflection, __data* _pData, type) \
    { \
        return _pReflection->m_fnGet ## name ## MetaVariant(_pData, type2, nullptr, sizeof(KeyType), typeid(KeyType).hash_code()); \
    } \
 \
    template <std::size_t Length> \
    static inline DUCKVIL_RESOURCE(variant_t) get_meta_value_handle(__ftable* _pReflection, __data* _pData, type, const char (&_key)[Length]) \
    { \
        return _pReflection->m_fnGet ## name ## MetaHandle(_pData, type2, _key, Length, CONST_CHAR_POINTER_ID); \
    } \
 \
    template <std::size_t Length> \
    static inline const __variant& get_meta(__ftable* _pReflection, __data* _pData, type, const char (&_key)[Length]) \
    { \
        return _pReflection->m_fnGet ## name ## MetaVariant(_pData, type2, _key, Length, CONST_CHAR_POINTER_ID); \
    } \
 \
    static inline const __meta_t& get_meta(__ftable* _pReflection, __data* _pData, type, DUCKVIL_RESOURCE(meta_t) _handle) \
    { \
        return _pReflection->m_fnGet ## name ## MetaByHandle(_pData, type2, _handle); \
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
    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(const Memory::FreeList& _heap, __ftable* _pReflection, __data* _pData, type) \
    { \
        return _pReflection->m_fnGet ## name ## Metas(_heap, _pData, type2); \
    } \
 \
    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(__ftable* _pReflection, __data* _pData, type) \
    { \
        const Memory::free_list_context& _heapContext = Memory::heap_get_current(); \
        return get_metas(_heapContext.m_heap, _pReflection, _pData, type2); \
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
	template <typename KeyType> \
    static inline const __variant& get_meta(type) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta<KeyType>(_context.m_pReflection, _context.m_pReflectionData, type2); \
    } \
 \
    template <std::size_t Length> \
    static inline const __variant& get_meta(type, const char (&_key)[Length]) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta(_context.m_pReflection, _context.m_pReflectionData, type2, _key); \
    } \
 \
    static inline const __meta_t& get_meta(type, DUCKVIL_RESOURCE(meta_t) _handle) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_meta(_context.m_pReflection, _context.m_pReflectionData, type2, _handle); \
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
    } \
 \
    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(const Memory::FreeList& _heap, type) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_metas(_heap, _context.m_pReflection, _context.m_pReflectionData, type2); \
    } \
 \
    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(type) \
    { \
        const duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext; \
        return get_metas(_context.m_pReflection, _context.m_pReflectionData, type2); \
    }

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_META_UTIL(Type, DUCKVIL_RESOURCE(type_t) _typeHandle, _typeHandle);
    DUCKVIL_META_UTIL(Property, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle), DUCKVIL_META_CAT(_typeHandle, _propertyHandle));
    DUCKVIL_META_UTIL(Constructor, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle), DUCKVIL_META_CAT(_typeHandle, _constructorHandle));
    DUCKVIL_META_UTIL(ConstructorArgument, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex), DUCKVIL_META_CAT(_typeHandle, _constructorHandle, _uiArgumentIndex));
    DUCKVIL_META_UTIL(Function, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle), DUCKVIL_META_CAT(_typeHandle, _functionHandle));
    DUCKVIL_META_UTIL(FunctionArgument, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex), DUCKVIL_META_CAT(_typeHandle, _functionHandle, _uiArgumentIndex));

    template <typename KeyType, typename ValueType>
    static inline DUCKVIL_RESOURCE(meta_t) add_object_meta(__ftable* _pReflection, __data* _pData, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, void* _pObject, const KeyType& _key, const ValueType& _value)
    {
        return _pReflection->m_fnAddObjectMeta(
            _pData,
            _pMemoryInterface,
            _pAllocator,
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            alignof(KeyType),
            recorder_generate_property_traits(_key),
            &_key,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_property_traits(_value),
            &_value
            );
    }

    template <typename KeyType, typename ValueType>
    static inline DUCKVIL_RESOURCE(meta_t) add_object_meta(void* _pObject, const KeyType& _key, const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnAddObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            alignof(KeyType),
            recorder_generate_property_traits(_key),
            &_key,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_property_traits(_value),
            &_value
            );
    }

    template <typename KeyType>
    static inline __variant_t get_object_meta(void* _pObject, const KeyType& _key)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetObjectMeta(
            _context.m_pReflectionData,
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            &_key
        );
    }

    template <typename KeyType, typename ValueType>
    static inline void set_object_meta(void* _pObject, const KeyType& _key, const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnSetObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            &_key,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_property_traits(_value),
            &_value
        );
    }

    template <typename KeyType>
    static inline void remove_object_meta(void* _pObject, const KeyType& _key)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnRemoveObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            &_key
        );
    }

    template <typename ValueType, size_t Length>
    static inline DUCKVIL_RESOURCE(meta_t) add_object_meta(void* _pObject, const char (&_sKey)[Length], const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnAddObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            CONST_CHAR_POINTER_ID,
            Length,
            8,
            recorder_generate_property_traits(_sKey),
            _sKey,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_property_traits(_value),
            &_value
        );
    }

    template <size_t Length>
    static inline __variant_t get_object_meta(void* _pObject, const char (&_sKey)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetObjectMeta(
            _context.m_pReflectionData,
            _pObject,
            CONST_CHAR_POINTER_ID,
            Length,
            _sKey
        );
    }

    template <typename ValueType, size_t Length>
    static inline void set_object_meta(void* _pObject, const char (&_sKey)[Length], const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnSetObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            CONST_CHAR_POINTER_ID,
            Length,
            &_sKey,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_property_traits(_value),
            &_value
        );
    }

    template <size_t Length>
    static inline void remove_object_meta(void* _pObject, const char (&_sKey)[Length])
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnRemoveObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            CONST_CHAR_POINTER_ID,
            Length,
            &_sKey
        );
    }

    static inline void clear_object_metas(void* _pObject)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnClearObjectMetas(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject
        );
    }

    template <typename KeyType>
    static bool meta_has_value(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_constructor_t& _constructorHandle, const __duckvil_resource_argument_t& _argumentHandle, const KeyType& _key)
    {
        const auto _meta = RuntimeReflection::get_meta(_typeHandle, _constructorHandle, _argumentHandle.m_ID, _key);

        if(_meta.m_pData != nullptr && _meta.m_ullTypeID == typeid(bool).hash_code() && *static_cast<bool*>(_meta.m_pData))
        {
            return true;
        }

        return false;
    }

}}