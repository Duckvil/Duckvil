#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#define DUCKVIL_RUNTIME_REFLECTION_RECORD(index) \
    DUCKVIL_EXPORT void duckvil_runtime_reflection_record_##index(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData)

#define DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(count) \
    DUCKVIL_EXPORT uint32_t duckvil_get_runtime_reflection_recorder_count() { return count; }

#define DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF _pMemoryInterface, _pAllocator, _pRecorder, _pData

namespace Duckvil { namespace RuntimeReflection {

    struct __recorder_ftable
    {
        DUCKVIL_RESOURCE(type_t) (*m_fnRecordType)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX]);
        DUCKVIL_RESOURCE(constructor_t) (*m_fnRecordConstructor)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, uint8_t* _pConctructor);
        DUCKVIL_RESOURCE(property_t) (*m_fnRecordProperty)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], uintptr_t _ullAddress);
        DUCKVIL_RESOURCE(namespace_t) (*m_fnRecordNamespace)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, const char _sName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX]);
        DUCKVIL_RESOURCE(inheritance_t) (*m_fnRecordInheritance)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, DUCKVIL_RESOURCE(type_t) _typeHandle, __protection _protection);
    };

    template <typename Type>
    DUCKVIL_RESOURCE(type_t) record_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        return _pFunctions->m_fnRecordType(_pMemoryInterface, _pAllocator, _pData, typeid(Type).hash_code(), _sName);
    }

    template <typename Type, typename... Args>
    DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                return _pFunctions->m_fnRecordConstructor(_pMemoryInterface, _pAllocator, _pData, _type.m_ullTypeID, _type.m_uiSlotIndex, (uint8_t*)&create_type<Type, Args...>);
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename A, typename B>
    DUCKVIL_RESOURCE(property_t) record_property(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, uintptr_t _ullOffset, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX])
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

    template <typename B>
    DUCKVIL_RESOURCE(property_t) record_property(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, uintptr_t _ullOffset, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX])
    {
        static std::size_t _typeB_ID = typeid(B).hash_code();

        return _pFunctions->m_fnRecordProperty(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _typeB_ID, _sName, _ullOffset);
    }

    DUCKVIL_RESOURCE(namespace_t) record_namespace(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX])
    {
        return _pFunctions->m_fnRecordNamespace(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _sName);
    }

    DUCKVIL_RESOURCE(inheritance_t) record_inheritance(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __recorder_ftable* _pFunctions, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritance, __protection _protection)
    {
        return _pFunctions->m_fnRecordInheritance(_pMemoryInterface, _pAllocator, _pData, _typeHandle, _inheritance, _protection);
    }

}}

typedef Duckvil::RuntimeReflection::__recorder_ftable* (*duckvil_runtime_reflection_recorder_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);