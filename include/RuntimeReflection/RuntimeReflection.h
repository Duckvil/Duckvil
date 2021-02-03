#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Vector.h"
#include "Memory/FreeList.h"
#include "Memory/SlotArray.h"

#include <typeinfo>

#define slot(T, ...) \
    struct T \
        __VA_ARGS__ \
    ; \
    DUCKVIL_SLOT_ARRAY_DECLARE(T)

#define DUCKVIL_RESOURCE(t) \
    __duckvil_resource_##t

#define DUCKVIL_RESOURCE_DECLARE(t) \
    struct DUCKVIL_RESOURCE(t) \
    { \
        uint32_t m_ID; \
    }

#define DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX 32
#define DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX 32
#define DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX 32
#define DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX 32

namespace Duckvil { namespace RuntimeReflection {

    static const std::size_t& CONST_CHAR_POINTER_ID = typeid(const char*).hash_code();

    template <typename Type, typename... Args>
    void* create_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, Args... _vArgs)
    {
        return new(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Type), alignof(Type))) Type(_vArgs...);
    }

    struct __ifunction
    {

    };

    template <typename ReturnType, typename... Args>
    struct __proxy_static_function : public __ifunction
    {
        virtual ReturnType Invoke(const Args&... _vArgs) = 0;
    };

    template <typename ReturnType, typename... Args>
    struct __proxy_member_function : public __ifunction
    {
        virtual ReturnType Invoke(void* _pObject, const Args&... _vArgs) = 0;
    };

    template <typename T>
    struct __function;

// Member function with specific arguments which returns void
    template <typename Type, typename... Args>
    struct __function<void(Type::*)(Args...)> : public __proxy_member_function<void, Args...>
    {
        typedef void (Type::*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        inline void Invoke(void* _pObject, const Args&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))(_vArgs...);
        }
    };

// Member function with specific arguments which returns specific type
    template <typename Type, typename ReturnType, typename... Args>
    struct __function<ReturnType(Type::*)(Args...)> : public __proxy_member_function<ReturnType, Args...>
    {
        typedef ReturnType (Type::*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        inline ReturnType Invoke(void* _pObject, const Args&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))(_vArgs...);
        }
    };

