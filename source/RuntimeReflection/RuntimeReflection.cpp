#include "RuntimeReflection/RuntimeReflection.h"

// #ifndef g_pDuckvilFrontendReflectionContext
// duckvil_frontend_reflection_context* g_pDuckvilFrontendReflectionContext = nullptr;
// #endif

#define DUCKVIL_META_DEFINE_UTIL(name, arr) \
    DUCKVIL_RESOURCE(variant_t) get_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(name ## _t) _ ## name ## Handle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID) \
    { \
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID); \
        const __ ## name ## _t& _ ## name = DUCKVIL_SLOT_ARRAY_GET(_type.arr, _ ## name ## Handle.m_ID); \
 \
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_ ## name.m_metas.m_data); ++i) \
        { \
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_ ## name.m_metas, i); \
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID); \
 \
            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0) \
            { \
                return { i }; \
            } \
        } \
 \
        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE }; \
    } \
 \
    void* get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(name ## _t) _ ## name ## Handle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID) \
    { \
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID); \
        const __ ## name ## _t& _ ## name = DUCKVIL_SLOT_ARRAY_GET(_type.arr, _ ## name ## Handle.m_ID); \
 \
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_ ## name.m_metas.m_data); ++i) \
        { \
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_ ## name.m_metas, i); \
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID); \
 \
            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0) \
            { \
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID); \
 \
                return _valueVariant.m_variant.m_pData; \
            } \
        } \
 \
        return nullptr; \
    } \
 \
    const __variant& get_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(name ## _t) _ ## name ## Handle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID) \
    { \
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID); \
        const __ ## name ## _t& _ ## name = DUCKVIL_SLOT_ARRAY_GET(_type.arr, _ ## name ## Handle.m_ID); \
        static __variant _result; \
 \
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_ ## name.m_metas.m_data); ++i) \
        { \
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_ ## name.m_metas, i); \
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID); \
 \
            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0) \
            { \
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID); \
 \
                return _valueVariant.m_variant; \
            } \
        } \
 \
        return _result; \
    } \
 \
    Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(name ## _t) _ ## name ## Handle) \
    { \
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID); \
        const __ ## name ## _t& _ ## name  = DUCKVIL_SLOT_ARRAY_GET(_type.arr, _ ## name ## Handle.m_ID); \
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> _result; \
 \
        _heap.Allocate(_result, 1); \
 \
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_ ## name.m_metas.m_data); ++i) \
        { \
            if(_result.Full()) \
            { \
                _result.Resize(_result.Size() * 2); \
            } \
 \
            _result.Allocate({ i }); \
        } \
 \
        return _result; \
    } \
 \
    const __meta_t& get_meta(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(name ## _t) _ ## name ## Handle, DUCKVIL_RESOURCE(meta_t) _handle) \
    { \
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID); \
        const __ ## name ## _t& _ ## name  = DUCKVIL_SLOT_ARRAY_GET(_type.arr, _ ## name ## Handle.m_ID); \
 \
        return DUCKVIL_SLOT_ARRAY_GET(_ ## name.m_metas, _handle.m_ID); \
    }

namespace Duckvil { namespace RuntimeReflection {

    __data* init(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pFunctions)
    {
        __data* _data = static_cast<__data*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data)));

        _data->m_aTypes = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __type_t);
        _data->m_aObjects = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __object_t);
        // _data->m_aFrontend = Memory::Vector<IReflectedType*>(_pMemoryInterface, _pAllocator, 1);

        return _data;
    }

    duckvil_frontend_reflection_context* create_context(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pFTable, __data* _pData)
    {
        duckvil_frontend_reflection_context* _ctx =
            static_cast<duckvil_frontend_reflection_context*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(duckvil_frontend_reflection_context), alignof(duckvil_frontend_reflection_context)));

        _ctx->m_pReflection = _pFTable;
        _ctx->m_pReflectionData = _pData;

        return _ctx;
    }

    DUCKVIL_RESOURCE(type_t) get_type_by_name(__data* _pData, const char* _sName, std::size_t _ullLength)
    {
        // DUCKVIL_DYNAMIC_ARRAY_FOR(_pData->m_aTypes.m_data, __type_t, _type)
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

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

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator)
    {
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

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

    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
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

    const __constructor_t& get_constructor(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
    }

    DUCKVIL_RESOURCE(destructor_t) get_destructor_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_destructors.m_data); ++i)
        {
            return { i };
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
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

    const void* get_property_value(__data* _pData, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID, const void* _pObject)
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
                        return static_cast<const uint8_t*>(_pObject) + _property.m_ullAddress;
                    }
                }
            }
        }

        return nullptr;
    }

    Memory::Vector<DUCKVIL_RESOURCE(property_t)> get_properties(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(property_t)> _properties(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _properties.Allocate({ i });
        }

        return _properties;
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _handle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor.m_arguments.m_data);
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
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _handle.m_ID);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _handle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_function.m_arguments.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> _arguments(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _arguments.Allocate({ i });
        }

        return _arguments;
    }

    const __argument_t& get_argument(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_function.m_arguments, _handle.m_ID);
    }

    const void* get_property_value_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _type_handle.m_ID);
        const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);

        return static_cast<const uint8_t*>(_pObject) + _property.m_ullAddress;
    }

    const void* get_property_value_by_name(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, const void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++i)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return static_cast<const uint8_t*>(_pObject) + _property.m_ullAddress;
            }
        }

        return nullptr;
    }

    const __property_t& get_property_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);
    }

    Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> get_inheritances(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
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

    const __variant_t& get_variant_key_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _handle.m_ID);
    }

    const __variant_t& get_variant_value_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _handle.m_ID);
    }



    //////////////////////////////////////
    /////////////// META /////////////////
    //////////////////////////////////////

