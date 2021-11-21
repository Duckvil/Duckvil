#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Vector.h"
#include "Memory/FreeList.h"
#include "Memory/SlotArray.h"

#include "RuntimeReflection/Function.h"

// #include "Event/ImmediatePool.h"

#define DUCKVIL_META_DECLARE_UTIL(name, type) \
    DUCKVIL_RESOURCE(variant_t) (*m_fnGet ## name ## MetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type ## _t) _ ## type ## Handle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID); \
    void* (*m_fnGet ## name ## MetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type ## _t) _ ## type ## Handle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID); \
    const __variant& (*m_fnGet ## name ## MetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type ## _t) _ ## type ## Handle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID); \
    Memory::Vector<DUCKVIL_RESOURCE(meta_t)> (*m_fnGet ## name ## Metas)(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type ## _t) _ ## type ## Handle);

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

#define DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(...) typeid(void(__VA_ARGS__)).hash_code()

#define DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(type, trait) (std::trait<type>::value ? static_cast<property_traits>((uint8_t)_traits | (uint8_t)property_traits::trait) : _traits)

namespace std {

    template <typename Type>
    struct is_bool : std::is_same<bool, typename std::remove_cv<Type>::type> {};

}

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

inline duckvil_frontend_reflection_context g_duckvilFrontendReflectionContext;

namespace Duckvil { namespace RuntimeReflection {

    static const std::size_t& CONST_CHAR_POINTER_ID = typeid(const char*).hash_code();

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

    enum class property_traits : uint16_t
    {
        is_pointer          = 1 << 0,
        is_reference        = 1 << 1,
        is_array            = 1 << 2,
        is_void             = 1 << 3,
        is_integral         = 1 << 4,
        is_floating_point   = 1 << 5,
        is_enum             = 1 << 6,
        is_bool             = 1 << 7,
        is_const            = 1 << 8
    };

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

    slot(__object_t,
    {
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantKeys;
        DUCKVIL_SLOT_ARRAY(__variant_t) m_variantValues;
    // This contains only metas for specific type
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;

        void* m_pObject;
    });

    template <typename Type>
    static inline property_traits recorder_generate_traits()
    {
        property_traits _traits = {};

        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_pointer);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_reference);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_array);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_void);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_integral);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_floating_point);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_enum);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_bool);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, is_const);

        return _traits;
    }

    template <typename Type>
    static inline property_traits recorder_generate_traits(const Type& _type)
    {
        return recorder_generate_traits<Type>();
    }

    template <std::size_t Length>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const char(&_sName)[Length])
    {
        if(!_bRes)
        {
            return;
        }

        const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_typeNamespaces, _uiIndex);

        if(strcmp(_namespace.m_sNamespaceName, _sName) != 0)
        {
            _bRes = false;
        }

        _uiIndex++;
    }

    template <typename Arg>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const Arg& _arg)
    {
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _arg);
    }

    template <typename Arg, typename... Args>
    void compare_namespace(const DUCKVIL_SLOT_ARRAY(__namespace_t)& _typeNamespaces, uint32_t& _uiIndex, bool& _bRes, const Arg& _arg, const Args&... _args)
    {
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _arg);
        compare_namespace(_typeNamespaces, _uiIndex, _bRes, _args...);
    }

    struct __data
    {
        DUCKVIL_SLOT_ARRAY(__type_t) m_aTypes;
        DUCKVIL_SLOT_ARRAY(__object_t) m_aObjects;
        // Event::Pool<Event::mode::immediate> m_events;
        void* m_pEvents;
    };

    struct __ftable
    {
        __data* (*m_fnInit)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pFunctions);

        duckvil_frontend_reflection_context* (*m_fnCreateContext)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pFTable, __data* _pData);

        DUCKVIL_RESOURCE(type_t) (*m_fnGetTypeHandleByName)(__data* _pData, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(type_t) (*m_fnGetTypeHandleByTypeID)(__data* _pData, const std::size_t& _ullTypeID);
        const __type_t& (*m_fnGetType)(__data* _pData, const DUCKVIL_RESOURCE(type_t)& _typeHandle);
        Memory::Vector<DUCKVIL_RESOURCE(type_t)> (*m_fnGetTypes)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator);

        Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> (*m_fnGetConstructors)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        DUCKVIL_RESOURCE(constructor_t) (*m_fnGetConstructorHandleByTypeID)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID);
        const __constructor_t& (*m_fnGetConstructor)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);

        DUCKVIL_RESOURCE(destructor_t) (*m_fnGetDestructorHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle);

        const void* (*m_fnGetPropertyByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject);
        const void* (*m_fnGetPropertyByName)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, const void* _pObject);
        DUCKVIL_RESOURCE(property_t) (*m_fnGetPropertyHandleByName)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength);

        Memory::Vector<DUCKVIL_RESOURCE(function_t)> (*m_fnGetFunctions)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        DUCKVIL_RESOURCE(function_t) (*m_fnGetFunctionHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID);
        __ifunction* (*m_fnGetFunctionCallback)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID);
        __ifunction* (*m_fnGetFunctionCallbackByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, std::size_t _ullTypeID);
        const __function_t& (*m_fnGetFunctionByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);

        const void* (*m_fnGetProperty)(__data* _pData, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID, const void* _pObject);
        Memory::Vector<DUCKVIL_RESOURCE(property_t)> (*m_fnGetProperties)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);

        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> (*m_fnGetConstructorArguments)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle);
        const __argument_t& (*m_fnGetConstructorArgument)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _handle);

        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> (*m_fnGetFunctionArguments)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _handle);
        const __argument_t& (*m_fnGetFunctionArgument)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _handle);

        Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> (*m_fnGetInheritances)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        const __inheritance_t& (*m_fnGetInheritance)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(inheritance_t) _inheritanceHandle);
        bool (*m_fnInherits)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritanceHandle);
        bool (*m_fnInheritsByTypeID)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID);

        DUCKVIL_RESOURCE(variant_t) (*m_fnGetTypeMetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        void* (*m_fnGetTypeMetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        const __variant& (*m_fnGetTypeMetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> (*m_fnGetTypeMetas)(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle);

        DUCKVIL_RESOURCE(variant_t) (*m_fnGetConstructorArgumentMetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        void* (*m_fnGetConstructorArgumentMetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        const __variant& (*m_fnGetConstructorArgumentMetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> (*m_fnGetConstructorArgumentMetas)(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex);

        DUCKVIL_RESOURCE(variant_t) (*m_fnGetFunctionArgumentMetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        void* (*m_fnGetFunctionArgumentMetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        const __variant& (*m_fnGetFunctionArgumentMetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> (*m_fnGetFunctionArgumentMetas)(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex);

        DUCKVIL_RESOURCE(meta_t) (*m_fnAddObjectMeta)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, uint8_t _ucKeyAlignment, property_traits _keyTraits, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData);
        const __variant_t& (*m_fnGetObjectMeta)(__data* _pData, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData);
        void (*m_fnSetObjectMeta)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData, std::size_t _ullValueTypeID, std::size_t _ullValueSize, uint8_t _ucValueAlignment, property_traits _valueTraits, const void* _pValueData);
        void (*m_fnRemoveObjectMeta)(__data* _pData, void* _pObject, std::size_t _ullKeyTypeID, std::size_t _ullKeySize, const void* _pKeyData);

        DUCKVIL_META_DECLARE_UTIL(Property, property)
        DUCKVIL_META_DECLARE_UTIL(Function, function)
        DUCKVIL_META_DECLARE_UTIL(Constructor, constructor)
    };

    static inline void make_current(const duckvil_frontend_reflection_context& _context)
    {
        g_duckvilFrontendReflectionContext = _context;
    }

    static inline const duckvil_frontend_reflection_context& get_current()
    {
        return g_duckvilFrontendReflectionContext;
    }

// Create type object using type name string
// Note: It will compare given arguments with available constructors arguments
    template <typename... Args, std::size_t Length>
    static void* create(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pReflection, __data* _pData, const char (&_sTypeName)[Length], bool _bTracked, const Args&... _vArgs)
    {
        const __data& _data = *_pData;
        static const std::size_t& _constructorTypeID = typeid(void*(Args...)).hash_code();

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sTypeName) == 0)
            {
                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); ++j)
                {
                    const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

                    if(_constructor.m_ullTypeID == _constructorTypeID)
                    {
                        void* (*_constructor_callback)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*, bool, Args...) = (void* (*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*, bool, Args...))_constructor.m_pData;

                        return _constructor_callback(_pMemoryInterface, _pAllocator, _pReflection, _pData, _bTracked, _vArgs...);
                    }
                }
            }
        }

        return nullptr;
    }