// Static function with specific arguments which returns void
    template <typename... Args>
    struct __function<void(*)(Args...)> : public __proxy_static_function<void, Args...>
    {
        typedef void (*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        inline void Invoke(const Args&... _vArgs) final override
        {
            m_fnFunction(_vArgs...);
        }
    };

// Static function with specific arguments which returns specific type
    template <typename ReturnType, typename... Args>
    struct __function<ReturnType(*)(Args...)> : public __proxy_static_function<ReturnType, Args...>
    {
        typedef ReturnType (*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        inline ReturnType Invoke(const Args&... _vArgs) final override
        {
            return m_fnFunction(_vArgs...);
        }
    };

    DUCKVIL_RESOURCE_DECLARE(type_t);
    DUCKVIL_RESOURCE_DECLARE(constructor_t);
    DUCKVIL_RESOURCE_DECLARE(property_t);
    DUCKVIL_RESOURCE_DECLARE(namespace_t);
    DUCKVIL_RESOURCE_DECLARE(inheritance_t);
    DUCKVIL_RESOURCE_DECLARE(function_t);
    DUCKVIL_RESOURCE_DECLARE(variant_t);
    DUCKVIL_RESOURCE_DECLARE(meta_t);

    enum class __traits : uint8_t
    {
        is_pointer          = 1 << 0,
        is_reference        = 1 << 1,
        is_array            = 1 << 2,
        is_void             = 1 << 3,
        is_integral         = 1 << 4,
        is_floating_point   = 1 << 5,
        is_enum             = 1 << 6,
        is_bool             = 1 << 7
    };

    enum __protection
    {
        __protection_public,
        __protection_protected,
        __protection_private
    };

    enum __variant_owner
    {
        __variant_owner_type,
        __variant_owner_property,
        __variant_owner_constructor
    };

    struct __variant
    {
        std::size_t m_ullSize;
        std::size_t m_ullTypeID;
        void* m_pData;
        __traits m_traits;
    };

    slot(__variant_t,
    {
        __variant m_variant;
        __variant_owner m_owner;
    });

    slot(__meta_t,
    {
        DUCKVIL_RESOURCE(variant_t) m_key;
        DUCKVIL_RESOURCE(variant_t) m_value;
        uint32_t m_uiOwner;
    });

    slot(__constructor_t,
    {
        std::size_t m_ullTypeID;
        DUCKVIL_RESOURCE(type_t) m_owner;
        uint8_t* m_pData;
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
    });

    slot(__property_t,
    {
        DUCKVIL_RESOURCE(type_t) m_owner;
        std::size_t m_ullTypeID;
        char m_sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX];
        uintptr_t m_ullAddress;
        __traits m_traits;
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
    });

    slot(__namespace_t,
    {
        char m_sTypeName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX];
    });

    slot(__inheritance_t,
    {
        // DUCKVIL_RESOURCE(type_t) m_uiTypeSlotIndex; // TODO: Check if set
        std::size_t m_ullInheritanceTypeID;
        __protection m_protection;
    });

    slot(__function_t,
    {
        DUCKVIL_RESOURCE(function_t) m_uiTypeSlotIndex; // TODO: Check if set
        __ifunction* m_pFunction;
        char m_sFunctionName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX];
        std::size_t m_ullReturnTypeID;
        std::size_t m_ullArgumentsTypeID;
    });

    slot(__type_t,
    {
        DUCKVIL_RESOURCE(type_t) m_uiSlotIndex;
        std::size_t m_ullTypeID;
        char m_sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX];
        DUCKVIL_SLOT_ARRAY(__constructor_t) m_constructors;
        DUCKVIL_SLOT_ARRAY(__property_t) m_properties;
        DUCKVIL_SLOT_ARRAY(__namespace_t) m_namespaces;
        DUCKVIL_SLOT_ARRAY(__inheritance_t) m_inheritances;
        DUCKVIL_SLOT_ARRAY(__function_t) m_functions;
    // This contains all variant keys, even for properties, functions, etc.
    // It could be useful if we want to get all metas that contains specific key in specific type
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantKeys;
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantValues;
    // This contains only metas for specific type
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
    });

    template <std::size_t Length>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const char(&_sName)[Length])
    {
        if(!_bRes)
        {
            return;
        }

        const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_typeNamespaces, _uiIndex);

        if(strcmp(_namespace.m_sTypeName, _sName) != 0)
        {
            _bRes = false;
        }

        _uiIndex++;
    }

    template <typename Arg>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const Arg& _arg)
    {
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _arg);
    }

    template <typename Arg, typename... Args>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const Arg& _arg, const Args&... _args)
    {
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _arg);
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _args...);
    }

    struct __data
    {
        DUCKVIL_SLOT_ARRAY(__type_t) m_aTypes;
    };

    struct __ftable
    {
        __data* (*m_fnInit)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __ftable* _pFunctions);

        DUCKVIL_RESOURCE(type_t) (*m_fnGetType)(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX]);
        void* (*m_fnGetProperty)(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject);
    };

// Create type object using constructor handle
    // template <typename... Args>
    // static void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, Args... _vArgs)
    // {
    //     const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aConstructors, _constructorHandle.m_ID);

    //     void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

    //     return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
    // }

// Create type object using type name string
// Note: It will compare given arguments with available constructors arguments
    template <typename... Args>
    static void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX], Args... _vArgs)
    {
        static const std::size_t& _constructorTypeID = typeid(void*(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sTypeName) == 0)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
                {
                    const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

                    if(_constructor.m_ullTypeID == _constructorTypeID)
                    {
                        void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

                        return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
                    }
                }
            }
        }

        return nullptr;
    }