// Type
    DUCKVIL_RESOURCE(variant_t) get_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
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

    void* get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
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

    const __variant& get_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
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

    const __meta_t& get_meta(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(meta_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_metas, _handle.m_ID);
    }

    Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> _result;

        _heap.Allocate(_result, 1);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_metas.m_data); ++i)
        {
            if(_result.Full())
            {
                _result.Resize(_result.Size() * 2);
            }

            _result.Allocate({ i });
        }

        return _result;
    }

// Constructor
    DUCKVIL_RESOURCE(variant_t) get_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _uiArgumentIndex);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    void* get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _uiArgumentIndex);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant.m_pData;
            }
        }

        return nullptr;
    }

    const __variant& get_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _uiArgumentIndex);
        static __variant _result;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant;
            }
        }

        return _result;
    }

    Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _uiArgumentIndex);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> _result;

        _heap.Allocate(_result, 1);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            if(_result.Full())
            {
                _result.Resize(_result.Size() * 2);
            }

            _result.Allocate({ i });
        }

        return _result;
    }

    const __meta_t& get_meta(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, DUCKVIL_RESOURCE(meta_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _uiArgumentIndex);

        return DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, _handle.m_ID);
    }

// Function
    DUCKVIL_RESOURCE(variant_t) get_meta_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_function.m_arguments, _uiArgumentIndex);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    void* get_meta_value(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_function.m_arguments, _uiArgumentIndex);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant.m_pData;
            }
        }

        return nullptr;
    }

    const __variant& get_meta_variant(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_function.m_arguments, _uiArgumentIndex);
        static __variant _result;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            const __meta_t& _meta = DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, i);
            const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, _meta.m_key.m_ID);

            if(_keyVariant.m_variant.m_ullTypeID == _ullTypeID && _keyVariant.m_variant.m_ullSize == _ullSize && memcmp(_keyVariant.m_variant.m_pData, _pKey, _ullSize) == 0)
            {
                const __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, _meta.m_key.m_ID);

                return _valueVariant.m_variant;
            }
        }

        return _result;
    }

    Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_metas(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_function.m_arguments, _uiArgumentIndex);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> _result;

        _heap.Allocate(_result, 1);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_argument.m_metas.m_data); ++i)
        {
            if(_result.Full())
            {
                _result.Resize(_result.Size() * 2);
            }

            _result.Allocate({ i });
        }

        return _result;
    }

    const __meta_t& get_meta(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, DUCKVIL_RESOURCE(meta_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);
        const __argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_function.m_arguments, _uiArgumentIndex);

        return DUCKVIL_SLOT_ARRAY_GET(_argument.m_metas, _handle.m_ID);
    }

    DUCKVIL_META_DEFINE_UTIL(property, m_properties)
    DUCKVIL_META_DEFINE_UTIL(function, m_functions)
    DUCKVIL_META_DEFINE_UTIL(constructor, m_constructors)

