#include "RuntimeReflection/Recorder.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) record_type(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        __type_t _type = {};

        _type.m_ullTypeID = _ullTypeID;
        _type.m_constructors = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __constructor_t);
        _type.m_properties = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __property_t);
        _type.m_namespaces = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __namespace_t);
        memcpy(_type.m_sTypeName, _sTypeName, DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX);

        uint32_t _type_handle = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _pData->m_aTypes, _type);

        DUCKVIL_RESOURCE(type_t) _handle = {};

        _handle.m_ID = _type_handle;

        DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _type_handle)->m_uiSlotIndex = _handle;

        return _handle;
    }

    DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(type_t) _owner, uint8_t* _pConctructor)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __constructor_t _constructor = {};

        _constructor.m_ullTypeID = _ullTypeID;
        _constructor.m_owner = _owner;
        _constructor.m_pData = _pConctructor;

        DUCKVIL_RESOURCE(constructor_t) _handle = {};

        _handle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_constructors, _constructor);

        return _handle;
    }

    DUCKVIL_RESOURCE(property_t) record_property(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], uintptr_t _ullAddress)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __property_t _property = {};

        _property.m_ullAddress = _ullAddress;
        _property.m_ullTypeID = _ullTypeID;
        _property.m_owner = _owner;
        memcpy(_property.m_sName, _sName, DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX);

        DUCKVIL_RESOURCE(property_t) _handle = {};

        _handle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_properties, _property);

        return _handle;
    }

    DUCKVIL_RESOURCE(namespace_t) record_namespace(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, const char _sName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX])
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __namespace_t _namespace = {};

        memcpy(_namespace.m_sTypeName, _sName, DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX);

        DUCKVIL_RESOURCE(namespace_t) _handle = {};

        _handle.m_ID = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _type->m_namespaces, _namespace);

        return _handle;
    }

}}

Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__recorder_ftable* _functions = (Duckvil::RuntimeReflection::__recorder_ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__recorder_ftable), alignof(Duckvil::RuntimeReflection::__recorder_ftable));

    _functions->m_fnRecordType = &Duckvil::RuntimeReflection::record_type;
    _functions->m_fnRecordConstructor = &Duckvil::RuntimeReflection::record_constructor;
    _functions->m_fnRecordProperty = &Duckvil::RuntimeReflection::record_property;
    _functions->m_fnRecordNamespace = &Duckvil::RuntimeReflection::record_namespace;

    return _functions;
}