// Create type object using type handle
// Note: It will compare given arguments with available constructors arguments
    template <typename... Args>
    static void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, Args... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _constructorTypeID = typeid(void*(Args...)).hash_code();

        for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
        {
            const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

            if(_constructor.m_ullTypeID == _constructorTypeID)
            {
                void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

                return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
            }
        }

        return nullptr;
    }

    template <typename KeyType>
    static DUCKVIL_RESOURCE(variant_t) get_meta_value_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const KeyType& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _keyTypeID = typeid(KeyType).hash_code();
        static const std::size_t& _keyTypeSize = sizeof(KeyType);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == _keyTypeID && _keyVariant.m_variant.m_ullSize == _keyTypeSize && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename ValueType, typename KeyType>
    static inline const ValueType& get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const KeyType&& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _handle.m_ID);
        static const std::size_t& _keyTypeID = typeid(KeyType).hash_code();
        static const std::size_t& _keyTypeSize = sizeof(KeyType);
        static ValueType _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == _keyTypeID && _keyVariant.m_variant.m_ullSize == _keyTypeSize && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }

        return _invalid;
    }

    template <typename KeyType>
    static inline __variant get_meta(__data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const KeyType&& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _handle.m_ID);
        static const std::size_t& _keyTypeID = typeid(KeyType).hash_code();
        static const std::size_t& _keyTypeSize = sizeof(KeyType);
        __variant _variant;

        _variant.m_ullTypeID = std::numeric_limits<std::size_t>::max();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == _keyTypeID && _keyVariant.m_variant.m_ullSize == _keyTypeSize && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                _variant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i).m_variant;

                break;
            }
        }

        return _variant;
    }

    template <typename ValueType, std::size_t Length>
    static inline const ValueType& get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const char (&_key)[Length])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _handle.m_ID);
        static ValueType _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == CONST_CHAR_POINTER_ID && _keyVariant.m_variant.m_ullSize == Length && memcmp(_keyVariant.m_variant.m_pData, _key, Length) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }

        return _invalid;
    }

    template <typename ValueType, typename KeyType>
    static inline const ValueType& get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle, const KeyType&& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);
        static const std::size_t& _keyTypeID = typeid(KeyType).hash_code();
        static const std::size_t& _keyTypeSize = sizeof(KeyType);
        static ValueType _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); i++)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _keyTypeID && _keyVariant.m_variant.m_ullSize == _keyTypeSize && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }

        return _invalid;
    }

    template <typename ValueType, std::size_t Length>
    static inline const ValueType& get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle, const char (&_key)[Length])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);
        static ValueType _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); i++)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == CONST_CHAR_POINTER_ID && _keyVariant.m_variant.m_ullSize == Length && memcmp(_keyVariant.m_variant.m_pData, _key, Length) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }

        return _invalid;
    }

    template <typename ValueType, typename KeyType>
    static inline const ValueType& get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle, const KeyType&& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _handle.m_ID);
        static const std::size_t& _keyTypeID = typeid(KeyType).hash_code();
        static const std::size_t& _keyTypeSize = sizeof(KeyType);
        static ValueType _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor.m_metas.m_data); i++)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _keyTypeID && _keyVariant.m_variant.m_ullSize == _keyTypeSize && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }

        return _invalid;
    }

    template <typename ValueType, std::size_t Length>
    static inline const ValueType& get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle, const char (&_key)[Length])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _handle.m_ID);
        static ValueType _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor.m_metas.m_data); i++)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == CONST_CHAR_POINTER_ID && _keyVariant.m_variant.m_ullSize == Length && memcmp(_keyVariant.m_variant.m_pData, _key, Length) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }

        return _invalid;
    }

    static DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const std::size_t& _ullTypeID)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _ullTypeID)
            {
                return _type.m_uiSlotIndex;
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename Type>
    static DUCKVIL_RESOURCE(type_t) get_type(__data* _pData)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();

        return get_type(_pData, _typeID);
    }

    template <std::size_t Length, std::size_t... Lengths>
    static DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char (&_sName)[Length], const char (&..._sNamespaces)[Lengths])
    {
        static const std::size_t& _argsSize = sizeof...(Lengths);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_argsSize == DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data))
            {
                uint32_t _index = 0;
                bool _res = true;

                compare_namespace(_type.m_namespaces, _index, _res, _sNamespaces...);

                if(_res && strcmp(_type.m_sTypeName, _sName) == 0)
                {
                    return _type.m_uiSlotIndex;
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    static const __type_t& get_type(__data* _pData, const DUCKVIL_RESOURCE(type_t)& _typeHandle)
    {
        return DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
    }

    static DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char* _sName, const std::vector<const char*>& _aNamespaces)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);
            size_t _namespacesSize = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data);

            if(_aNamespaces.size() == _namespacesSize)
            {
                bool _valid = true;

                for(uint32_t j = 0; j < _aNamespaces.size(); j++)
                {
                    const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j);

                    if(strcmp(_namespace.m_sTypeName, _aNamespaces[j]) != 0)
                    {
                        _valid = false;

                        break;
                    }
                }

                if(_valid && strcmp(_sName, _type.m_sTypeName) == 0)
                {
                    return { i };
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    static Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    {
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            _types.Allocate(_type.m_uiSlotIndex);
        }

        return _types;
    }

    // template <>
    // static Memory::Vector<DUCKVIL_RESOURCE(type_t), Memory::__free_list_allocator> get_types(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    // {
    //     std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data);
    //     auto _types = Memory::VectorFactory<DUCKVIL_RESOURCE(type_t)>::Create(_pMemory, _pAllocator, _size);

    //     for(uint32_t i = 0; i < _size; i++)
    //     {
    //         __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

    //         _types.Allocate(_type.m_uiSlotIndex);
    //     }

    //     return _types;
    // }

    // template <>
    // static Memory::Vector<DUCKVIL_RESOURCE(type_t), Memory::__linear_allocator> get_types(__data* _pData, Memory::IMemory* _pMemory, Memory::__linear_allocator* _pAllocator)
    // {
    //     std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data);
    //     auto _types = Memory::VectorFactory<DUCKVIL_RESOURCE(type_t)>::Create(_pMemory, _pAllocator, _size);

    //     for(uint32_t i = 0; i < _size; i++)
    //     {
    //         __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

    //         _types.Allocate(_type.m_uiSlotIndex);
    //     }

    //     return _types;
    // }

    static DUCKVIL_RESOURCE(property_t) get_property_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); i++)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename... Args>
    static DUCKVIL_RESOURCE(constructor_t) get_constructor_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, Args... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _constructorTypeID = typeid(void*(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); i++)
        {
            const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, i);

            if(_constructor.m_ullTypeID == _constructorTypeID)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename... Args>
    static DUCKVIL_RESOURCE(function_t) get_function_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); i++)
        {
            const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, i);

            if(strcmp(_function.m_sFunctionName, _sName) == 0 && _function.m_ullArgumentsTypeID == _argsTypeID)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

