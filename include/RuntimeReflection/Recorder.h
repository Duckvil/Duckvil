#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

// Index will be incremented each source file to avoid function name collision

#define DUCKVIL_RUNTIME_REFLECTION_RECORD(index) \
    DUCKVIL_EXPORT void duckvil_runtime_reflection_record_##index(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData)

// Each plugin/__module will be created file which contains total count of recorders

#define DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(count) \
    DUCKVIL_EXPORT uint32_t duckvil_get_runtime_reflection_recorder_count() { return count; }

#define DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF _pMemoryInterface, _pAllocator, _pRecorder, _pData
#define DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(type, trait) (std::trait<type>::value ? static_cast<__traits>((uint8_t)_traits | (uint8_t)__traits::trait) : _traits)

namespace std {

    template <typename Type>
    struct is_bool : std::is_same<bool, typename std::remove_cv<Type>::type> {};

}

namespace Duckvil { namespace RuntimeReflection {

    struct __recorder_meta_info
    {
        std::size_t m_ullKeyTypeID;
        std::size_t m_ullKeyTypeSize;
        uint8_t m_ucKeyTypeAlignment;
        __traits m_keyTraits;
        const void* m_pKeyData;
        std::size_t m_ullValueTypeID;
        std::size_t m_ullValueTypeSize;
        uint8_t m_ucValueTypeAlignment;
        __traits m_valueTraits;
        const void* m_pValueData;
    };

