#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace RuntimeReflection {

    __data* init(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __ftable* _pFunctions)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _data->m_aTypes = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __type_t);
        // _data->m_aFrontend = Memory::Vector<IReflectedType*>(_pMemoryInterface, _pAllocator, 1);

        return _data;
    }

    DUCKVIL_RESOURCE(type_t) get_type_by_name(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        // DUCKVIL_DYNAMIC_ARRAY_FOR(_pData->m_aTypes.m_data, __type_t, _type)
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sName) == 0)
            {
                return { _type.m_uiSlotIndex };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    const __type_t& get_type_data(__data* _pData, const DUCKVIL_RESOURCE(type_t)& _typeHandle)
    {
        return DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
    }

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    {
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            _types.Allocate(_type.m_uiSlotIndex);
        }

        return _types;
    }

    DUCKVIL_RESOURCE(type_t) get_type_by_type_id(__data* _pData, const std::size_t& _ullTypeID)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _ullTypeID)
            {
                return _type.m_uiSlotIndex;
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(property_t) get_property_handle_by_name(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX])
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++i)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return { i };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> get_constructors(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(constructor_t)> _constructors(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _constructors.Allocate({ i });
        }

        return _constructors;
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _handle.m_ID);
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data);
        Memory::Vector<DUCKVIL_RESOURCE(argument_t)> _arguments(_pMemory, _pAllocator, _size);

        for(uint32_t i = 0; i < _size; ++i)
        {
            _arguments.Allocate({ i });
        }

        return _arguments;
    }

    const __argument_t& get_argument(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _handle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __constructor_t _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, _constructorHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, _handle.m_ID);
    }

    void* get_property_by_handle(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject)
    {
        __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _type_handle.m_ID);
        __property_t _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);

        return (uint8_t*)_pObject + _property.m_ullAddress;
    }

    void* get_property_by_name(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char _sName[DUCKVIL_RUNTIME_REFLECTION_PROPERTY_NAME_MAX], const void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++i)
        {
            const __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, i);

            if(strcmp(_property.m_sName, _sName) == 0)
            {
                return (uint8_t*)_pObject + _property.m_ullAddress;
            }
        }

        return nullptr;
    }

    Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> get_inheritances(__data* _pData, Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        Memory::Vector<DUCKVIL_RESOURCE(inheritance_t)> _inheritances(_pMemory, _pAllocator, DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data));

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data); ++i)
        {
            _inheritances.Allocate({ i });
        }

        return _inheritances;
    }

    const __inheritance_t& get_inheritance(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(inheritance_t) _inheritanceHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        return DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, _inheritanceHandle.m_ID);
    }

    bool inherits(__data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(type_t) _inheritanceHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        const __type_t& _inheritanceType = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_inheritances.m_data); ++i)
        {
            auto& _inheritance = DUCKVIL_SLOT_ARRAY_GET(_type.m_inheritances, i);

            if(_inheritance.m_ullInheritanceTypeID == _inheritanceType.m_ullTypeID)
            {
                return true;
            }
        }

        return false;
    }

}}

Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__ftable* _functions = (Duckvil::RuntimeReflection::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__ftable), alignof(Duckvil::RuntimeReflection::__ftable));

    _functions->m_fnInit = &Duckvil::RuntimeReflection::init;

    _functions->m_fnGetTypeHandleByName = &Duckvil::RuntimeReflection::get_type_by_name;
    _functions->m_fnGetPropertyByName = &Duckvil::RuntimeReflection::get_property_by_name;
    _functions->m_fnGetTypeHandleByTypeID = &Duckvil::RuntimeReflection::get_type_by_type_id;
    _functions->m_fnGetType = &Duckvil::RuntimeReflection::get_type_data;
    _functions->m_fnGetTypes = &Duckvil::RuntimeReflection::get_types;
    _functions->m_fnGetPropertyHandleByName = &Duckvil::RuntimeReflection::get_property_handle_by_name;
    _functions->m_fnGetConstructors = &Duckvil::RuntimeReflection::get_constructors;
    _functions->m_fnGetArguments = &Duckvil::RuntimeReflection::get_arguments;
    _functions->m_fnGetArgument = &Duckvil::RuntimeReflection::get_argument;
    _functions->m_fnGetPropertyByName = &Duckvil::RuntimeReflection::get_property_by_name;
    _functions->m_fnGetInheritances = &Duckvil::RuntimeReflection::get_inheritances;
    _functions->m_fnGetInheritance = &Duckvil::RuntimeReflection::get_inheritance;
    _functions->m_fnInherits = &Duckvil::RuntimeReflection::inherits;

    return _functions;
}