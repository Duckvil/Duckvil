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

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type, typename... Args>
    void* create_type(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, Args... _vArgs)
    {
        return new(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Type), alignof(Type))) Type(_vArgs...);
    }

    DUCKVIL_RESOURCE_DECLARE(type_t);
    DUCKVIL_RESOURCE_DECLARE(constructor_t);
    DUCKVIL_RESOURCE_DECLARE(property_t);
    DUCKVIL_RESOURCE_DECLARE(namespace_t);
    DUCKVIL_RESOURCE_DECLARE(inheritance_t);

    struct __traits
    {
        bool m_bIsPointer;
    };

    enum __protection
    {
        __protection_public,
        __protection_protected,
        __protection_private
    };

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
    });

    slot(__namespace_t,
    {
        char m_sTypeName[DUCKVIL_RUNTIME_REFLECTION_NAMESPACE_NAME_MAX];
    });

    slot(__inheritance_t,
    {
        DUCKVIL_RESOURCE(type_t) m_uiTypeSlotIndex;
        __protection m_protection;
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

    template <typename Type>
    DUCKVIL_RESOURCE(type_t) get_type(__data* _pData)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            // __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            return { i };
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    template <typename Type>
    void* get_property(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], const Type* _pObject)
    {
        static std::size_t _typeID = typeid(Type).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _typeID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); j++)
                {
                    __property_t _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, j);

                    if(strcmp(_property.m_sName, _sName) == 0)
                    {
                        return (uint8_t*)_pObject + _property.m_ullAddress;
                    }
                } 
            }
        }

        return 0;
    }

    template <typename... Args>
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, Args... _vArgs)
    {
        __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aConstructors, _constructorHandle.m_ID);

        void* (*_constructor_callback)(Memory::IMemory*, Memory::__free_list_allocator*, Args...) = (void* (*)(Memory::IMemory*, Memory::__free_list_allocator*, Args...))_constructor.m_pData;

        return _constructor_callback(_pMemoryInterface, _pAllocator, _vArgs...);
    }

    template <typename... Args>
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX], Args... _vArgs)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sTypeName) == 0)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
                {
                    __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

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

    template <typename... Args>
    void* create(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, Args... _vArgs)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_typeHandle.m_ID == _type.m_uiSlotIndex.m_ID)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); j++)
                {
                    __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

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

}}

typedef Duckvil::RuntimeReflection::__ftable* (*duckvil_runtime_reflection_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);