#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

// Index will be incremented each source file to avoid function name collision

#define DUCKVIL_RUNTIME_REFLECTION_RECORD(index) \
    DUCKVIL_EXPORT void duckvil_runtime_reflection_record_##index(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData)

// Each plugin/module will be created file which contains total count of recorders

#define DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(count) \
    DUCKVIL_EXPORT uint32_t duckvil_get_runtime_reflection_recorder_count() { return count; }

#define DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF _pMemoryInterface, _pAllocator, _pRecorder, _pData

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type>
    static __traits recorder_generate_traits()
    {
        __traits _traits = {};

        _traits.m_bIsPointer = std::is_pointer<Type>::value;
        _traits.m_bIsReference = std::is_reference<Type>::value;
        _traits.m_bIsArray = std::is_array<Type>::value;
        _traits.m_bIsVoid = std::is_void<Type>::value;
        _traits.m_bIsIntegral = std::is_integral<Type>::value;
        _traits.m_bIsFloatingPoint = std::is_floating_point<Type>::value;
        _traits.m_bIsEnum = std::is_enum<Type>::value;

        return _traits;
    }

    template <typename Type>
    static __traits recorder_generate_traits(const Type& _type)
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
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const KeyType& _key, const ValueType& _value)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _typeHandle.m_ID);
        std::size_t _keyTypeID = typeid(KeyType).hash_code();
        std::size_t _keyTypeSize = sizeof(KeyType);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type->m_variantKeys.m_data); i++)
        {
            __variant_t _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, i);

            if(_key.m_ullTypeID == _keyTypeID && _key.m_ullSize == _keyTypeSize && memcmp(_key.m_pData, &_key, _keyTypeSize) == 0)
            {
                // The key was found, we need to change value

                _keyFound = true;

                break;
            }
        }

        if(!_keyFound)
        {
            DUCKVIL_RESOURCE(variant_t) _keyHandle = {};
            DUCKVIL_RESOURCE(variant_t) _valueHandle = {};

            {
                __variant_t _keyVariant = {};

                _keyVariant.m_ullTypeID = _keyTypeID;
                _keyVariant.m_ullSize = _keyTypeSize;
                _keyVariant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _keyVariant.m_ullSize, alignof(KeyType));
                _keyVariant.m_traits = recorder_generate_traits<KeyType>();

                memcpy(_keyVariant.m_pData, &_key, _keyVariant.m_ullSize);

                _keyHandle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_ullTypeID = typeid(ValueType).hash_code();
                _valueVariant.m_ullSize = sizeof(ValueType);
                _valueVariant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _valueVariant.m_ullSize, alignof(ValueType));
                _valueVariant.m_traits = recorder_generate_traits<ValueType>();

                memcpy(_valueVariant.m_pData, &_value, _valueVariant.m_ullSize);

                _valueHandle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _meta = {};

            _meta.m_key = _keyHandle;
            _meta.m_value = _valueHandle;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_metas, _meta);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename ValueType, std::size_t Length>
    static DUCKVIL_RESOURCE(meta_t) record_meta(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_key)[Length], const ValueType& _value)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _typeHandle.m_ID);
        std::size_t _keyTypeID = typeid(const char*).hash_code();
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type->m_variantKeys.m_data); i++)
        {
            __variant_t _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, i);

            if(_key.m_ullTypeID == _keyTypeID && _key.m_ullSize == Length && memcmp(_key.m_pData, &_key, Length) == 0)
            {
                // The key was found, we need to change value

                _keyFound = true;

                break;
            }
        }

        if(!_keyFound)
        {
            DUCKVIL_RESOURCE(variant_t) _keyHandle = {};
            DUCKVIL_RESOURCE(variant_t) _valueHandle = {};

            {
                __variant_t _keyVariant = {};

                _keyVariant.m_ullTypeID = _keyTypeID;
                _keyVariant.m_ullSize = Length;
                _keyVariant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _keyVariant.m_ullSize, 8);
                _keyVariant.m_traits = recorder_generate_traits(_key);

                memcpy(_keyVariant.m_pData, _key, Length);

                _keyHandle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_ullTypeID = typeid(ValueType).hash_code();
                _valueVariant.m_ullSize = sizeof(ValueType);
                _valueVariant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _valueVariant.m_ullSize, alignof(ValueType));
                _valueVariant.m_traits = recorder_generate_traits<ValueType>();

                memcpy(_valueVariant.m_pData, &_value, _valueVariant.m_ullSize);

                _valueHandle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _meta = {};

            _meta.m_key = _keyHandle;
            _meta.m_value = _valueHandle;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_metas, _meta);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
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
        __function<ReturnType (Type::*)(Args...)>* _function =
            (__function<ReturnType (Type::*)(Args...)>*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__function<ReturnType (Type::*)(Args...)>), alignof(__function<ReturnType (Type::*)(Args...)>));

        _function->m_fnFunction = func;

        return _pFunctions->m_fnRecordFunction(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _function, _sName, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code());
    }

    template <typename ReturnType, typename... Args>
    static DUCKVIL_RESOURCE(function_t) record_function(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, ReturnType (*func)(Args...), const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        __function<ReturnType (*)(Args...)>* _function =
            (__function<ReturnType (*)(Args...)>*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__function<ReturnType (*)(Args...)>), alignof(__function<ReturnType (*)(Args...)>));

        _function->m_fnFunction = func;

        return _pFunctions->m_fnRecordFunction(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _function, _sName, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code());
    }

}}

typedef Duckvil::RuntimeReflection::__recorder_ftable* (*duckvil_runtime_reflection_recorder_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);