// TODO: Check it
    template <typename... Args>
    static __function<void(*)(Args...)>* get_static_function_callback(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); i++)
        {
            const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, i);

            if(strcmp(_function.m_sFunctionName, _sName) == 0 && _function.m_ullArgumentsTypeID == _argsTypeID)
            {
                __function<void(*)(Args...)>* _func = (__function<void(*)(Args...)>*)_function.m_pFunction;

                return _func;
            }
        }

        return nullptr;
    }

    template <typename Type, typename... Args>
    static __function<void(Type::*)(Args...)>* get_function_callback(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); i++)
        {
            const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, i);

            if(strcmp(_function.m_sFunctionName, _sName) == 0 && _function.m_ullArgumentsTypeID == _argsTypeID)
            {
                __function<void(Type::*)(Args...)>* _func = (__function<void(Type::*)(Args...)>*)_function.m_pFunction;

                return _func;
            }
        }

        return nullptr;
    }

    template <typename Type, typename... Args>
    static __function<void(Type::*)(Args...)>* get_function_callback(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);

        return (__function<void(Type::*)(Args...)>*)_function.m_pFunction;
    }

    template <typename ReturnType, typename Type, typename... Args>
    static __function<ReturnType(Type::*)(Args...)>* get_function_callback(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); i++)
        {
            const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, i);

            if(strcmp(_function.m_sFunctionName, _sName) == 0 && _function.m_ullArgumentsTypeID == _argsTypeID)
            {
                __function<ReturnType(Type::*)(Args...)>* _func = (__function<ReturnType(Type::*)(Args...)>*)_function.m_pFunction;

                return _func;
            }
        }

        return nullptr;
    }

    template <typename Type>
    static void* get_property(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], const Type* _pObject)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); j++)
                {
                    const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, j);

                    if(strcmp(_property.m_sName, _sName) == 0)
                    {
                        return (uint8_t*)_pObject + _property.m_ullAddress;
                    }
                }
            }
        }

        return nullptr;
    }

    static void* get_property(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], const void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); i++)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return (uint8_t*)_pObject + _property.m_ullAddress;
            }
        }

        return nullptr;
    }

