#pragma once

#include "Memory/SlotArray.h"

#include "RuntimeReflection/Function.h"
#include "RuntimeReflection/Trait.h"

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include <unordered_map>

#define DUCKVIL_META_CAT(...) __VA_ARGS__

#define DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(...) typeid(void(__VA_ARGS__)).hash_code()

#define DUCKVIL_META_DECLARE_UTIL(name, type) \
    DUCKVIL_RESOURCE(variant_t) (*m_fnGet ## name ## MetaHandle)(__data* _pData, type, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID); \
    void* (*m_fnGet ## name ## MetaValue)(__data* _pData, type, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID); \
    const __variant& (*m_fnGet ## name ## MetaVariant)(__data* _pData, type, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID); \
    const __meta_t& (*m_fnGet ## name ## MetaByHandle)(__data* _pData, type, DUCKVIL_RESOURCE(meta_t) _handle); \
    Memory::Vector<DUCKVIL_RESOURCE(meta_t)> (*m_fnGet ## name ## Metas)(const Memory::FreeList& _heap, __data* _pData, type);

#define DUCKVIL_RESOURCE(t) \
    __duckvil_resource_##t

#define DUCKVIL_RESOURCE_DECLARE(t) \
    struct DUCKVIL_RESOURCE(t) \
    { \
        uint32_t m_ID; \
    }

#define slot(T, ...) \
    struct T \
        __VA_ARGS__ \
    ; \
    DUCKVIL_SLOT_ARRAY_DECLARE(T)

namespace Duckvil { namespace RuntimeReflection {

    struct __ftable;
    struct __data;
    struct __recorder_ftable;

}}

struct duckvil_frontend_reflection_context
{
    Duckvil::RuntimeReflection::__ftable* m_pReflection = nullptr;
    Duckvil::RuntimeReflection::__data* m_pReflectionData = nullptr;
    Duckvil::RuntimeReflection::__recorder_ftable* m_pRecorder = nullptr;
};

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE_DECLARE(type_t);
    DUCKVIL_RESOURCE_DECLARE(constructor_t);
    DUCKVIL_RESOURCE_DECLARE(destructor_t);
    DUCKVIL_RESOURCE_DECLARE(property_t);
    DUCKVIL_RESOURCE_DECLARE(namespace_t);
    DUCKVIL_RESOURCE_DECLARE(inheritance_t);
    DUCKVIL_RESOURCE_DECLARE(function_t);
    DUCKVIL_RESOURCE_DECLARE(variant_t);
    DUCKVIL_RESOURCE_DECLARE(meta_t);
    DUCKVIL_RESOURCE_DECLARE(argument_t);
    DUCKVIL_RESOURCE_DECLARE(enum_t);
    DUCKVIL_RESOURCE_DECLARE(enum_element_t);
    DUCKVIL_RESOURCE_DECLARE(ntype_t);

    enum __protection : uint8_t
    {
        __protection_public,
        __protection_protected,
        __protection_private
    };

    enum __variant_owner : uint8_t
    {
        __variant_owner_object,
        __variant_owner_type,
        __variant_owner_property,
        __variant_owner_constructor,
        __variant_owner_constructor_argument,
        __variant_owner_function,
        __variant_owner_function_argument
    };

    struct __variant
    {
        __variant()
        {
            m_pData = nullptr;
            m_ullTypeID = -1;
            m_ullSize = 0;
            m_traits = (property_traits)0;
        }

        std::size_t m_ullSize;
        std::size_t m_ullTypeID;
        void* m_pData;
        property_traits m_traits;
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

    slot(__argument_t,
    {
        std::size_t m_ullTypeID;
        property_traits m_traits;
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
    });

    slot(__constructor_t,
    {
        DUCKVIL_SLOT_ARRAY(__argument_t) m_arguments;
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        std::size_t m_ullTypeID;
        uint8_t* m_pData;
        DUCKVIL_RESOURCE(type_t) m_owner;
    });

    slot(__destructor_t,
    {
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        uint8_t* m_pData;
        DUCKVIL_RESOURCE(type_t) m_owner;
    });

    slot(__property_t,
    {
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        std::size_t m_ullTypeID;
        char* m_sName;
        uintptr_t m_ullAddress;
        DUCKVIL_RESOURCE(type_t) m_owner;
        property_traits m_traits;
    });

    slot(__namespace_t,
    {
        char* m_sNamespaceName;
    });

    slot(__inheritance_t,
    {
        // DUCKVIL_RESOURCE(type_t) m_uiTypeSlotIndex; // TODO: Check if set
        std::size_t m_ullInheritanceTypeID;
        __protection m_protection;
    });

    slot(__function_t,
    {
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        DUCKVIL_SLOT_ARRAY(__argument_t) m_arguments;
        __ifunction* m_pFunction;
        char* m_sFunctionName;
        std::size_t m_ullReturnTypeID;
        std::size_t m_ullArgumentsTypeID;
        // DUCKVIL_RESOURCE(function_t) m_uiTypeSlotIndex; // TODO: Check if set
        void* m_pRawFunction;
        function_traits m_traits;
    });

    slot(__enum_element_t,
    {
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        void* m_pValue;// I guess
        char* m_sName;
    });

    slot(__enum_t,
    {
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        DUCKVIL_SLOT_ARRAY(__enum_element_t) m_elements;
        std::size_t m_ullTypeID;
        char* m_sName;
    });

    slot(__type_t,
    {
        DUCKVIL_SLOT_ARRAY(__constructor_t) m_constructors;
        DUCKVIL_SLOT_ARRAY(__destructor_t) m_destructors;
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

        std::size_t m_ullTypeID;
        char* m_sTypeName;
        DUCKVIL_RESOURCE(type_t) m_uiSlotIndex;
    });

    slot(__ntype_t,
    {
        DUCKVIL_SLOT_ARRAY(__property_t) m_properties;
        DUCKVIL_SLOT_ARRAY(__namespace_t) m_namespaces;
        DUCKVIL_SLOT_ARRAY(__function_t) m_functions;
        DUCKVIL_SLOT_ARRAY(__enum_t) m_enums;
    // This contains all variant keys, even for properties, functions, etc.
    // It could be useful if we want to get all metas that contains specific key in specific type
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantKeys;
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantValues;
    });

    slot(__object_t,
    {
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantKeys;
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantValues;
    // This contains only metas for specific type
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;

        void* m_pObject;
    });



    struct __data
    {
        DUCKVIL_SLOT_ARRAY(__type_t) m_aTypes;
        DUCKVIL_SLOT_ARRAY(__ntype_t) m_aNTypes;
        DUCKVIL_SLOT_ARRAY(__object_t) m_aObjects;
        // Event::Pool<Event::mode::immediate> m_events;
        void* m_pEvents;

        std::unordered_map<std::size_t, __duckvil_resource_type_t>* m_pTypesByTypeID;
    };

    struct __ftable
    {
        __data* (*m_fnInit)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pFunctions);

        duckvil_frontend_reflection_context* (*m_fnCreateContext)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pFTable, __data* _pData);

    // Type
        DUCKVIL_RESOURCE(type_t) (*m_fnGetTypeHandleByName)(__data* _pData, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(type_t) (*m_fnGetTypeHandleByTypeID)(__data* _pData, const std::size_t& _ullTypeID);
        const __type_t& (*m_fnGetTypeByHandle)(__data* _pData, const DUCKVIL_RESOURCE(type_t)& _typeHandle);

        Memory::Vector<DUCKVIL_RESOURCE(type_t)> (*m_fnGetTypes)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator);

    // Constructor
        Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> (*m_fnGetConstructors)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        DUCKVIL_RESOURCE(constructor_t) (*m_fnGetConstructorHandleByTypeID)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID);
        const __constructor_t& (*m_fnGetConstructor)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);

    // Desctructor
        DUCKVIL_RESOURCE(destructor_t) (*m_fnGetDestructorHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle);

    // Property
        const void* (*m_fnGetPropertyValueByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject);
        const void* (*m_fnGetPropertyValueByName)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, const void* _pObject);
        DUCKVIL_RESOURCE(property_t) (*m_fnGetPropertyHandleByName)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength);

        const void* (*m_fnGetPropertyValue)(__data* _pData, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID, const void* _pObject);
        Memory::Vector<DUCKVIL_RESOURCE(property_t)> (*m_fnGetProperties)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);

        const __property_t& (*m_fnGetPropertyByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle);

    // Function
        Memory::Vector<DUCKVIL_RESOURCE(function_t)> (*m_fnGetFunctions)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        DUCKVIL_RESOURCE(function_t) (*m_fnGetFunctionHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID);
        DUCKVIL_RESOURCE(function_t) (*m_fnGetFunctionHandleByPointer)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pFunction);
        __ifunction* (*m_fnGetFunctionCallback)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID);
        __ifunction* (*m_fnGetFunctionCallbackByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, std::size_t _ullTypeID);
        const __function_t& (*m_fnGetFunctionByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);

    // Argument
        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> (*m_fnGetConstructorArguments)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle);
        const __argument_t& (*m_fnGetConstructorArgument)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _handle);

        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> (*m_fnGetFunctionArguments)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _handle);
        const __argument_t& (*m_fnGetFunctionArgument)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _handle);

    // Inheritance
        Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> (*m_fnGetInheritances)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        const __inheritance_t& (*m_fnGetInheritance)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(inheritance_t) _inheritanceHandle);
        bool (*m_fnInherits)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritanceHandle);
        bool (*m_fnInheritsByTypeID)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID);

    // Variant
        const __variant_t& (*m_fnGetVariantKeyByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle);
        const __variant_t& (*m_fnGetVariantValueByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle);



    //////////////////////////////////////
    /////////////// GLOBAL /////////////////
    //////////////////////////////////////

        DUCKVIL_RESOURCE(ntype_t) (*m_fnGetNTypeHandleByNamespaces)(__data* _pData, const std::vector<const char*>& _aNamespaces);
        const __ntype_t& (*m_fnGetNTypeByHandle)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle);

        DUCKVIL_RESOURCE(enum_t) (*m_fnGetNEnumHandleByName)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(enum_t) (*m_fnGetNEnumHandleByTypeID)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, std::size_t _ullTypeID);
        const __enum_t& (*m_fnGetNEnumByHandle)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle);
        const __enum_t& (*m_fnGetNEnumByTypeID)(__data* _pData, std::size_t _ullTypeID, DUCKVIL_RESOURCE(ntype_t)* _pTypeHandle, DUCKVIL_RESOURCE(enum_t)* _pHandle);

        DUCKVIL_RESOURCE(enum_element_t) (*m_fnGetNEnumElementHandleByName)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(enum_element_t) (*m_fnGetNEnumElementHandleByValue)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _handle, const void* _pValue, std::size_t _ullSize);
        const __enum_element_t& (*m_fnGetNEnumElementByHandle)(__data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _enumHandle, DUCKVIL_RESOURCE(enum_element_t) _handle);



    //////////////////////////////////////
    /////////////// META /////////////////
    //////////////////////////////////////

        DUCKVIL_RESOURCE(meta_t) (*m_fnAddObjectMeta)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, uint8_t _ucKeyAlignment, property_traits _keyTraits, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData);
        const __variant_t& (*m_fnGetObjectMeta)(__data* _pData, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData);
        void (*m_fnSetObjectMeta)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData);
        void (*m_fnRemoveObjectMeta)(__data* _pData, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData);
        void (*m_fnClearObjectMetas)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject);

        DUCKVIL_META_DECLARE_UTIL(Type, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t)))
        DUCKVIL_META_DECLARE_UTIL(Property, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(property_t)))
        DUCKVIL_META_DECLARE_UTIL(Function, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t)))
        DUCKVIL_META_DECLARE_UTIL(FunctionArgument, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(function_t), uint32_t))
        DUCKVIL_META_DECLARE_UTIL(Constructor, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t)))
        DUCKVIL_META_DECLARE_UTIL(ConstructorArgument, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(constructor_t), uint32_t))
        DUCKVIL_META_DECLARE_UTIL(Enum, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t), DUCKVIL_RESOURCE(enum_t)))
    };

}}