// Create type object using type handle
// Note: It will compare given arguments with available constructors arguments
    template <typename... Args>
    static void* create(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, const Args&... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _constructorTypeID = typeid(void*(Args...)).hash_code();

        for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); ++j)
        {
            const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

            if(_constructor.m_ullTypeID == _constructorTypeID)
            {
                void* (*_constructor_callback)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*, bool, Args...) = (void* (*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*, bool, Args...))_constructor.m_pData;

                return _constructor_callback(_pMemoryInterface, _pAllocator, _pReflection, _pData, _bTracked, _vArgs...);
            }
        }

        return nullptr;
    }

    static void destroy(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_destructors.m_data); ++j)
        {
            const __destructor_t& _destructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_destructors, j);
            void (*_destructor_callback)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*, bool, void*) = (void (*)(Memory::ftable*, Memory::free_list_allocator*, __ftable*, __data*, bool, void*))_destructor.m_pData;

            _destructor_callback(_pMemoryInterface, _pAllocator, _pReflection, _pData, _bTracked, _pObject);

            return;
        }
    }

    static inline DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData, const std::size_t& _ullTypeID)
    {
        return _pFTable->m_fnGetTypeHandleByTypeID(_pData, _ullTypeID);
    }

    template <typename Type>
    static inline DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();

        return _pFTable->m_fnGetTypeHandleByTypeID(_pData, _typeID);
    }

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData, const char (&_sName)[Length])
    {
        return _pFTable->m_fnGetTypeHandleByName(_pData, _sName, Length);
    }

    template <std::size_t Length, std::size_t... Lengths>
    static DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char (&_sName)[Length], const char (&..._sNamespaces)[Lengths])
    {
        const __data& _data = *_pData;
        static const std::size_t& _argsSize = sizeof...(Lengths);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);

            if(_argsSize == DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data))
            {
                uint32_t _index = 0;
                bool _res = true;

                compare_namespace(_type.m_namespaces, _index, _res, _sNamespaces...);

                if(_res && strcmp(_type.m_sTypeName, _sName) == 0)
                {
                    return _type.m_uiSlotIndex;
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    static DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char* _sName, const std::vector<const char*>& _aNamespaces)
    {
        const __data& _data = *_pData;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);
            size_t _namespacesSize = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data);

            if(_aNamespaces.size() == _namespacesSize)
            {
                bool _valid = true;

                for(uint32_t j = 0; j < _aNamespaces.size(); ++j)
                {
                    const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j);

                    if(strcmp(_namespace.m_sNamespaceName, _aNamespaces[j]) != 0)
                    {
                        _valid = false;

                        break;
                    }
                }

                if(_valid && strcmp(_sName, _type.m_sTypeName) == 0)
                {
                    return { i };
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    static inline const __type_t& get_type(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetType(_pData, _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap)
    {
        return _pReflection->m_fnGetTypes(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator());
    }

    template <typename... Args>
    static inline DUCKVIL_RESOURCE(constructor_t) get_constructor_handle(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetConstructorHandleByTypeID(_pData, _typeHandle, typeid(void*(Args...)).hash_code());
    }

    static inline const __constructor_t& get_constructor(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        return _pReflection->m_fnGetConstructor(_pData, _typeHandle, _constructorHandle);
    }

    template <typename... Args, std::size_t Length>
    static inline DUCKVIL_RESOURCE(function_t) get_function_handle(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetFunctionHandle(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetFunctions(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_functions(_pReflection, _pData, _heapContext.m_heap, _typeHandle);
    }

// TODO: Check it
    template <typename... Args, std::size_t Length>
    static inline __function<void(*)(Args...)>* get_static_function_callback(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return (__function<void(*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));
    }

    template <typename Type, typename... Args, std::size_t Length>
    static inline __function<void(Type::*)(Args...)>* get_function_callback(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return (__function<void(Type::*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));
    }

    template <typename Type, typename... Args>
    static inline __function<void(Type::*)(Args...)>* get_function_callback(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);

        return (__function<void(Type::*)(Args...)>*)_function.m_pFunction;
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline __function<ReturnType(Type::*)(Args...)>* get_function_callback(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return (__function<ReturnType(Type::*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));
    }

    static inline const __function_t& get_function(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        return _pReflection->m_fnGetFunctionByHandle(_pData, _typeHandle, _functionHandle);
    }

    template <typename Type, std::size_t Length>
    static inline const void* get_property(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Type* _pObject)
    {
        return _pReflection->m_fnGetProperty(_pData, _sName, Length, typeid(Type).hash_code(), _pObject);
    }

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(property_t) get_property(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetPropertyHandleByName(_pData, _typeHandle, _sName, Length);
    }

// Invoke member function
    template <typename Type, typename... Args, std::size_t Length>
    static inline void invoke(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], Type* _pObject, const Args&... _vArgs)
    {
        auto& _typeHandle = _pReflection->m_fnGetTypeHandleByTypeID(_pData, typeid(Type).hash_code());
        __function<void(Type::*)(Args...)>* _func = (__function<void(Type::*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        (_pObject->*(_func->m_fnFunction))(_vArgs...);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline ReturnType invoke(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], Type* _pObject, const Args&... _vArgs)
    {
        auto& _typeHandle = _pReflection->m_fnGetTypeHandleByTypeID(_pData, typeid(Type).hash_code());
        __function<ReturnType(Type::*)(Args...)>* _func = (__function<ReturnType(Type::*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        return (_pObject->*(_func->m_fnFunction))(_vArgs...);
    }

    template <typename... Args>
    static inline void invoke_member(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        __proxy_member_function<void, Args...>* _func = (__proxy_member_function<void, Args...>*)_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        _func->Invoke(_pObject, _vArgs...);
    }

    template <typename ReturnType, typename... Args>
    static inline ReturnType invoke_member_result(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        __proxy_member_function<ReturnType, Args...>* _func = (__proxy_member_function<ReturnType, Args...>*)_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        return _func->Invoke(_pObject, _vArgs...);
    }

    template <typename... Args>
    static inline void invoke_member(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const void* _pObject, const Args&... _vArgs)
    {
        __proxy_member_const_function<void, Args...>* _func = (__proxy_member_const_function<void, Args...>*)_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        _func->Invoke(_pObject, _vArgs...);
    }

    template <typename ReturnType, typename... Args>
    static inline ReturnType invoke_member_result(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const void* _pObject, const Args&... _vArgs)
    {
        __proxy_member_const_function<ReturnType, Args...>* _func = (__proxy_member_const_function<ReturnType, Args...>*)_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        return _func->Invoke(_pObject, _vArgs...);
    }

// Invoke static function
    template <typename Type, typename... Args, std::size_t Length>
    static inline void invoke(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Args&... _vArgs)
    {
        auto& _typeHandle = _pReflection->m_fnGetTypeHandleByTypeID(_pData, typeid(Type).hash_code());
        __function<void(*)(Args...)>* _func = (__function<void(*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        (*(_func->m_fnFunction))(_vArgs...);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline ReturnType invoke(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Args&... _vArgs)
    {
        auto& _typeHandle = _pReflection->m_fnGetTypeHandleByTypeID(_pData, typeid(Type).hash_code());
        __function<ReturnType(*)(Args...)>* _func = (__function<ReturnType(*)(Args...)>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        return (*(_func->m_fnFunction))(_vArgs...);
    }

    template <typename... Args>
    static inline void invoke_static(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        __proxy_static_function<void, Args...>* _func = (__proxy_static_function<void, Args...>*)_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        _func->Invoke(_vArgs...);
    }

    template <typename ReturnType, typename... Args>
    static inline ReturnType invoke_static_result(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        __proxy_static_function<ReturnType, Args...>* _func = (__proxy_static_function<ReturnType, Args...>*)_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

        return _func->Invoke(_vArgs...);
    }

    template <typename Type>
    static inline bool inherits(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnInheritsByTypeID(_pData, _typeHandle, typeid(Type).hash_code());
    }

    template <typename... Args>
    static inline void* create(const Memory::FreeList& _memory, __ftable* _pReflection, __data* _pData, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX], bool _bTracked, const Args&... _vArgs)
    {
        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pReflection, _pData, _sTypeName, _bTracked, _vArgs...);
    }

    template <typename... Args>
    static inline void* create(const Memory::FreeList& _memory, __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, const Args&... _vArgs)
    {
        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pReflection, _pData, _typeHandle, _bTracked, _vArgs...);
    }

    static inline void destroy(const Memory::FreeList& _memory, __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject)
    {
        destroy(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pReflection, _pData, _typeHandle, _bTracked, _pObject);
    }

// With context
    template <typename... Args>
    static inline void* create(const Memory::FreeList& _memory, const char _sTypeName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX], bool _bTracked, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _context.m_pReflection, _context.m_pReflectionData, _sTypeName, _bTracked, _vArgs...);
    }

    template <typename... Args>
    static inline void* create(const Memory::FreeList& _memory, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _context.m_pReflection, _context.m_pReflectionData, _typeHandle, _bTracked, _vArgs...);
    }

    static inline DUCKVIL_RESOURCE(type_t) get_type(const std::size_t& _ullTypeID)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _ullTypeID);
    }

    template <typename Type>
    static inline DUCKVIL_RESOURCE(type_t) get_type()
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type<Type>(_context.m_pReflection, _context.m_pReflectionData);
    }

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(type_t) get_type(const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_type(_context.m_pReflection, _context.m_pReflectionData, _sName);
    }

    template <std::size_t Length, std::size_t... Lengths>
    static DUCKVIL_RESOURCE(type_t) get_type(const char (&_sName)[Length], const char (&..._sNamespaces)[Lengths])
    {
         duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;
        const __data& _data = *_context.m_pReflectionData;
        static const std::size_t& _argsSize = sizeof...(Lengths);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);

            if(_argsSize == DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data))
            {
                uint32_t _index = 0;
                bool _res = true;

                compare_namespace(_type.m_namespaces, _index, _res, _sNamespaces...);

                if(_res && strcmp(_type.m_sTypeName, _sName) == 0)
                {
                    return _type.m_uiSlotIndex;
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    static DUCKVIL_RESOURCE(type_t) get_type(const char* _sName, const std::vector<const char*>& _aNamespaces)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;
        const __data& _data = *_context.m_pReflectionData;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);
            size_t _namespacesSize = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data);

            if(_aNamespaces.size() == _namespacesSize)
            {
                bool _valid = true;

                for(uint32_t j = 0; j < _aNamespaces.size(); ++j)
                {
                    const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j);

                    if(strcmp(_namespace.m_sNamespaceName, _aNamespaces[j]) != 0)
                    {
                        _valid = false;

                        break;
                    }
                }

                if(_valid && strcmp(_sName, _type.m_sTypeName) == 0)
                {
                    return { i };
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(const Memory::FreeList& _heap)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_types(_context.m_pReflection, _context.m_pReflectionData, _heap);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types()
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_types(_heapContext.m_heap);
    }

    static inline const __type_t& get_type(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetType(_context.m_pReflectionData, _typeHandle);
    }

    template <typename... Args>
    static inline DUCKVIL_RESOURCE(constructor_t) get_constructor_handle(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_constructor_handle<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle);
    }

    static inline const __constructor_t& get_constructor(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_constructor(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _constructorHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructors(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_constructors(_heapContext.m_heap, _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetFunctions(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_functions(_heapContext.m_heap, _typeHandle);
    }

    template <typename... Args, std::size_t Length>
    static inline DUCKVIL_RESOURCE(function_t) get_function_handle(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_handle<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

// TODO: Check it
    template <typename... Args, std::size_t Length>
    static inline __function<void(*)(Args...)>* get_static_function_callback(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_static_function_handle<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    template <typename Type, typename... Args, std::size_t Length>
    static inline __function<void(Type::*)(Args...)>* get_function_callback(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_callback<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    template <typename Type, typename... Args>
    static inline __function<void(Type::*)(Args...)>* get_function_callback(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_callback<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline __function<ReturnType(Type::*)(Args...)>* get_function_callback(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function_callback<ReturnType, Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

    static inline const __function_t& get_function(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_function(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle);
    }

    template <typename Type, std::size_t Length>
    static inline void* get_property(const char (&_sName)[Length], const Type* _pObject)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_property<Type>(_context.m_pReflection, _context.m_pReflectionData, _sName, _pObject);
    }

    template <std::size_t Length>
    static inline DUCKVIL_RESOURCE(property_t) get_property(DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return get_property(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _sName);
    }

// Invoke member function
    template <typename Type, typename... Args, std::size_t Length>
    static inline void invoke(const char (&_sName)[Length], Type* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        invoke<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _sName, _pObject, _vArgs...);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline ReturnType invoke(const char (&_sName)[Length], Type* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return invoke<ReturnType, Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _sName, _pObject, _vArgs...);
    }

    template <typename... Args>
    static inline void invoke_member(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        invoke_member<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _pObject, _vArgs...);
    }

    template <typename ReturnType, typename... Args>
    static inline ReturnType invoke_member_result(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, void* _pObject, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return invoke_member_result<ReturnType, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _pObject, _vArgs...);
    }

// Invoke static function
    template <typename Type, typename... Args, std::size_t Length>
    static inline void invoke(const char (&_sName)[Length], const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        invoke<Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _sName, _vArgs...);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    static inline ReturnType invoke(const char (&_sName)[Length], const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return invoke<ReturnType, Type, Args...>(_context.m_pReflection, _context.m_pReflectionData, _sName, _vArgs...);
    }

    template <typename... Args>
    static inline void invoke_static(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        invoke_static<Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _vArgs...);
    }

    template <typename ReturnType, typename... Args>
    static inline ReturnType invoke_static_result(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return invoke_static_result<ReturnType, Args...>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle, _functionHandle, _vArgs...);
    }

    template <typename Type>
    static inline bool inherits(DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return inherits<Type>(_context.m_pReflection, _context.m_pReflectionData, _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArguments(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle, _constructorHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_arguments(_heapContext.m_heap, _typeHandle, _constructorHandle);
    }

    static inline __argument_t get_argument(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgument(_context.m_pReflectionData, _typeHandle, _constructorHandle, _argumentHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetFunctionArguments(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle, _functionHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_arguments(_heapContext.m_heap, _typeHandle, _functionHandle);
    }

    static inline __argument_t get_argument(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetFunctionArgument(_context.m_pReflectionData, _typeHandle, _functionHandle, _argumentHandle);
    }

    template <size_t Length>
    static inline DUCKVIL_RESOURCE(variant_t) get_argument_meta_handle(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const char (&_key)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetaHandle(_context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex, _key, Length, typeid(const char*).hash_code());
    }

    template <typename Key>
    static inline DUCKVIL_RESOURCE(variant_t) get_argument_meta_handle(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const Key& _key)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetaHandle(_context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex, &_key, sizeof(Key), typeid(Key).hash_code());
    }

    template <size_t Length>
    static inline __variant get_argument_meta_variant(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const char (&_key)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetaVariant(_context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex, _key, Length, typeid(const char*).hash_code());
    }

    template <typename Key>
    static inline __variant get_argument_meta_variant(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const Key& _key)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetaVariant(_context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex, &_key, sizeof(Key), typeid(Key).hash_code());
    }

    template <size_t Length>
    static inline void* get_argument_meta_value(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const char (&_key)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetaValue(_context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex, _key, Length, typeid(const char*).hash_code());
    }

    template <typename Key>
    static inline void* get_argument_meta_value(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex, const Key& _key)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetaValue(_context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex, &_key, sizeof(Key), typeid(Key).hash_code());
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_argument_metas(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructorArgumentMetas(_heap, _context.m_pReflectionData, _typeHandle, _constructorHandle, _uiArgumentIndex);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_argument_metas(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_argument_metas(_heapContext.m_heap, _typeHandle, _constructorHandle, _uiArgumentIndex);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_argument_metas(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetFunctionArgumentMetas(_heap, _context.m_pReflectionData, _typeHandle, _functionHandle, _uiArgumentIndex);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(meta_t)> get_argument_metas(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_argument_metas(_heapContext.m_heap, _typeHandle, _functionHandle, _uiArgumentIndex);
    }

    static inline void destroy(const Memory::FreeList& _memory, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        destroy(_memory.GetMemoryInterface(), _memory.GetAllocator(), _context.m_pReflection, _context.m_pReflectionData, _typeHandle, _bTracked, _pObject);
    }

    template <typename KeyType, typename ValueType>
    static inline DUCKVIL_RESOURCE(meta_t) add_object_meta(__ftable* _pReflection, __data* _pData, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, void* _pObject, const KeyType& _key, const ValueType& _value)
    {
        return _pReflection->m_fnAddObjectMeta(
            _pData,
            _pMemoryInterface,
            _pAllocator,
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            alignof(KeyType),
            recorder_generate_traits(_key),
            &_key,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_traits(_value),
            &_value
            );
    }

    template <typename KeyType, typename ValueType>
    static inline DUCKVIL_RESOURCE(meta_t) add_object_meta(void* _pObject, const KeyType& _key, const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnAddObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            alignof(KeyType),
            recorder_generate_traits(_key),
            &_key,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_traits(_value),
            &_value
            );
    }

    template <typename KeyType>
    static inline __variant_t get_object_meta(void* _pObject, const KeyType& _key)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetObjectMeta(
            _context.m_pReflectionData,
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            &_key
        );
    }

    template <typename KeyType, typename ValueType>
    static inline void set_object_meta(void* _pObject, const KeyType& _key, const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnSetObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            &_key,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_traits(_value),
            &_value
        );
    }

    template <typename KeyType>
    static inline void remove_object_meta(void* _pObject, const KeyType& _key)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnRemoveObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(KeyType).hash_code(),
            sizeof(KeyType),
            &_key
        );
    }

    template <typename ValueType, size_t Length>
    static inline DUCKVIL_RESOURCE(meta_t) add_object_meta(void* _pObject, const char (&_sKey)[Length], const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnAddObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(const char*).hash_code(),
            Length,
            8,
            recorder_generate_traits(_sKey),
            _sKey,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_traits(_value),
            &_value
        );
    }

    template <size_t Length>
    static inline __variant_t get_object_meta(void* _pObject, const char (&_sKey)[Length])
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetObjectMeta(
            _context.m_pReflectionData,
            _pObject,
            typeid(const char*).hash_code(),
            Length,
            _sKey
        );
    }

    template <typename ValueType, size_t Length>
    static inline void set_object_meta(void* _pObject, const char (&_sKey)[Length], const ValueType& _value)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        _context.m_pReflection->m_fnSetObjectMeta(
            _context.m_pReflectionData,
            _heapContext.m_heap.GetMemoryInterface(),
            _heapContext.m_heap.GetAllocator(),
            _pObject,
            typeid(const char*).hash_code(),
            Length,
            &_sKey,
            typeid(ValueType).hash_code(),
            sizeof(ValueType),
            alignof(ValueType),
            recorder_generate_traits(_value),
            &_value
        );
    }

}}

typedef Duckvil::RuntimeReflection::__ftable* (*duckvil_runtime_reflection_init_callback)();

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init();