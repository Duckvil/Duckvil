#include "RuntimeReflection/RuntimeReflection.h"

// #ifndef g_pDuckvilFrontendReflectionContext
// duckvil_frontend_reflection_context* g_pDuckvilFrontendReflectionContext = nullptr;
// #endif

namespace Duckvil { namespace RuntimeReflection {

    __data* init(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __ftable* _pFunctions)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _data->m_aTypes = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __type_t);
        // _data->m_aFrontend = Memory::Vector<IReflectedType*>(_pMemoryInterface, _pAllocator, 1);

        return _data;
    }

    duckvil_frontend_reflection_context* create_context(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __ftable* _pFTable, __data* _pData)
    {
        duckvil_frontend_reflection_context* _ctx =
            (duckvil_frontend_reflection_context*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(duckvil_frontend_reflection_context), alignof(duckvil_frontend_reflection_context));

        _ctx->m_pReflection = _pFTable;
        _ctx->m_pReflectionData = _pData;

        return _ctx;
    }

    DUCKVIL_RESOURCE(type_t) get_type_by_name(__data* _pData, const char* _sName, std::size_t _ullLength)
    {
        // DUCKVIL_DYNAMIC_ARRAY_FOR(_pData->m_aTypes.m_data, __type_t, _type)
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sName) == 0)
            {
                return { _type.m_uiSlotIndex };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    const __type_t& get_type_data(__data* _pData, const DUCKVIL_RESOURCE(type_t)& _typeHandle)
    {
        return DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
    }

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    {
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            _types.Allocate(_type.m_uiSlotIndex);
        }

        return _types;
    }

    DUCKVIL_RESOURCE(type_t) get_type_by_type_id(__data* _pData, const std::size_t& _ullTypeID)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _ullTypeID)
            {
                return _type.m_uiSlotIndex;
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(property_t) get_property_handle_by_name(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++i)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> _constructors(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _constructors.Allocate({ i });
        }

        return _constructors;
    }

    DUCKVIL_RESOURCE(constructor_t) get_constructor_handle_by_type_id(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); ++i)
        {
            const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, i);

            if(_constructor.m_ullTypeID == _ullTypeID)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(function_t)> _functions(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _functions.Allocate({ i });
        }

        return _functions;
    }

    DUCKVIL_RESOURCE(function_t) get_function_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); ++i)
        {
            const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, i);

            if(_function.m_ullArgumentsTypeID == _ullTypeID && strcmp(_function.m_sFunctionName, _sName) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    __ifunction* get_function_callback(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); ++i)
        {
            const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, i);

            if(_function.m_ullArgumentsTypeID == _ullTypeID && strcmp(_function.m_sFunctionName, _sName) == 0)
            {
                return _function.m_pFunction;
            }
        }

        return nullptr;
    }

    __ifunction* get_function_callback_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, std::size_t _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);

        if(_function.m_ullArgumentsTypeID == _ullTypeID)
        {
            return _function.m_pFunction;
        }

        return nullptr;
    }

    const __function_t& get_function_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
    }

    void* get_property(__data* _pData, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID, const void* _pObject)
    {
        const __data& _data = *_pData;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);

            if(_type.m_ullTypeID == _ullTypeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++j)
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

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _handle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> _arguments(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _arguments.Allocate({ i });
        }

        return _arguments;
    }

    const __argument_t& get_argument(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _handle.m_ID);
    }

    void* get_property_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject)
    {
        __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _type_handle.m_ID);
        __property_t _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);

        return (uint8_t*)_pObject + _property.m_ullAddress;
    }

    void* get_property_by_name(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, const void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++i)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return (uint8_t*)_pObject + _property.m_ullAddress;
            }
        }

        return nullptr;
    }

    Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> get_inheritances(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> _inheritances(_pMemory, _pAllocator, DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data));

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data); ++i)
        {
            _inheritances.Allocate({ i });
        }

        return _inheritances;
    }

    const __inheritance_t& get_inheritance(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(inheritance_t) _inheritanceHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, _inheritanceHandle.m_ID);
    }

    bool inherits(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritanceHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __type_t& _inheritanceType = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data); ++i)
        {
            auto& _inheritance = DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, i);

            if(_inheritance.m_ullInheritanceTypeID == _inheritanceType.m_ullTypeID)
            {
                return true;
            }
        }

        return false;
    }

    bool inherits_by_type_id(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data); ++i)
        {
            auto& _inheritance = DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, i);

            if(_inheritance.m_ullInheritanceTypeID == _ullTypeID)
            {
                return true;
            }
        }

        return false;
    }