// Object
    DUCKVIL_RESOURCE(meta_t) add_object_meta(Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, const __object_t* _pObject, uint32_t _uiObjectHandle, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, uint8_t _ucKeyAlignment, property_traits _keyTraits, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData)
    {
        DUCKVIL_RESOURCE(variant_t) _keyHandle = {};
        DUCKVIL_RESOURCE(variant_t) _valueHandle = {};

        {
            __variant_t _variant = {};

            _variant.m_variant.m_ullTypeID = _ullKeyTypeID;
            _variant.m_variant.m_ullSize = _ullKeySize;
            _variant.m_variant.m_pData = _pMemory->m_fnFreeListAllocate_(_pAllocator, _variant.m_variant.m_ullSize, _ucKeyAlignment);
            _variant.m_variant.m_traits = _keyTraits;
            _variant.m_owner = __variant_owner::__variant_owner_object;

            memcpy(_variant.m_variant.m_pData, _pKeyData, _variant.m_variant.m_ullSize);

            _keyHandle.m_ID = duckvil_slot_array_insert(_pMemory, _pAllocator, _pObject->m_variantKeys, _variant);
        }

        {
            __variant_t _variant = {};

            _variant.m_variant.m_ullTypeID = _ullValueTypeID;
            _variant.m_variant.m_ullSize = _ullValueSize;
            _variant.m_variant.m_pData = _pMemory->m_fnFreeListAllocate_(_pAllocator, _variant.m_variant.m_ullSize, _ucValueAlignment);
            _variant.m_variant.m_traits = _valueTraits;
            _variant.m_owner = __variant_owner::__variant_owner_object;

            memcpy(_variant.m_variant.m_pData, _pValueData, _variant.m_variant.m_ullSize);

            _valueHandle.m_ID = duckvil_slot_array_insert(_pMemory, _pAllocator, _pObject->m_variantValues, _variant);
        }

        if(_keyHandle.m_ID != _valueHandle.m_ID)
        {
            // Something gone wrong... should not happen

            return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
        }

        __meta_t _metaData = {};

        _metaData.m_key = _keyHandle;
        _metaData.m_value = _valueHandle;
        _metaData.m_uiOwner = _uiObjectHandle;

        DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

        _metaHandle.m_ID = duckvil_slot_array_insert(_pMemory, _pAllocator, _pObject->m_metas, _metaData);

        return _metaHandle;
    }

    DUCKVIL_RESOURCE(meta_t) add_object_meta(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, uint8_t _ucKeyAlignment, property_traits _keyTraits, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData)
    {
        bool _found = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aObjects.m_data); ++i)
        {
            const __object_t& _object = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aObjects, i);

            if(_object.m_pObject == _pObject)
            {
                _found = true;

                return add_object_meta(
                    _pMemory,
                    _pAllocator,
                    &_object,
                    i,
                    _ullKeyTypeID,
                    _ullKeySize,
                    _ucKeyAlignment,
                    _keyTraits,
                    _pKeyData,
                    _ullValueTypeID,
                    _ullValueSize,
                    _ucValueAlignment,
                    _valueTraits,
                    _pValueData
                );
            }
        }

        if(!_found)
        {
            __object_t _object = {};

            _object.m_pObject = _pObject;
            _object.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_pMemory, _pAllocator, __meta_t);
            _object.m_variantKeys = DUCKVIL_SLOT_ARRAY_NEW(_pMemory, _pAllocator, __variant_t);
            _object.m_variantValues = DUCKVIL_SLOT_ARRAY_NEW(_pMemory, _pAllocator, __variant_t);

            uint32_t _objectHandle = duckvil_slot_array_insert(_pMemory, _pAllocator, _pData->m_aObjects, _object);

            const auto* _objectPointer = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aObjects, _objectHandle);

            return add_object_meta(
                _pMemory,
                _pAllocator,
                _objectPointer,
                _objectHandle,
                _ullKeyTypeID,
                _ullKeySize,
                _ucKeyAlignment,
                _keyTraits,
                _pKeyData,
                _ullValueTypeID,
                _ullValueSize,
                _ucValueAlignment,
                _valueTraits,
                _pValueData
            );
        }
    }

    const __variant_t& get_object_meta(__data* _pData, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData)
    {
        static __variant_t _invalid;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aObjects.m_data); ++i)
        {
            const __object_t& _object = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aObjects, i);

            if(_object.m_pObject == _pObject)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_object.m_variantKeys.m_data); ++j)
                {
                    const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_object.m_variantKeys, j);

                    if(_keyVariant.m_variant.m_ullTypeID == _ullKeyTypeID && _keyVariant.m_variant.m_ullSize == _ullKeySize && memcmp(_keyVariant.m_variant.m_pData, _pKeyData, _ullKeySize) == 0)
                    {
                        return DUCKVIL_SLOT_ARRAY_GET(_object.m_variantValues, j);
                    }
                }
            }
        }

        return _invalid;
    }

    void set_object_meta(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aObjects.m_data); ++i)
        {
            const __object_t& _object = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aObjects, i);

            if(_object.m_pObject == _pObject)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_object.m_variantKeys.m_data); ++j)
                {
                    const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_object.m_variantKeys, j);

                    if(_keyVariant.m_variant.m_ullTypeID == _ullKeyTypeID && _keyVariant.m_variant.m_ullSize == _ullKeySize && memcmp(_keyVariant.m_variant.m_pData, _pKeyData, _ullKeySize) == 0)
                    {
                        __variant_t& _valueVariant = DUCKVIL_SLOT_ARRAY_GET(_object.m_variantValues, j);
                        void* _oldData = _valueVariant.m_variant.m_pData;

                        _valueVariant.m_variant.m_ullTypeID = _ullValueTypeID;
                        _valueVariant.m_variant.m_ullSize = _ullValueSize;
                        _valueVariant.m_variant.m_pData = _pMemory->m_fnFreeListAllocate_(_pAllocator, _valueVariant.m_variant.m_ullSize, _ucValueAlignment);
                        _valueVariant.m_variant.m_traits = _valueTraits;
                        _valueVariant.m_owner = __variant_owner::__variant_owner_object;

                        memcpy(_valueVariant.m_variant.m_pData, _pValueData, _valueVariant.m_variant.m_ullSize);

                        _pMemory->m_fnFreeListFree_(_pAllocator, _oldData);

                        // _valueHandle.m_ID = duckvil_slot_array_insert(_pMemory, _pAllocator, _pObject->m_variantValues, _variant);
                    }
                }
            }
        }
    }

    void remove_object_meta(__data* _pData, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aObjects.m_data); ++i)
        {
            const __object_t& _object = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aObjects, i);

            if(_object.m_pObject == _pObject)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_object.m_variantKeys.m_data); ++j)
                {
                    const __variant_t& _keyVariant = DUCKVIL_SLOT_ARRAY_GET(_object.m_variantKeys, j);

                    if(_keyVariant.m_variant.m_ullTypeID == _ullKeyTypeID && _keyVariant.m_variant.m_ullSize == _ullKeySize && memcmp(_keyVariant.m_variant.m_pData, _pKeyData, _ullKeySize) == 0)
                    {
                        duckvil_slot_array_erase(_object.m_variantKeys, j);
                        duckvil_slot_array_erase(_object.m_variantValues, j);
                        duckvil_slot_array_erase(_object.m_metas, j);
                    }
                }
            }
        }
    }

    void clear_object_metas(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aObjects.m_data); ++i)
        {
            __object_t& _object = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aObjects, i);

            if(_object.m_pObject == _pObject)
            {
                DUCKVIL_SLOT_ARRAY_CLEAR(_object.m_variantKeys);
                DUCKVIL_SLOT_ARRAY_CLEAR(_object.m_variantValues);
                DUCKVIL_SLOT_ARRAY_CLEAR(_object.m_metas);

                DUCKVIL_SLOT_ARRAY_FREE(_pMemory, _pAllocator, _object.m_variantKeys);
                DUCKVIL_SLOT_ARRAY_FREE(_pMemory, _pAllocator, _object.m_variantValues);
                DUCKVIL_SLOT_ARRAY_FREE(_pMemory, _pAllocator, _object.m_metas);

                duckvil_slot_array_erase(_pData->m_aObjects, i);
            }
        }
    }

}}

