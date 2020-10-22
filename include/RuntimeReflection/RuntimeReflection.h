#pragma once

#include "Memory/SlotArray.h"

#include "PlugNPlay/Module.h"

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

    template <typename Type, typename... Args>
    void* create_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, Args... _vArgs)
    {
        return new(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Type), alignof(Type))) Type(_vArgs...);
    }

    struct __ifunction
    {

    };

    template <typename T>
    struct __function;

// Member function with specific arguments which returns void
    template <typename Type, typename... Args>
    struct __function<void(Type::*)(Args...)> : public __ifunction
    {
        typedef void (Type::*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;
    };

// Member function with specific arguments which returns specific type
    template <typename Type, typename ReturnType, typename... Args>
    struct __function<ReturnType(Type::*)(Args...)> : public __ifunction
    {
        typedef ReturnType (Type::*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;
    };

// Static function with specific arguments which returns void
    template <typename... Args>
    struct __function<void(*)(Args...)> : public __ifunction
    {
        typedef void (*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;
    };

// Static function with specific arguments which returns specific type
    template <typename ReturnType, typename... Args>
    struct __function<ReturnType(*)(Args...)> : public __ifunction
    {
        typedef ReturnType (*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;
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
        is_pointer        = 1 << 0,
        is_reference      = 1 << 1,
        is_array          = 1 << 2,
        is_void           = 1 << 3,
        is_integral       = 1 << 4,
        is_floating_point = 1 << 5,
        is_enum           = 1 << 6
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
        __variant_owner_property
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
        DUCKVIL_RESOURCE(type_t) m_uiTypeSlotIndex; // TODO: Check if set
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
    template <typename... Args>
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, Args... _vArgs)
    {
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aConstructors, _constructorHandle.m_ID);

        void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

        return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
    }

// Create type object using type name string
// Note: It will compare given arguments with available constructors arguments
    template <typename... Args>
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX], Args... _vArgs)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sTypeName) == 0)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
                {
                    const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

                    if(_constructor.m_ullTypeID == typeid(void*(Args...)).hash_code())
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
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, Args... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
        {
            const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

            if(_constructor.m_ullTypeID == typeid(void*(Args...)).hash_code())
            {
                void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

                return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
            }
        }

        // for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        // {
        //     const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

        //     if(_typeHandle.m_ID == _type.m_uiSlotIndex.m_ID)
        //     {
        //         for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
        //         {
        //             const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

        //             if(_constructor.m_ullTypeID == typeid(void*(Args...)).hash_code())
        //             {
        //                 void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

        //                 return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
        //             }
        //         }
        //     }
        // }

        return nullptr;
    }

    template <typename KeyType>
    DUCKVIL_RESOURCE(variant_t) get_meta_value_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const KeyType& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == typeid(KeyType).hash_code() && _keyVariant.m_variant.m_ullSize == sizeof(KeyType) && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                return { i };
            }
        }
    }

    template <typename ValueType, typename KeyType>
    ValueType get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const KeyType&& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _handle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == typeid(KeyType).hash_code() && _keyVariant.m_variant.m_ullSize == sizeof(KeyType) && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }
    }

    template <typename ValueType, std::size_t Length>
    ValueType get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _handle, const char (&_key)[Length])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _handle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); i++)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == typeid(const char*).hash_code() && _keyVariant.m_variant.m_ullSize == Length && memcmp(_keyVariant.m_variant.m_pData, _key, Length) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }
    }

    template <typename ValueType, typename KeyType>
    ValueType get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle, const KeyType&& _key)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); i++)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == typeid(KeyType).hash_code() && _keyVariant.m_variant.m_ullSize == sizeof(KeyType) && memcmp(_keyVariant.m_variant.m_pData, &_key, _keyVariant.m_variant.m_ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }
    }

    template <typename ValueType, std::size_t Length>
    ValueType get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle, const char (&_key)[Length])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); i++)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == typeid(const char*).hash_code() && _keyVariant.m_variant.m_ullSize == Length && memcmp(_keyVariant.m_variant.m_pData, _key, Length) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return *(ValueType*)_valueVariant.m_variant.m_pData;
            }
        }
    }

    template <typename Type>
    DUCKVIL_RESOURCE(type_t) get_type(__data* _pData)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == typeid(Type).hash_code())
            {
                return _type.m_uiSlotIndex;
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

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

    template <typename Type>
    void* get_property(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], const Type* _pObject)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

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

        return 0;
    }

// Invoke member function
    template <typename Type, typename... Args>
    void invoke(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], Type* _pObject, Args... _vArgs)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); j++)
                {
                    const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    if(strcmp(_function.m_sFunctionName, _sName) == 0 && typeid(void).hash_code() == _function.m_ullReturnTypeID && typeid(void(Args...)).hash_code() == _function.m_ullArgumentsTypeID)
                    {
                        __function<void(Type::*)(Args...)>* _func = (__function<void(Type::*)(Args...)>*)_function.m_pFunction;

                        (_pObject->*(_func->m_fnFunction))(_vArgs...);
                    }
                } 
            }
        }
    }

// Invoke static function
    template <typename Type, typename... Args>
    void invoke(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_FUNCTION_NAME_MAX], Args... _vArgs)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); j++)
                {
                    const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    if(strcmp(_function.m_sFunctionName, _sName) == 0 && typeid(void).hash_code() == _function.m_ullReturnTypeID && typeid(void(Args...)).hash_code() == _function.m_ullArgumentsTypeID)
                    {
                        __function<void(*)(Args...)>* _func = (__function<void(*)(Args...)>*)_function.m_pFunction;

                        (*(_func->m_fnFunction))(_vArgs...);
                    }
                } 
            }
        }
    }

}}

typedef Duckvil::RuntimeReflection::__ftable* (*duckvil_runtime_reflection_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);