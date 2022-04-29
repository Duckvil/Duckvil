#pragma once

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/ObjectCreatedEvent.h"
#include "RuntimeReflection/TrackedObjectCreatedEvent.h"
#include "RuntimeReflection/PrepareObjectEvent.h"
#include "RuntimeReflection/Meta.h"

#include "Memory/Queue.h"

#include "Event/ImmediatePool.h"

// Index will be incremented each source file to avoid function name collision
// Each plugin/__module will be created file which contains total count of recorders

#undef max

struct duckvil_recorderd_types
{
    Duckvil::RuntimeReflection::__duckvil_resource_type_t* m_aTypes;
    size_t m_ullCount;
    const char* m_sFile;
    Duckvil::PlugNPlay::__module_information* m_pModule;
    uint32_t m_uiRecorderID;
};

struct duckvil_runtime_reflection_recorder_stuff
{
    Duckvil::Memory::ftable* _pMemoryInterface;
    Duckvil::Memory::free_list_allocator* _pAllocator;
    Duckvil::RuntimeReflection::__recorder_ftable* _pFunctions;
    Duckvil::RuntimeReflection::__data* _pData;
};

static inline duckvil_recorderd_types duckvil_recorded_types_create(Duckvil::Memory::ftable *_pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, const std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t>& _aRecordedTypes, const char* _sFilename, uint32_t _uiRecorderID)
{
    duckvil_recorderd_types _res = {};

    Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = (Duckvil::RuntimeReflection::__duckvil_resource_type_t*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _aRecordedTypes.size() * sizeof(Duckvil::RuntimeReflection::__duckvil_resource_type_t), 8);

    for(size_t i = 0; i < _aRecordedTypes.size(); ++i)
    {
        _types[i] = _aRecordedTypes[i];
    }

    _res.m_aTypes = _types;
    _res.m_ullCount = _aRecordedTypes.size();
    _res.m_sFile = _sFilename;
    _res.m_uiRecorderID = _uiRecorderID;

    return _res;
}

#define DUCKVIL_META_CAT(...) __VA_ARGS__

#define DUCKVIL_RUNTIME_REFLECTION_RECORD(index) \
    DUCKVIL_EXPORT duckvil_recorderd_types duckvil_runtime_reflection_record_##index(const duckvil_runtime_reflection_recorder_stuff& _data)

#define DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(count) \
    DUCKVIL_EXPORT uint32_t duckvil_get_runtime_reflection_recorder_count() { return count; }

#define DUCKVIL_REFLECTION_META_UTIL(name, types, types2) \
    template <typename KeyType, typename ValueType> \
    static DUCKVIL_RESOURCE(meta_t) record_meta(const duckvil_runtime_reflection_recorder_stuff& _data, types, const KeyType& _key, const ValueType& _value) \
    { \
        __recorder_meta_info _meta = recorder_generate_meta_info(_key, _value); \
 \
        return _data._pFunctions->m_fnRecord ## name ## Meta(_data, types2, _meta); \
    } \
 \
    template <typename ValueType, std::size_t Length> \
    static DUCKVIL_RESOURCE(meta_t) record_meta(const duckvil_runtime_reflection_recorder_stuff& _data, types, const char (&_key)[Length], const ValueType& _value) \
    { \
        __recorder_meta_info _meta = recorder_generate_meta_info(_key, _value, sizeof(ValueType)); \
 \
        return _data._pFunctions->m_fnRecord ## name ## Meta(_data, types2, _meta); \
    } \
 \
    template <typename KeyType, std::size_t Length> \
    static DUCKVIL_RESOURCE(meta_t) record_meta(const duckvil_runtime_reflection_recorder_stuff& _data, types, const KeyType& _key, const char (&_value)[Length]) \
    { \
        __recorder_meta_info _meta = recorder_generate_meta_info(_key, sizeof(KeyType), _value); \
 \
        return _data._pFunctions->m_fnRecord ## name ## Meta(_data, types2, _meta); \
    } \
 \
    template <std::size_t Length, std::size_t Length2> \
    static DUCKVIL_RESOURCE(meta_t) record_meta(const duckvil_runtime_reflection_recorder_stuff& _data, types, const char (&_key)[Length], const char (&_value)[Length2]) \
    { \
        __recorder_meta_info _meta = recorder_generate_meta_info(_key, _value); \
 \
        return _data._pFunctions->m_fnRecord ## name ## Meta(_data, types2, _meta); \
    }

namespace Duckvil { namespace RuntimeReflection {

    using RecordFunction = duckvil_recorderd_types (*)(const duckvil_runtime_reflection_recorder_stuff&);
    using GetRecordersCountFunction = uint32_t (*)();