#define DUCKVIL_ASSIGN(ftable, name) \
    ftable.m_fnGet ## name ## MetaHandle = &Duckvil::RuntimeReflection::get_meta_handle; \
    ftable.m_fnGet ## name ## MetaValue = &Duckvil::RuntimeReflection::get_meta_value; \
    ftable.m_fnGet ## name ## MetaVariant = &Duckvil::RuntimeReflection::get_meta_variant; \
    ftable.m_fnGet ## name ## MetaByHandle = &Duckvil::RuntimeReflection::get_meta; \
    ftable.m_fnGet ## name ## Metas = &Duckvil::RuntimeReflection::get_metas; \

Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init()
{
    static Duckvil::RuntimeReflection::__ftable _functions = { 0 };

    _functions.m_fnInit = &Duckvil::RuntimeReflection::init;

    _functions.m_fnCreateContext = &Duckvil::RuntimeReflection::create_context;

// Type
    _functions.m_fnGetTypeHandleByName = &Duckvil::RuntimeReflection::get_type_by_name;
    _functions.m_fnGetTypeHandleByTypeID = &Duckvil::RuntimeReflection::get_type_by_type_id;
    _functions.m_fnGetTypeByHandle = &Duckvil::RuntimeReflection::get_type_data;
    _functions.m_fnGetTypes = &Duckvil::RuntimeReflection::get_types;

// Argument
    _functions.m_fnGetConstructorArguments = &Duckvil::RuntimeReflection::get_arguments;
    _functions.m_fnGetConstructorArgument = &Duckvil::RuntimeReflection::get_argument;

    _functions.m_fnGetFunctionArguments = &Duckvil::RuntimeReflection::get_arguments;
    _functions.m_fnGetFunctionArgument = &Duckvil::RuntimeReflection::get_argument;

// Inheritance
    _functions.m_fnGetInheritances = &Duckvil::RuntimeReflection::get_inheritances;
    _functions.m_fnGetInheritance = &Duckvil::RuntimeReflection::get_inheritance;
    _functions.m_fnInherits = &Duckvil::RuntimeReflection::inherits;
    _functions.m_fnInheritsByTypeID = &Duckvil::RuntimeReflection::inherits_by_type_id;

// Constructor
    _functions.m_fnGetConstructors = &Duckvil::RuntimeReflection::get_constructors;
    _functions.m_fnGetConstructorHandleByTypeID = &Duckvil::RuntimeReflection::get_constructor_handle_by_type_id;
    _functions.m_fnGetConstructor = &Duckvil::RuntimeReflection::get_constructor;

// Function
    _functions.m_fnGetFunctions = &Duckvil::RuntimeReflection::get_functions;
    _functions.m_fnGetFunctionHandle = &Duckvil::RuntimeReflection::get_function_handle;
    _functions.m_fnGetFunctionCallback = &Duckvil::RuntimeReflection::get_function_callback;
    _functions.m_fnGetFunctionCallbackByHandle = &Duckvil::RuntimeReflection::get_function_callback_by_handle;
    _functions.m_fnGetFunctionByHandle = &Duckvil::RuntimeReflection::get_function_by_handle;

// Property
    _functions.m_fnGetPropertyValue = &Duckvil::RuntimeReflection::get_property_value;
    _functions.m_fnGetPropertyValueByName = &Duckvil::RuntimeReflection::get_property_value_by_name;
    _functions.m_fnGetPropertyValueByHandle = &Duckvil::RuntimeReflection::get_property_value_by_handle;
    _functions.m_fnGetProperties = &Duckvil::RuntimeReflection::get_properties;
    _functions.m_fnGetPropertyHandleByName = &Duckvil::RuntimeReflection::get_property_handle_by_name;
    _functions.m_fnGetPropertyByHandle = &Duckvil::RuntimeReflection::get_property_by_handle;

// Variant
    _functions.m_fnGetVariantKeyByHandle = &Duckvil::RuntimeReflection::get_variant_key_by_handle;
    _functions.m_fnGetVariantValueByHandle = &Duckvil::RuntimeReflection::get_variant_value_by_handle;

// Meta
    DUCKVIL_ASSIGN(_functions, Type)
    DUCKVIL_ASSIGN(_functions, Property)
    DUCKVIL_ASSIGN(_functions, Constructor)
    DUCKVIL_ASSIGN(_functions, ConstructorArgument)
    DUCKVIL_ASSIGN(_functions, Function)
    DUCKVIL_ASSIGN(_functions, FunctionArgument)

    _functions.m_fnAddObjectMeta = &Duckvil::RuntimeReflection::add_object_meta;
    _functions.m_fnGetObjectMeta = &Duckvil::RuntimeReflection::get_object_meta;
    _functions.m_fnSetObjectMeta = &Duckvil::RuntimeReflection::set_object_meta;
    _functions.m_fnRemoveObjectMeta = &Duckvil::RuntimeReflection::remove_object_meta;
    _functions.m_fnClearObjectMetas = &Duckvil::RuntimeReflection::clear_object_metas;

    return &_functions;
}