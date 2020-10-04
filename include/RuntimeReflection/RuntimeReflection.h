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

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type, typename... Args>
    void* create_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, Args... _vArgs)
    {
        void* _ptr = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Type), alignof(Type));

        return new(_ptr) Type(_vArgs...);
    }

    DUCKVIL_RESOURCE_DECLARE(type_t);
    DUCKVIL_RESOURCE_DECLARE(constructor_t);
    DUCKVIL_RESOURCE_DECLARE(property_t);

    slot(__type_t,
    {
        DUCKVIL_RESOURCE(type_t) m_uiSlotIndex;
        std::size_t m_ullTypeID;
        char m_sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX];
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
    });

    struct __data
    {
        DUCKVIL_SLOT_ARRAY(__type_t) m_aTypes;
        DUCKVIL_SLOT_ARRAY(__constructor_t) m_aConstructors;
        DUCKVIL_SLOT_ARRAY(__property_t) m_aProperties;
    };

    struct __functions
    {
        __data* m_pData;
        bool (*m_fnInit)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions);

        DUCKVIL_RESOURCE(type_t) (*m_fnRecordType)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char* _sTypeName);
        DUCKVIL_RESOURCE(constructor_t) (*m_fnRecordConstructor)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(type_t) _owner, uint8_t* _pConctructor);
        DUCKVIL_RESOURCE(property_t) (*m_fnRecordProperty)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char* _sName, uintptr_t _ullAddress);

        void* (*m_fnGetProperty)(__data* _pData, DUCKVIL_RESOURCE(property_t) _handle, void* _pObject);
    };

    DUCKVIL_RESOURCE(type_t) record_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX]);
    DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(type_t) _owner, uint8_t* _pConctructor);
    DUCKVIL_RESOURCE(property_t) record_property(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], uintptr_t _ullAddress);

    void* get_property(__data* _pData, DUCKVIL_RESOURCE(property_t) _handle, void* _pObject);

    bool init(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions);

    template <typename Type>
    DUCKVIL_RESOURCE(type_t) record_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions, const char _sName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        return _pFunctions->m_fnRecordType(_pMemoryInterface, _pAllocator, _pFunctions->m_pData, typeid(Type).hash_code(), _sName);
    }

    template <typename Type, typename... Args>
    DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pFunctions->m_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pFunctions->m_pData->m_aTypes, i);

            if(_type.m_ullTypeID == typeid(Type).hash_code())
            {
                return _pFunctions->m_fnRecordConstructor(_pMemoryInterface, _pAllocator, _pFunctions->m_pData, _type.m_ullTypeID, _type.m_uiSlotIndex, (uint8_t*)&create_type<Type, Args...>);
            }
        }

        return DUCKVIL_RESOURCE(constructor_t) { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename... Args>
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, Args... _vArgs)
    {
        __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aConstructors, _constructorHandle.m_ID);

        void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

        return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
    }

}}

typedef Duckvil::RuntimeReflection::__functions* (*duckvil_runtime_reflection_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__functions* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);