#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace RuntimeReflection {

    bool init(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _pFunctions->m_pData = _data;
        _data->m_aTypes = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __type_t);
        _data->m_aConstructors = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __constructor_t);
        _data->m_aProperties = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __property_t);

        return true;
    }

    DUCKVIL_RESOURCE(type_t) record_type(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        __type_t _type = {};

        _type.m_ullTypeID = _ullTypeID;
        memcpy(_type.m_sTypeName, _sTypeName, DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX);

        uint32_t _type_handle = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _pData->m_aTypes, _type);

        DUCKVIL_RESOURCE(type_t) _handle = {};

        _handle.m_ID = _type_handle;

        DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _type_handle)->m_uiSlotIndex = _handle;

        return _handle;
    }

    DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(type_t) _owner, uint8_t* _pConctructor)
    {
        __constructor_t _constructor = {};

        _constructor.m_ullTypeID = _ullTypeID;
        _constructor.m_owner = _owner;
        _constructor.m_pData = _pConctructor;

        DUCKVIL_RESOURCE(constructor_t) _handle = {};

        _handle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _pData->m_aConstructors, _constructor);

        return _handle;
    }

    DUCKVIL_RESOURCE(property_t) record_property(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], uintptr_t _ullAddress)
    {
        __property_t _property = {};

        _property.m_ullAddress = _ullAddress;
        _property.m_ullTypeID = _ullTypeID;
        _property.m_owner = _owner;
        memcpy(_property.m_sName, _sName, DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX);

        DUCKVIL_RESOURCE(property_t) _handle = {};

        _handle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _pData->m_aProperties, _property);

        return _handle;
    }

    void* get_property(__data* _pData, DUCKVIL_RESOURCE(property_t) _handle, void* _pObject)
    {
        __property_t* _property = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aProperties, _handle.m_ID);

        return (uint8_t*)_pObject + _property->m_ullAddress;
    }

}}

Duckvil::RuntimeReflection::__functions* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__functions* _functions = (Duckvil::RuntimeReflection::__functions*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__functions), alignof(Duckvil::RuntimeReflection::__functions));

    _functions->m_fnInit = &Duckvil::RuntimeReflection::init;
    _functions->m_fnRecordType = &Duckvil::RuntimeReflection::record_type;
    _functions->m_fnRecordConstructor = &Duckvil::RuntimeReflection::record_constructor;
    _functions->m_fnRecordProperty = &Duckvil::RuntimeReflection::record_property;

    _functions->m_fnGetProperty = &Duckvil::RuntimeReflection::get_property;

    return _functions;
}