#include "RuntimeReflection/Recorder.h"

#include "tracy/Tracy.hpp"

#include "RuntimeReflection/ReflectionTypeRecordedEvent.h"
#include "RuntimeReflection/ReflectionFileRecordedEvent.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) record_type(const duckvil_runtime_reflection_recorder_stuff& _data, std::size_t _ullTypeID, const char* _sTypeName, std::size_t _ullLength)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data._pData->m_aTypes.m_data); ++i)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_data._pData->m_aTypes, i);

            if(_type.m_ullTypeID == _ullTypeID)
            {
                FrameMarkStart("CleaningOldTypeReflection");

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); ++j)
                {
                    __function_t& _func = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    Memory::free_list_free(_data._pMemoryInterface, _data._pAllocator, _func.m_pFunction);
                    Memory::free_list_free(_data._pMemoryInterface, _data._pAllocator, _func.m_sFunctionName);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); ++j)
                {
                    __variant_t& _variant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, j);

                    Memory::free_list_free(_data._pMemoryInterface, _data._pAllocator, _variant.m_variant.m_pData);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantValues.m_data); ++j)
                {
                    __variant_t& _variant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, j);

                    Memory::free_list_free(_data._pMemoryInterface, _data._pAllocator, _variant.m_variant.m_pData);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); ++j)
                {
                    __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

                    DUCKVIL_SLOT_ARRAY_FREE(_data._pMemoryInterface, _data._pAllocator, _constructor.m_arguments);
                    DUCKVIL_SLOT_ARRAY_FREE(_data._pMemoryInterface, _data._pAllocator, _constructor.m_metas);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++j)
                {
                    __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, j);

                    DUCKVIL_SLOT_ARRAY_FREE(_data._pMemoryInterface, _data._pAllocator, _property.m_metas);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data); ++j)
                {
                    __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j);

                    Memory::free_list_free(_data._pMemoryInterface, _data._pAllocator, _namespace.m_sNamespaceName);
                }

                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_constructors);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_destructors);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_properties);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_namespaces);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_inheritances);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_functions);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_variantKeys);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_variantValues);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_metas);

                FrameMarkEnd("CleaningOldTypeReflection");

                return _type.m_uiSlotIndex;
            }
        }

        __type_t _type = {};

        _type.m_ullTypeID =     _ullTypeID;
        _type.m_constructors =  DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __constructor_t);
        _type.m_destructors =   DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __destructor_t);
        _type.m_properties =    DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __property_t);
        _type.m_namespaces =    DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __namespace_t);
        _type.m_inheritances =  DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __inheritance_t);
        _type.m_functions =     DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __function_t);
        _type.m_variantKeys =   DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __variant_t);
        _type.m_variantValues = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __variant_t);
        _type.m_metas =         DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);

        // _type.m_sTypeName = new char[_ullLength];
        _type.m_sTypeName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_type.m_sTypeName, _sTypeName, _ullLength);

        uint32_t _type_handle = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _data._pData->m_aTypes, _type);

        DUCKVIL_RESOURCE(type_t) _handle = {};

        _handle.m_ID = _type_handle;

        DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _type_handle)->m_uiSlotIndex = _handle;

        _data._pData->m_pTypesByTypeID->insert({ _ullTypeID, _handle });

        return _handle;
    }

    DUCKVIL_RESOURCE(meta_t) record_type_meta(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type->m_variantKeys.m_data); ++i)
        {
            const auto& _m = DUCKVIL_SLOT_ARRAY_GET(_type->m_metas, i);
            const auto& _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, _m.m_key.m_ID);

            if(_key.m_owner == __variant_owner::__variant_owner_type &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
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

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;

                if(_meta.m_pKeyData != nullptr)
                {
					_keyVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                    memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);
                }
                else
                {
                    printf("AAA\n");
                }

                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_type;

                _keyHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_type;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(ntype_t) record_ntype(const duckvil_runtime_reflection_recorder_stuff& _data, const std::vector<const char*>& _aNamespaces)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data._pData->m_aNTypes.m_data); ++i)
        {
            const auto& _t = DUCKVIL_SLOT_ARRAY_GET(_data._pData->m_aNTypes, i);

            if(DUCKVIL_DYNAMIC_ARRAY_SIZE(_t.m_namespaces.m_data) != _aNamespaces.size())
            {
                continue;
            }

            bool _skip = false;

            for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_t.m_namespaces.m_data); ++j)
            {
                const auto& _n = DUCKVIL_SLOT_ARRAY_GET(_t.m_namespaces, j);

                if(strcmp(_n.m_sNamespaceName, _aNamespaces[j]) != 0)
                {
                    _skip = true;

                    break;
                }
            }

            if(!_skip)
            {
                return { i };
            }
        }

        __ntype_t _type = {};

        _type.m_properties =    DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __property_t);
        _type.m_namespaces =    DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __namespace_t);
        _type.m_functions =     DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __function_t);
        _type.m_variantKeys =   DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __variant_t);
        _type.m_variantValues = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __variant_t);

        for(uint32_t i = 0; i < _aNamespaces.size(); ++i)
        {
            std::size_t _len = strlen(_aNamespaces[i]) + 1;
            __namespace_t _namespace = {};

            _namespace.m_sNamespaceName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _len, 8));

            memcpy(_namespace.m_sNamespaceName, _aNamespaces[i], _len);

            duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type.m_namespaces, _namespace);
        }

        return { duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _data._pData->m_aNTypes, _type) };
    }

    DUCKVIL_RESOURCE(meta_t) record_property_meta(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _typeHandle.m_ID);
        __property_t* _property = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_properties, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property->m_metas.m_data); ++i)
        {
            const auto& _m = DUCKVIL_SLOT_ARRAY_GET(_property->m_metas, i);
            const auto& _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, _m.m_key.m_ID);

            if(_key.m_owner == __variant_owner::__variant_owner_property &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
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

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_property;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_property;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _property->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(meta_t) record_constructor_meta(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _typeHandle.m_ID);
        __constructor_t* _property = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_constructors, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_property->m_metas.m_data); ++i)
        {
            const auto& _m = DUCKVIL_SLOT_ARRAY_GET(_property->m_metas, i);
            const auto& _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, _m.m_key.m_ID);

            if(_key.m_owner == __variant_owner::__variant_owner_constructor &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
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

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_constructor;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_constructor;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _property->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(meta_t) record_argument_meta(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _owner, uint32_t _uiArgumentIndex, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _typeHandle.m_ID);
        __constructor_t* _constructor = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_constructors, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_constructor->m_metas.m_data); ++i)
        {
            const auto& _m = DUCKVIL_SLOT_ARRAY_GET(_constructor->m_metas, i);
            const auto& _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, _m.m_key.m_ID);

            if(_key.m_owner == __variant_owner::__variant_owner_constructor_argument &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
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

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_constructor_argument;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_constructor_argument;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            // _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _constructor->m_metas, _metaData);
            __argument_t* _argument = DUCKVIL_SLOT_ARRAY_GET_POINTER(_constructor->m_arguments, _uiArgumentIndex);
            _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _argument->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(meta_t) record_function_meta(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _typeHandle.m_ID);
        __function_t* _function = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_functions, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_function->m_metas.m_data); ++i)
        {
            const auto& _m = DUCKVIL_SLOT_ARRAY_GET(_function->m_metas, i);
            const auto& _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, _m.m_key.m_ID);

            if(_key.m_owner == __variant_owner::__variant_owner_function &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
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

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_function;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_function;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _function->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(meta_t) record_argument_meta(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _owner, uint32_t _uiArgumentIndex, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _typeHandle.m_ID);
        __function_t* _function = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_functions, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_function->m_metas.m_data); ++i)
        {
            const auto& _m = DUCKVIL_SLOT_ARRAY_GET(_function->m_metas, i);
            const auto& _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, _m.m_key.m_ID);

            if(_key.m_owner == __variant_owner::__variant_owner_function_argument &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
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

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_function_argument;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_function_argument;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            // _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _constructor->m_metas, _metaData);
            __argument_t* _argument = DUCKVIL_SLOT_ARRAY_GET_POINTER(_function->m_arguments, _uiArgumentIndex);
            _metaHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _argument->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(constructor_t) record_constructor(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, uint8_t* _pConctructor, Memory::Queue<__argument_t>& _arguments)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __constructor_t _constructor = {};

        _constructor.m_ullTypeID = _ullTypeID;
        _constructor.m_owner = _owner;
        _constructor.m_pData = _pConctructor;
        _constructor.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);
        _constructor.m_arguments = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __argument_t);

        while(!_arguments.Empty())
        {
            auto& _argument = _arguments.Begin();

            _argument.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);

            DUCKVIL_RESOURCE(argument_t) _argumentHandle = {};

            _argumentHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _constructor.m_arguments, _argument);

            _arguments.Pop();
        }

        DUCKVIL_RESOURCE(constructor_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_constructors, _constructor);

        return _handle;
    }

    DUCKVIL_RESOURCE(destructor_t) record_destructor(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, uint8_t* _pDectructor)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __destructor_t _destructor = {};

        _destructor.m_owner = _owner;
        _destructor.m_pData = _pDectructor;
        _destructor.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);

        DUCKVIL_RESOURCE(destructor_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_destructors, _destructor);

        return _handle;
    }

    DUCKVIL_RESOURCE(property_t) record_property_by_offset(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char* _sName, std::size_t _ullLength, uintptr_t _ullOffset, const property_traits& _traits)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __property_t _property = {};

        _property.m_ullAddress = _ullOffset;
        _property.m_ullTypeID = _ullTypeID;
        _property.m_owner = _owner;
        _property.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);
        _property.m_traits = _traits;

        // _property.m_sName = new char[_ullLength];
        _property.m_sName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_property.m_sName, _sName, _ullLength);

        DUCKVIL_RESOURCE(property_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_properties, _property);

        return _handle;
    }

    DUCKVIL_RESOURCE(property_t) record_property_by_address(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char* _sName, std::size_t _ullLength, uintptr_t _ullAddress, const property_traits& _traits)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __property_t _property = {};

        _property.m_ullAddress = _ullAddress;
        _property.m_ullTypeID = _ullTypeID;
        _property.m_owner = _owner;
        _property.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);
        _property.m_traits = _traits | property_traits::is_static;

        // _property.m_sName = new char[_ullLength];
        _property.m_sName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_property.m_sName, _sName, _ullLength);

        DUCKVIL_RESOURCE(property_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_properties, _property);

        return _handle;
    }

    DUCKVIL_RESOURCE(namespace_t) record_namespace(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, const char* _sName, std::size_t _ullLength)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __namespace_t _namespace = {};

        // _namespace.m_sNamespaceName = new char[_ullLength];
        _namespace.m_sNamespaceName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_namespace.m_sNamespaceName, _sName, _ullLength);

        DUCKVIL_RESOURCE(namespace_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_namespaces, _namespace);

        return _handle;
    }

    DUCKVIL_RESOURCE(inheritance_t) record_inheritance(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullInheritanceTypeID, __protection _protection)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __inheritance_t _inheritance = {};

        _inheritance.m_ullInheritanceTypeID = _ullInheritanceTypeID;
        _inheritance.m_protection = _protection;

        DUCKVIL_RESOURCE(inheritance_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_inheritances, _inheritance);

        return _handle;
    }

    DUCKVIL_RESOURCE(function_t) record_function(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, __ifunction* _pFunction, const char* _sName, std::size_t _ullLength, std::size_t _ullReturnTypeID, std::size_t _ullArgumentsTypeID, Memory::Queue<__argument_t>& _arguments, const function_traits& _traits)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aTypes, _owner.m_ID);
        __function_t _function = {};

        _function.m_pFunction = _pFunction;
        _function.m_ullReturnTypeID = _ullReturnTypeID;
        _function.m_ullArgumentsTypeID = _ullArgumentsTypeID;
        _function.m_pRawFunction = _pFunction->GetRawPointer();
        _function.m_traits = _traits;

        // _function.m_sFunctionName = new char[_ullLength];
        _function.m_sFunctionName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_function.m_sFunctionName, _sName, _ullLength);

        while(!_arguments.Empty())
        {
            auto& _argument = _arguments.Begin();

            _argument.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);

            DUCKVIL_RESOURCE(argument_t) _argumentHandle = {};

            _argumentHandle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _function.m_arguments, _argument);

            _arguments.Pop();
        }

        DUCKVIL_RESOURCE(function_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_functions, _function);

        return _handle;
    }

    DUCKVIL_RESOURCE(enum_t) record_enum(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(ntype_t) _owner, std::size_t _ullTypeID, const char* _sName, std::size_t _ullLength)
    {
        __ntype_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aNTypes, _owner.m_ID);
        __enum_t _enum = {};

        _enum.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);
        _enum.m_elements = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __enum_element_t);
        _enum.m_ullTypeID = _ullTypeID;
        _enum.m_sName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_enum.m_sName, _sName, _ullLength);

        return { duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _type->m_enums, _enum) };
    }

    DUCKVIL_RESOURCE(enum_element_t) record_enum_element(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(ntype_t) _owner, DUCKVIL_RESOURCE(enum_t) _enumHandle, void* _pValue, std::size_t _ullTypeSize, const char* _sName, std::size_t _ullLength)
    {
        __ntype_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_data._pData->m_aNTypes, _owner.m_ID);
        __enum_t* _enum = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_enums, _enumHandle.m_ID);
        __enum_element_t _element = {};

        _element.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_data._pMemoryInterface, _data._pAllocator, __meta_t);;
        _element.m_pValue = static_cast<void*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullTypeSize, 8));

        memcpy(_element.m_pValue, _pValue, _ullTypeSize);

        _element.m_sName = static_cast<char*>(_data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, _ullLength, 8));

        memcpy(_element.m_sName, _sName, _ullLength);

        DUCKVIL_RESOURCE(enum_element_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_data._pMemoryInterface, _data._pAllocator, _enum->m_elements, _element);

        return _handle;
    }

    duckvil_recorderd_types record(const duckvil_runtime_reflection_recorder_stuff& _stuff, const PlugNPlay::__module_information& _module, RuntimeReflection::RecordFunction _fnRecorder)
    {
        duckvil_recorderd_types _types = _fnRecorder(_stuff);

        _types.m_pModule = &_module;

        uint32_t _index = 0;

        while(_index < _types.m_ullCount)
        {
            auto _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_stuff._pData->m_aTypes, _types.m_aTypes[_index].m_ID);

            _type->m_pModule = &_module;

            static_cast<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_stuff._pData->m_pEvents)->Broadcast(ReflectionTypeRecordedEvent{ _type->m_uiSlotIndex });

            _index++;
        }

        static_cast<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_stuff._pData->m_pEvents)->Broadcast(ReflectionFileRecordedEvent(_types));

        return _types;
    }

    Memory::Vector<duckvil_recorderd_types> record_module(const duckvil_runtime_reflection_recorder_stuff& _stuff, const PlugNPlay::__module& _moduleTable, const PlugNPlay::__module_information& _module, const Memory::FreeList& _heap)
    {
        RuntimeReflection::GetRecordersCountFunction get_recorder_count = nullptr;

        _moduleTable.get(_module, "duckvil_get_runtime_reflection_recorders_count", reinterpret_cast<void**>(&get_recorder_count));

        if(get_recorder_count == nullptr)
        {
            throw recorders_count_getter_not_found();
        }

        const uint32_t _recordersCount = get_recorder_count();
        
        Memory::Vector<duckvil_recorderd_types> _res(_heap.GetMemoryInterface(), _heap.GetAllocator(), _recordersCount);

        for(uint32_t j = 0; j < _recordersCount; ++j)
        {
            RuntimeReflection::RecordFunction record = nullptr;

            _moduleTable.get(_module, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if(record == nullptr)
            {
                continue;
            }

            if(_res.Full())
            {
                _res.Resize(_res.Size() * 2);
            }

            _res.Allocate(_stuff._pFunctions->m_fnRecordFile(_stuff, _module, record));
        }

        return _res;
    }

}}

const Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init()
{
    static Duckvil::RuntimeReflection::__recorder_ftable _functions = { 0 };

    _functions.m_fnRecordType = &Duckvil::RuntimeReflection::record_type;
    _functions.m_fnRecordConstructor = &Duckvil::RuntimeReflection::record_constructor;
    _functions.m_fnRecordDestructor = &Duckvil::RuntimeReflection::record_destructor;
    _functions.m_fnRecordPropertyByOffset = &Duckvil::RuntimeReflection::record_property_by_offset;
    _functions.m_fnRecordPropertyByAddress = &Duckvil::RuntimeReflection::record_property_by_address;
    _functions.m_fnRecordNamespace = &Duckvil::RuntimeReflection::record_namespace;
    _functions.m_fnRecordInheritance = &Duckvil::RuntimeReflection::record_inheritance;
    _functions.m_fnRecordFunction = &Duckvil::RuntimeReflection::record_function;
    _functions.m_fnRecordTypeMeta = &Duckvil::RuntimeReflection::record_type_meta;
    _functions.m_fnRecordPropertyMeta = &Duckvil::RuntimeReflection::record_property_meta;
    _functions.m_fnRecordConstructorMeta = &Duckvil::RuntimeReflection::record_constructor_meta;
    _functions.m_fnRecordConstructorArgumentMeta = &Duckvil::RuntimeReflection::record_argument_meta;
    _functions.m_fnRecordFunctionMeta = &Duckvil::RuntimeReflection::record_function_meta;
    _functions.m_fnRecordFunctionArgumentMeta = &Duckvil::RuntimeReflection::record_argument_meta;

    _functions.m_fnRecordNType = &Duckvil::RuntimeReflection::record_ntype;
    _functions.m_fnRecordNEnum = &Duckvil::RuntimeReflection::record_enum;
    _functions.m_fnRecordNEnumElement = &Duckvil::RuntimeReflection::record_enum_element;

    _functions.m_fnRecordFile = &Duckvil::RuntimeReflection::record;
    _functions.m_fnRecordModule = &Duckvil::RuntimeReflection::record_module;

    return &_functions;
}