// Invoke member function
    template <typename Type, typename... Args>
    static void invoke(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], Type* _pObject, Args... _vArgs)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); j++)
                {
                    const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    if(strcmp(_function.m_sFunctionName, _sName) == 0 && typeid(void).hash_code() == _function.m_ullReturnTypeID && _argsTypeID == _function.m_ullArgumentsTypeID)
                    {
                        __function<void(Type::*)(Args...)>* _func = (__function<void(Type::*)(Args...)>*)_function.m_pFunction;

                        (_pObject->*(_func->m_fnFunction))(_vArgs...);

                        return;
                    }
                }
            }
        }
    }

    template <typename ReturnType, typename Type, typename... Args>
    static ReturnType invoke(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], Type* _pObject, Args... _vArgs)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); j++)
                {
                    const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    if(strcmp(_function.m_sFunctionName, _sName) == 0 && typeid(ReturnType).hash_code() == _function.m_ullReturnTypeID && _argsTypeID == _function.m_ullArgumentsTypeID)
                    {
                        __function<ReturnType(Type::*)(Args...)>* _func = (__function<ReturnType(Type::*)(Args...)>*)_function.m_pFunction;

                        return (_pObject->*(_func->m_fnFunction))(_vArgs...);
                    }
                }
            }
        }

        return ReturnType();
    }

    template <typename... Args>
    static void invoke_member(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        if(_argsTypeID == _function.m_ullArgumentsTypeID)
        {
            __proxy_member_function<void, Args...>* _func = (__proxy_member_function<void, Args...>*)_function.m_pFunction;

            _func->Invoke(_pObject, _vArgs...);
        }
    }

    template <typename ReturnType, typename... Args>
    static ReturnType invoke_member_result(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        if(_argsTypeID == _function.m_ullArgumentsTypeID)
        {
            __proxy_member_function<ReturnType, Args...>* _func = (__proxy_member_function<ReturnType, Args...>*)_function.m_pFunction;

            return _func->Invoke(_pObject, _vArgs...);
        }

        return ReturnType();
    }

// Invoke static function
    template <typename Type, typename... Args>
    static void invoke(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], Args... _vArgs)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); j++)
                {
                    const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    if(strcmp(_function.m_sFunctionName, _sName) == 0 && typeid(void).hash_code() == _function.m_ullReturnTypeID && _argsTypeID == _function.m_ullArgumentsTypeID)
                    {
                        __function<void(*)(Args...)>* _func = (__function<void(*)(Args...)>*)_function.m_pFunction;

                        (*(_func->m_fnFunction))(_vArgs...);

                        return;
                    }
                }
            }
        }
    }

    template <typename ReturnType, typename Type, typename... Args>
    static ReturnType invoke(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], Args... _vArgs)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); j++)
                {
                    const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    if(strcmp(_function.m_sFunctionName, _sName) == 0 && typeid(ReturnType).hash_code() == _function.m_ullReturnTypeID && _argsTypeID == _function.m_ullArgumentsTypeID)
                    {
                        __function<ReturnType(*)(Args...)>* _func = (__function<ReturnType(*)(Args...)>*)_function.m_pFunction;

                        return (*(_func->m_fnFunction))(_vArgs...);
                    }
                }
            }
        }

        return ReturnType();
    }

    template <typename... Args>
    static void invoke_static(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        if(_argsTypeID == _function.m_ullArgumentsTypeID)
        {
            __proxy_static_function<void, Args...>* _func = (__proxy_static_function<void, Args...>*)_function.m_pFunction;

            _func->Invoke(_vArgs...);
        }
    }

    template <typename ReturnType, typename... Args>
    static ReturnType invoke_static_result(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        static const std::size_t& _argsTypeID = typeid(void(Args...)).hash_code();

        if(_argsTypeID == _function.m_ullArgumentsTypeID)
        {
            __proxy_static_function<ReturnType, Args...>* _func = (__proxy_static_function<ReturnType, Args...>*)_function.m_pFunction;

            return _func->Invoke(_vArgs...);
        }

        return ReturnType();
    }

    static Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> get_inheritances(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> _inheritances(_pMemory, _pAllocator, DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data));

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data); i++)
        {
            auto& _a = DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, i);

            _inheritances.Allocate({ i });
        }

        return _inheritances;
    }

    static const __inheritance_t& get_inheritance(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(inheritance_t) _inheritanceHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, _inheritanceHandle.m_ID);
    }

    template <typename... Args>
    static void* create(const Memory::FreeList& _memory, __data* _pData, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX], Args... _vArgs)
    {
        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pData, _sTypeName, _vArgs...);
    }

    template <typename... Args>
    static void* create(const Memory::FreeList& _memory, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, Args... _vArgs)
    {
        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pData, _typeHandle, _vArgs...);
    }

}}

typedef Duckvil::RuntimeReflection::__ftable* (*duckvil_runtime_reflection_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);