// META
    DUCKVIL_RESOURCE(variant_t) get_type_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); ++i)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    void* get_type_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); ++i)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i);

                return _valueVariant.m_variant.m_pData;
            }
        }

        return nullptr;
    }

    const __variant& get_type_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static __variant _result;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); ++i)
        {
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, i);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, i);

                return _valueVariant.m_variant;
            }
        }

        return _result;
    }

    DUCKVIL_RESOURCE(variant_t) get_property_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _propertyHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    void* get_property_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _propertyHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant.m_pData;
            }
        }

        return nullptr;
    }

    const __variant& get_property_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _propertyHandle.m_ID);
        static __variant _result;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_property.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant;
            }
        }

        return _result;
    }

    DUCKVIL_RESOURCE(variant_t) get_constructor_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    void* get_constructor_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant.m_pData;
            }
        }

        return nullptr;
    }

    const __variant& get_constructor_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
        static __variant _result;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant;
            }
        }

        return _result;
    }

}}

Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__ftable* _functions = (Duckvil::RuntimeReflection::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__ftable), alignof(Duckvil::RuntimeReflection::__ftable));

    _functions->m_fnInit = &Duckvil::RuntimeReflection::init;

    _functions->m_fnCreateContext = &Duckvil::RuntimeReflection::create_context;

    _functions->m_fnGetTypeHandleByName = &Duckvil::RuntimeReflection::get_type_by_name;
    _functions->m_fnGetPropertyByName = &Duckvil::RuntimeReflection::get_property_by_name;
    _functions->m_fnGetTypeHandleByTypeID = &Duckvil::RuntimeReflection::get_type_by_type_id;
    _functions->m_fnGetType = &Duckvil::RuntimeReflection::get_type_data;
    _functions->m_fnGetTypes = &Duckvil::RuntimeReflection::get_types;
    _functions->m_fnGetPropertyHandleByName = &Duckvil::RuntimeReflection::get_property_handle_by_name;
    _functions->m_fnGetArguments = &Duckvil::RuntimeReflection::get_arguments;
    _functions->m_fnGetArgument = &Duckvil::RuntimeReflection::get_argument;
    _functions->m_fnGetPropertyByName = &Duckvil::RuntimeReflection::get_property_by_name;
    _functions->m_fnGetInheritances = &Duckvil::RuntimeReflection::get_inheritances;
    _functions->m_fnGetInheritance = &Duckvil::RuntimeReflection::get_inheritance;
    _functions->m_fnInherits = &Duckvil::RuntimeReflection::inherits;
    _functions->m_fnInheritsByTypeID = &Duckvil::RuntimeReflection::inherits_by_type_id;

    _functions->m_fnGetConstructors = &Duckvil::RuntimeReflection::get_constructors;
    _functions->m_fnGetConstructorHandleByTypeID = &Duckvil::RuntimeReflection::get_constructor_handle_by_type_id;

    _functions->m_fnGetFunctions = &Duckvil::RuntimeReflection::get_functions;
    _functions->m_fnGetFunctionHandle = &Duckvil::RuntimeReflection::get_function_handle;
    _functions->m_fnGetFunctionCallback = &Duckvil::RuntimeReflection::get_function_callback;
    _functions->m_fnGetFunctionCallbackByHandle = &Duckvil::RuntimeReflection::get_function_callback_by_handle;
    _functions->m_fnGetFunctionByHandle = &Duckvil::RuntimeReflection::get_function_by_handle;

    _functions->m_fnGetProperty = &Duckvil::RuntimeReflection::get_property;

    _functions->m_fnGetTypeMetaHandle = &Duckvil::RuntimeReflection::get_type_meta_handle;
    _functions->m_fnGetTypeMetaValue = &Duckvil::RuntimeReflection::get_type_meta_value;
    _functions->m_fnGetTypeMetaVariant = &Duckvil::RuntimeReflection::get_type_meta_variant;

    _functions->m_fnGetPropertyMetaHandle = &Duckvil::RuntimeReflection::get_property_meta_handle;
    _functions->m_fnGetPropertyMetaValue = &Duckvil::RuntimeReflection::get_property_meta_value;
    _functions->m_fnGetPropertyMetaVariant = &Duckvil::RuntimeReflection::get_property_meta_variant;

    _functions->m_fnGetConstructorMetaHandle = &Duckvil::RuntimeReflection::get_constructor_meta_handle;
    _functions->m_fnGetConstructorMetaValue = &Duckvil::RuntimeReflection::get_constructor_meta_value;
    _functions->m_fnGetConstructorMetaVariant = &Duckvil::RuntimeReflection::get_constructor_meta_variant;

    return _functions;
}