    template <typename Type, typename... Args>
    void* create_object(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pReflection, __data* _pData, bool _bTracked, Args... _vArgs)
    {
        void* _address = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Type), alignof(Type));

        memset(_address, 0, sizeof(Type));

        Type* _object = new(_address) Type(_vArgs...);

        // RuntimeReflection::add_object_meta(_pReflection, _pData, _pMemoryInterface, _pAllocator, _object, "Tracked", _bTracked);

        Event::Pool<Event::mode::immediate>* _events = (Event::Pool<Event::mode::immediate>*)_pData->m_pEvents;

        _events->Broadcast(PrepareObjectEvent{ _object });

#ifdef DUCKVIL_HOT_RELOADING
        if(_bTracked)
        {
            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type<Type>(_pReflection, _pData);
            RuntimeReflection::__duckvil_resource_type_t _trackKeeperHandle = RuntimeReflection::get_type(_pData, "TrackKeeper", { "Duckvil", "HotReloader" });
            HotReloader::ITrackKeeper* _trackKeeper = (HotReloader::ITrackKeeper*)RuntimeReflection::create(_pMemoryInterface, _pAllocator, _pReflection, _pData, _trackKeeperHandle, false, static_cast<void*>(_object), _typeHandle);

            if(_events == nullptr)
            {
                return _trackKeeper;
            }

            _events->Broadcast(TrackedObjectCreatedEvent{ _trackKeeper });

            return _trackKeeper;
        }
        else
        {
            if(_events == nullptr)
            {
                return _object;
            }

            _events->Broadcast(ObjectCreatedEvent{ _object, typeid(Type).hash_code() });

            return _object;
        }
#else
        if(_events == nullptr)
        {
            return _object;
        }

        _events->Broadcast(ObjectCreatedEvent{ _object, typeid(Type).hash_code() });

