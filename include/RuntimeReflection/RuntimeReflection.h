#pragma once

#include "PlugNPlay/Module.h"

#include "Memory/Vector.h"
#include "Memory/FreeList.h"
#include "Memory/SlotArray.h"

#include "RuntimeReflection/Function.h"

// #include "Event/ImmediatePool.h"

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
        __variant_owner_type,
        __variant_owner_property,
        __variant_owner_constructor
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
    });

    slot(__constructor_t,
    {
        DUCKVIL_SLOT_ARRAY(__argument_t) m_arguments;
        DUCKVIL_SLOT_ARRAY(__meta_t) m_metas;
        std::size_t m_ullTypeID;
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
        __ifunction* m_pFunction;
        char* m_sFunctionName;
        std::size_t m_ullReturnTypeID;
        std::size_t m_ullArgumentsTypeID;
        DUCKVIL_RESOURCE(function_t) m_uiTypeSlotIndex; // TODO: Check if set
    });

    slot(__type_t,
    {
        DUCKVIL_SLOT_ARRAY(__constructor_t) m_constructors;
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

        void* (*m_fnGetPropertyByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject);
        void* (*m_fnGetPropertyByName)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, const void* _pObject);
        DUCKVIL_RESOURCE(property_t) (*m_fnGetPropertyHandleByName)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength);

        Memory::Vector<DUCKVIL_RESOURCE(function_t)> (*m_fnGetFunctions)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        DUCKVIL_RESOURCE(function_t) (*m_fnGetFunctionHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID);
        __ifunction* (*m_fnGetFunctionCallback)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID);
        __ifunction* (*m_fnGetFunctionCallbackByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, std::size_t _ullTypeID);
        const __function_t& (*m_fnGetFunctionByHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);

        void* (*m_fnGetProperty)(__data* _pData, const char* _sName, std::size_t _ullLength, std::size_t _ullTypeID, const void* _pObject);
        Memory::Vector<DUCKVIL_RESOURCE(property_t)> (*m_fnGetProperties)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);

        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> (*m_fnGetArguments)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle);
        const __argument_t& (*m_fnGetArgument)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _handle);

        Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> (*m_fnGetInheritances)(__data* _pData, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle);
        const __inheritance_t& (*m_fnGetInheritance)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(inheritance_t) _inheritanceHandle);
        bool (*m_fnInherits)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritanceHandle);
        bool (*m_fnInheritsByTypeID)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, std::size_t _ullTypeID);

        DUCKVIL_RESOURCE(variant_t) (*m_fnGetTypeMetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        void* (*m_fnGetTypeMetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        const __variant& (*m_fnGetTypeMetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);

        DUCKVIL_RESOURCE(variant_t) (*m_fnGetPropertyMetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        void* (*m_fnGetPropertyMetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        const __variant& (*m_fnGetPropertyMetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        Memory::Vector<DUCKVIL_RESOURCE(meta_t)> (*m_fnGetPropertyMetas)(const Memory::FreeList& _heap, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle);

        DUCKVIL_RESOURCE(variant_t) (*m_fnGetConstructorMetaHandle)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        void* (*m_fnGetConstructorMetaValue)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
        const __variant& (*m_fnGetConstructorMetaVariant)(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, const void* _pKey, const std::size_t& _ullSize, const std::size_t& _ullTypeID);
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

    template <typename... Args, std::size_t Length>
    static inline DUCKVIL_RESOURCE(function_t) get_function_handle(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetFunctionHandle(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetFunctions(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
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
    static inline void* get_property(__ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Type* _pObject)
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
        __proxy_member_function<ReturnType, Args...>* _func = (__proxy_member_function<ReturnType, Args...>*)_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));

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

// With context
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

    static inline Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetConstructors(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    static inline Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetFunctions(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
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

        return _context.m_pReflection->m_fnGetArguments(_context.m_pReflectionData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle, _constructorHandle);
    }

    static inline __argument_t get_argument(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        duckvil_frontend_reflection_context& _context = g_duckvilFrontendReflectionContext;

        return _context.m_pReflection->m_fnGetArgument(_context.m_pReflectionData, _typeHandle, _constructorHandle, _argumentHandle);
    }

}}

typedef Duckvil::RuntimeReflection::__ftable* (*duckvil_runtime_reflection_init_callback)();

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init();