    template <typename Type>
    static inline __traits recorder_generate_traits()
    {
        __traits _traits = {};

        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_pointer);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_reference);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_array);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_void);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_integral);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_floating_point);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_enum);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_bool);

        return _traits;
    }

    template <typename Type>
    static inline __traits recorder_generate_traits(const Type& _type)
    {
        return recorder_generate_traits<Type>();
    }

    struct __recorder_ftable
    {
        DUCKVIL_RESOURCE(type_t) (*m_fnRecordType)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX]);
        DUCKVIL_RESOURCE(constructor_t) (*m_fnRecordConstructor)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, uint8_t* _pConctructor);
        DUCKVIL_RESOURCE(property_t) (*m_fnRecordProperty)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], uintptr_t _ullAddress);
        DUCKVIL_RESOURCE(namespace_t) (*m_fnRecordNamespace)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, const char _sName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX]);
        DUCKVIL_RESOURCE(inheritance_t) (*m_fnRecordInheritance)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, DUCKVIL_RESOURCE(type_t) _typeHandle, __protection _protection);
        DUCKVIL_RESOURCE(function_t) (*m_fnRecordFunction)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, __ifunction* _pFunction, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], std::size_t _ullReturnTypeID, std::size_t _ullArgumentsTypeID);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordTypeMeta)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordPropertyMeta)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _owner, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordConstructorMeta)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _owner, const __recorder_meta_info& _meta);
    };

    template <typename Type>
    static DUCKVIL_RESOURCE(type_t) record_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        return _pFunctions->m_fnRecordType(_pMemoryInterface, _pAllocator, _pData, typeid(Type).hash_code(), _sName);
    }

    template <typename Type, typename... Args>
    static DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pFunctions->m_fnRecordConstructor(_pMemoryInterface, _pAllocator, _pData, _typeHandle, typeid(void*(Args...)).hash_code(), (uint8_t*)&create_type<Type, Args...>);
    }

    template <typename A, typename B>
    static DUCKVIL_RESOURCE(property_t) record_property(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, uintptr_t _ullOffset, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX])
    {
        static std::size_t _typeA_ID = typeid(A).hash_code();
        static std::size_t _typeB_ID = typeid(B).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeA_ID)
            {
                return _pFunctions->m_fnRecordProperty(_pMemoryInterface, _pAllocator, _pData, _type.m_uiSlotIndex, _typeB_ID, _sName, _ullOffset);
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename KeyType, typename ValueType>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const KeyType& _key, const ValueType& _value)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(KeyType).hash_code();
        _meta.m_ullKeyTypeSize = sizeof(KeyType);
        _meta.m_ucKeyTypeAlignment = alignof(KeyType);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_traits<KeyType>();

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = sizeof(ValueType);
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_traits<ValueType>();

        return _pFunctions->m_fnRecordTypeMeta(_pMemoryInterface, _pAllocator, _pData, _handle, _meta);
    }

    template <typename ValueType, std::size_t Length>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const char (&_key)[Length], const ValueType& _value)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(const char*).hash_code();
        _meta.m_ullKeyTypeSize = Length;
        _meta.m_ucKeyTypeAlignment = alignof(const char*);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_traits(_key);

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = sizeof(ValueType);
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_traits<ValueType>();

        return _pFunctions->m_fnRecordTypeMeta(_pMemoryInterface, _pAllocator, _pData, _handle, _meta);
    }

    template <typename KeyType, typename ValueType>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _type, DUCKVIL_RESOURCE(property_t) _handle, const KeyType& _key, const ValueType& _value)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(KeyType).hash_code();
        _meta.m_ullKeyTypeSize = sizeof(KeyType);
        _meta.m_ucKeyTypeAlignment = alignof(KeyType);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_traits<KeyType>();

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = sizeof(ValueType);
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_traits<ValueType>();

        return _pFunctions->m_fnRecordPropertyMeta(_pMemoryInterface, _pAllocator, _pData, _type, _handle, _meta);
    }

    template <typename ValueType, std::size_t Length>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _type, DUCKVIL_RESOURCE(property_t) _handle, const char (&_key)[Length], const ValueType& _value)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(const char*).hash_code();
        _meta.m_ullKeyTypeSize = Length;
        _meta.m_ucKeyTypeAlignment = alignof(const char*);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_traits(_key);

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = sizeof(ValueType);
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_traits<ValueType>();

        return _pFunctions->m_fnRecordPropertyMeta(_pMemoryInterface, _pAllocator, _pData, _type, _handle, _meta);
    }

    template <typename KeyType, typename ValueType>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _type, DUCKVIL_RESOURCE(constructor_t) _handle, const KeyType& _key, const ValueType& _value)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(KeyType).hash_code();
        _meta.m_ullKeyTypeSize = sizeof(KeyType);
        _meta.m_ucKeyTypeAlignment = alignof(KeyType);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_traits<KeyType>();

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = sizeof(ValueType);
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_traits<ValueType>();

        return _pFunctions->m_fnRecordConstructorMeta(_pMemoryInterface, _pAllocator, _pData, _type, _handle, _meta);
    }

    template <typename ValueType, std::size_t Length>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _type, DUCKVIL_RESOURCE(constructor_t) _handle, const char (&_key)[Length], const ValueType& _value)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(const char*).hash_code();
        _meta.m_ullKeyTypeSize = Length;
        _meta.m_ucKeyTypeAlignment = alignof(const char*);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_traits(_key);

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = sizeof(ValueType);
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_traits<ValueType>();

        return _pFunctions->m_fnRecordConstructorMeta(_pMemoryInterface, _pAllocator, _pData, _type, _handle, _meta);
    }

    template <typename B>
    static DUCKVIL_RESOURCE(property_t) record_property(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, uintptr_t _ullOffset, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX])
    {
        static std::size_t _typeB_ID = typeid(B).hash_code();

        return _pFunctions->m_fnRecordProperty(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _typeB_ID, _sName, _ullOffset);
    }

    static DUCKVIL_RESOURCE(namespace_t) record_namespace(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX])
    {
        return _pFunctions->m_fnRecordNamespace(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _sName);
    }

    static DUCKVIL_RESOURCE(inheritance_t) record_inheritance(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritance, __protection _protection)
    {
        return _pFunctions->m_fnRecordInheritance(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _inheritance, _protection);
    }

    template <typename Type, typename ReturnType, typename... Args>
    static DUCKVIL_RESOURCE(function_t) record_function(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, ReturnType (Type::*func)(Args...), const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        void* _pointer =
            _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__function<ReturnType (Type::*)(Args...)>), alignof(__function<ReturnType (Type::*)(Args...)>));

        __function<ReturnType (Type::*)(Args...)>* _function = new(_pointer) __function<ReturnType (Type::*)(Args...)>();

        _function->m_fnFunction = func;

        return _pFunctions->m_fnRecordFunction(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _function, _sName, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code());
    }

    template <typename ReturnType, typename... Args>
    static DUCKVIL_RESOURCE(function_t) record_function(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, ReturnType (*func)(Args...), const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        void* _pointer =
            _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__function<ReturnType (*)(Args...)>), alignof(__function<ReturnType (*)(Args...)>));

        __function<ReturnType (*)(Args...)>* _function = new(_pointer) __function<ReturnType (*)(Args...)>();

        _function->m_fnFunction = func;

        return _pFunctions->m_fnRecordFunction(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _function, _sName, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code());
    }

}}

typedef Duckvil::RuntimeReflection::__recorder_ftable* (*duckvil_runtime_reflection_recorder_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);