        return _object;
#endif
    }

    template <typename Type>
    void destroy_object(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __ftable* _pReflection, __data* _pData, bool _bTracked, void* _pObject)
    {
        ((Type*)_pObject)->~Type();

        const auto& _isHot = RuntimeReflection::get_object_meta(_pObject, "IsHot").m_variant;

        if(_isHot.m_ullTypeID == typeid(bool).hash_code() && *static_cast<bool*>(_isHot.m_pData))
        {
            RuntimeReflection::clear_object_metas(static_cast<HotReloader::ITrackKeeper*>(_pObject)->GetObject());
        }
        else
        {
            RuntimeReflection::clear_object_metas(_pObject);
        }

        _pMemoryInterface->m_fnFreeListFree_(_pAllocator, _pObject);
    }

    struct __recorder_meta_info
    {
        std::size_t m_ullKeyTypeID;
        std::size_t m_ullKeyTypeSize;

        std::size_t m_ullValueTypeID;
        std::size_t m_ullValueTypeSize;

        const void* m_pKeyData;

        const void* m_pValueData;

        property_traits m_keyTraits;

        property_traits m_valueTraits;

        uint8_t m_ucKeyTypeAlignment;

        uint8_t m_ucValueTypeAlignment;
    };

    template <typename KeyType, typename ValueType>
    __recorder_meta_info recorder_generate_meta_info(const KeyType& _key, size_t _ullKeySize, const ValueType& _value, size_t _ullValueSize)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(KeyType).hash_code();
        _meta.m_ullKeyTypeSize = _ullKeySize;
        _meta.m_ucKeyTypeAlignment = alignof(KeyType);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_property_traits<KeyType>();

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = _ullValueSize;
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_property_traits<ValueType>();

        return _meta;
    }

    template <typename ValueType, std::size_t Length>
    __recorder_meta_info recorder_generate_meta_info(const char (&_key)[Length], const ValueType& _value, size_t _ullValueSize)
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(const char*).hash_code();
        _meta.m_ullKeyTypeSize = Length;
        _meta.m_ucKeyTypeAlignment = 8;
        _meta.m_pKeyData = _key;
        _meta.m_keyTraits = recorder_generate_property_traits<const char*>();

        _meta.m_ullValueTypeID = typeid(ValueType).hash_code();
        _meta.m_ullValueTypeSize = _ullValueSize;
        _meta.m_ucValueTypeAlignment = alignof(ValueType);
        _meta.m_pValueData = &_value;
        _meta.m_valueTraits = recorder_generate_property_traits<ValueType>();

        return _meta;
    }

    template <typename KeyType, std::size_t Length>
    __recorder_meta_info recorder_generate_meta_info(const KeyType& _key, size_t _ullKeySize, const char (&_value)[Length])
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(KeyType).hash_code();
        _meta.m_ullKeyTypeSize = _ullKeySize;
        _meta.m_ucKeyTypeAlignment = alignof(KeyType);
        _meta.m_pKeyData = &_key;
        _meta.m_keyTraits = recorder_generate_property_traits<KeyType>();

        _meta.m_ullValueTypeID = typeid(const char*).hash_code();
        _meta.m_ullValueTypeSize = Length;
        _meta.m_ucValueTypeAlignment = 8;
        _meta.m_pValueData = _value;
        _meta.m_valueTraits = recorder_generate_property_traits<const char*>();

        return _meta;
    }

    template <std::size_t Length, std::size_t Length2>
    __recorder_meta_info recorder_generate_meta_info(const char (&_key)[Length], const char (&_value)[Length2])
    {
        __recorder_meta_info _meta = {};

        _meta.m_ullKeyTypeID = typeid(const char*).hash_code();
        _meta.m_ullKeyTypeSize = Length;
        _meta.m_ucKeyTypeAlignment = 8;
        _meta.m_pKeyData = _key;
        _meta.m_keyTraits = recorder_generate_property_traits<const char*>();

        _meta.m_ullValueTypeID = typeid(const char*).hash_code();
        _meta.m_ullValueTypeSize = Length2;
        _meta.m_ucValueTypeAlignment = 8;
        _meta.m_pValueData = _value;
        _meta.m_valueTraits = recorder_generate_property_traits<const char*>();

        return _meta;
    }

    template <typename KeyType, typename ValueType>
    __recorder_meta_info recorder_generate_meta_info(const KeyType& _key, const ValueType& _value)
    {
        return recorder_generate_meta_info(_key, sizeof(KeyType), _value, sizeof(ValueType));
    }

    template <typename Type>
    void get_argument_info(Memory::Queue<__argument_t>& _arguments)
    {
        __argument_t _arg = {};

        _arg.m_ullTypeID = typeid(Type).hash_code();
        _arg.m_traits = recorder_generate_property_traits<Type>();

        _arguments.Allocate(_arg);
    }

    struct __recorder_ftable
    {
        DUCKVIL_RESOURCE(type_t) (*m_fnRecordType)(const duckvil_runtime_reflection_recorder_stuff& _data, std::size_t _ullTypeID, const char* _sTypeName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(constructor_t) (*m_fnRecordConstructor)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, uint8_t* _pConctructor, Memory::Queue<__argument_t>& _arguments);
        DUCKVIL_RESOURCE(destructor_t) (*m_fnRecordDestructor)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, uint8_t* _pConctructor);
        DUCKVIL_RESOURCE(property_t) (*m_fnRecordProperty)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char* _sName, std::size_t _ullLength, uintptr_t _ullAddress, const property_traits& _traits);
        DUCKVIL_RESOURCE(namespace_t) (*m_fnRecordNamespace)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(inheritance_t) (*m_fnRecordInheritance)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullInheritanceTypeID, __protection _protection);
        DUCKVIL_RESOURCE(function_t) (*m_fnRecordFunction)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, __ifunction* _pFunction, const char* _sName, std::size_t _ullLength, std::size_t _ullReturnTypeID, std::size_t _ullArgumentsTypeID, Memory::Queue<__argument_t>& _arguments, const function_traits& _traits);
        DUCKVIL_RESOURCE(enum_t) (*m_fnRecordEnum)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(enum_element_t) (*m_fnRecordEnumElement)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, DUCKVIL_RESOURCE(enum_t) _enum, void* _pValue, std::size_t _ullTypeSize, const char* _sName, std::size_t _ullLength);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordTypeMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordPropertyMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _owner, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordConstructorMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _owner, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordConstructorArgumentMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _owner, uint32_t _uiArgumentIndex, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordFunctionMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _owner, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordFunctionArgumentMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _owner, uint32_t _uiArgumentIndex, const __recorder_meta_info& _meta);
        DUCKVIL_RESOURCE(meta_t) (*m_fnRecordEnumMeta)(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _owner, DUCKVIL_RESOURCE(enum_t) _enum, const __recorder_meta_info& _meta);
    };

    template <typename Type, std::size_t Length>
    static DUCKVIL_RESOURCE(type_t) record_type(const duckvil_runtime_reflection_recorder_stuff& _data, const char (&_sName)[Length])
    {
        DUCKVIL_RESOURCE(type_t) _typeHandle = _data._pFunctions->m_fnRecordType(_data, typeid(Type).hash_code(), _sName, Length);

        // ReflectedType_<Type>* _froentendType = new(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(ReflectedType_<Type>), alignof(ReflectedType_<Type>))) ReflectedType_<Type>(_typeHandle);

        // _froentendType->m_aConstructors = Memory::Vector<IReflectedConstructor*>(_pMemoryInterface, _pAllocator, 1);

        // if(_pData->m_aFrontend.Full())
        // {
        //     _pData->m_aFrontend.Resize(_pData->m_aFrontend.Size() * 2);
        // }

        // _pData->m_aFrontend.Allocate(_froentendType);

        return _typeHandle;
    }

    template <typename Type, typename... Args>
    static DUCKVIL_RESOURCE(constructor_t) record_constructor(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        constexpr const std::size_t _size = sizeof...(Args);
        Memory::Queue<__argument_t> _arguments(_data._pMemoryInterface, _data._pAllocator, _size ? _size : 1);

        int _[] = { 0, (get_argument_info<Args>(_arguments), 0)... };
        (void)_;

        DUCKVIL_RESOURCE(constructor_t) _constructorHandle = _data._pFunctions->m_fnRecordConstructor(_data, _typeHandle, typeid(void*(Args...)).hash_code(), (uint8_t*)&create_object<Type, Args...>, _arguments);

        // for(auto it : _pData->m_aFrontend)
        // {
        //     if(it->GetBackendTypeHandle().m_ID == _typeHandle.m_ID)
        //     {
        //         ReflectedConstructor<Args...>* _froentendType = new(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(ReflectedConstructor<Args...>), alignof(ReflectedConstructor<Args...>))) ReflectedConstructor<Args...>(_constructorHandle);

        //         break;
        //     }
        // }

        return _constructorHandle;
    }

    template <typename Type>
    static DUCKVIL_RESOURCE(destructor_t) record_destructor(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        DUCKVIL_RESOURCE(destructor_t) _destructorHandle = _data._pFunctions->m_fnRecordDestructor(_data, _typeHandle, (uint8_t*)&destroy_object<Type>);

        return _destructorHandle;
    }

    // template <typename A, typename B, std::size_t Length>
    // static DUCKVIL_RESOURCE(property_t) record_property(const duckvil_runtime_reflection_recorder_stuff& _data, uintptr_t _ullOffset, const char (&_sName)[Length])
    // {
    //     static std::size_t _typeA_ID = typeid(A).hash_code();
    //     static std::size_t _typeB_ID = typeid(B).hash_code();

    //     for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
    //     {
    //         __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

    //         if(_type.m_ullTypeID == _typeA_ID)
    //         {
    //             return _pFunctions->m_fnRecordProperty(_pMemoryInterface, _pAllocator, _pData, _type.m_uiSlotIndex, _typeB_ID, _sName, Length, _ullOffset, recorder_generate_property_traits<B>());
    //         }
    //     }

    //     return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    // }

    DUCKVIL_REFLECTION_META_UTIL(Type, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle), DUCKVIL_META_CAT(_handle))
    DUCKVIL_REFLECTION_META_UTIL(Property, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle, DUCKVIL_RESOURCE(property_t) _propertyHandle), DUCKVIL_META_CAT(_handle, _propertyHandle))
    DUCKVIL_REFLECTION_META_UTIL(Constructor, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle), DUCKVIL_META_CAT(_handle, _constructorHandle))
    DUCKVIL_REFLECTION_META_UTIL(ConstructorArgument, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, uint32_t _uiArgumentIndex), DUCKVIL_META_CAT(_handle, _constructorHandle, _uiArgumentIndex))
    DUCKVIL_REFLECTION_META_UTIL(Function, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle, DUCKVIL_RESOURCE(function_t) _functionHandle), DUCKVIL_META_CAT(_handle, _functionHandle))
    DUCKVIL_REFLECTION_META_UTIL(FunctionArgument, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle, DUCKVIL_RESOURCE(function_t) _functionHandle, uint32_t _uiArgumentIndex), DUCKVIL_META_CAT(_handle, _functionHandle, _uiArgumentIndex))
    DUCKVIL_REFLECTION_META_UTIL(Enum, DUCKVIL_META_CAT(DUCKVIL_RESOURCE(type_t) _handle, DUCKVIL_RESOURCE(enum_t) _enumHandle), DUCKVIL_META_CAT(_handle, _enumHandle))

    template <typename B, std::size_t Length>
    static DUCKVIL_RESOURCE(property_t) record_property(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, uintptr_t _ullOffset, const char (&_sName)[Length])
    {
        static std::size_t _typeB_ID = typeid(B).hash_code();

        return _data._pFunctions->m_fnRecordProperty(_data, _typeHandle, _typeB_ID, _sName, Length, _ullOffset, recorder_generate_property_traits<B>());
    }

    template <std::size_t Length>
    static DUCKVIL_RESOURCE(namespace_t) record_namespace(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _data._pFunctions->m_fnRecordNamespace(_data, _typeHandle, _sName, Length);
    }

    template <typename Type>
    static DUCKVIL_RESOURCE(inheritance_t) record_inheritance(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, __protection _protection)
    {
        return _data._pFunctions->m_fnRecordInheritance(_data, _typeHandle, typeid(Type).hash_code(), _protection);
    }

    template <typename Type, typename ReturnType, typename... Args, std::size_t Length>
    static DUCKVIL_RESOURCE(function_t) record_function(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, ReturnType (Type::*func)(Args...), const char (&_sName)[Length])
    {
        using FunctionType = ReturnType (Type::*)(Args...);

        void* _pointer =
            _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, sizeof(__function<FunctionType>), alignof(__function<FunctionType>));

        __function<FunctionType>* _function = new(_pointer) __function<FunctionType>(func);
        constexpr const std::size_t _size = sizeof...(Args);
        Memory::Queue<__argument_t> _arguments(_data._pMemoryInterface, _data._pAllocator, _size ? _size : 1);

        int _[] = { 0, (get_argument_info<Args>(_arguments), 0)... };
        (void)_;

        return _data._pFunctions->m_fnRecordFunction(_data, _typeHandle, _function, _sName, Length, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code(), _arguments, recorder_generate_function_traits<FunctionType>());
    }

    template <typename Type, typename ReturnType, typename... Args, std::size_t Length>
    static DUCKVIL_RESOURCE(function_t) record_function(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, ReturnType (Type::*func)(Args...) const, const char (&_sName)[Length])
    {
        using FunctionType = ReturnType (Type::*)(Args...) const;

        void* _pointer =
            _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, sizeof(__function<FunctionType>), alignof(__function<FunctionType>));

        __function<FunctionType>* _function = new(_pointer) __function<FunctionType>(func);
        constexpr const std::size_t _size = sizeof...(Args);
        Memory::Queue<__argument_t> _arguments(_data._pMemoryInterface, _data._pAllocator, _size ? _size : 1);

        int _[] = { 0, (get_argument_info<Args>(_arguments), 0)... };
        (void)_;

        return _data._pFunctions->m_fnRecordFunction(_data, _typeHandle, _function, _sName, Length, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code(), _arguments, recorder_generate_function_traits<FunctionType>());
    }

    template <typename ReturnType, typename... Args, std::size_t Length>
    static DUCKVIL_RESOURCE(function_t) record_function(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, ReturnType (*func)(Args...), const char (&_sName)[Length])
    {
        using FunctionType = ReturnType (*)(Args...);

        void* _pointer =
            _data._pMemoryInterface->m_fnFreeListAllocate_(_data._pAllocator, sizeof(__function<ReturnType (*)(Args...)>), alignof(__function<ReturnType (*)(Args...)>));

        __function<ReturnType (*)(Args...)>* _function = new(_pointer) __function<ReturnType (*)(Args...)>(func);
        constexpr const std::size_t _size = sizeof...(Args);
        Memory::Queue<__argument_t> _arguments(_data._pMemoryInterface, _data._pAllocator, _size ? _size : 1);

        int _[] = { 0, (get_argument_info<Args>(_arguments), 0)... };
        (void)_;

        return _data._pFunctions->m_fnRecordFunction(_data, _typeHandle, _function, _sName, Length, typeid(ReturnType).hash_code(), typeid(void(Args...)).hash_code(), _arguments, recorder_generate_function_traits<FunctionType>());
    }

    template <typename Type, std::size_t Length>
    static DUCKVIL_RESOURCE(enum_t) record_enum(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _data._pFunctions->m_fnRecordEnum(_data, _typeHandle, typeid(Type).hash_code(), _sName, Length);
    }

    template <typename Type, std::size_t Length>
    static DUCKVIL_RESOURCE(enum_element_t) record_enum_element(const duckvil_runtime_reflection_recorder_stuff& _data, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(enum_t) _enumHandle, Type _value, const char (&_sName)[Length])
    {
        return _data._pFunctions->m_fnRecordEnumElement(_data, _typeHandle, _enumHandle, &_value, sizeof(Type), _sName, Length);
    }

}}

typedef Duckvil::RuntimeReflection::__recorder_ftable* (*duckvil_runtime_reflection_recorder_init_callback)();

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init();