#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace RuntimeReflection {

    __data* init(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __ftable* _pFunctions)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _data->m_aTypes = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __type_t);

        return _data;
    }

    DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char _sName[DUCKVIL_RUNTIME_REFLECTION_TYPE_NAME_MAX])
    {
        // DUCKVIL_DYNAMIC_ARRAY_FOR(_pData->m_aTypes.m_data, __type_t, _type)
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); i++)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(strcmp(_type.m_sTypeName, _sName) == 0)
            {
                return { _type.m_uiSlotIndex };
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    void* get_property(__data* _pData, DUCKVIL_RESOURCE(type_t) _type_handle, DUCKVIL_RESOURCE(property_t) _handle, const void* _pObject)
    {
        __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _type_handle.m_ID);
        __property_t _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, _handle.m_ID);

        return (uint8_t*)_pObject + _property.m_ullAddress;
    }

}}

Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__ftable* _functions = (Duckvil::RuntimeReflection::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__ftable), alignof(Duckvil::RuntimeReflection::__ftable));

    _functions->m_fnInit = &Duckvil::RuntimeReflection::init;

    _functions->m_fnGetType = &Duckvil::RuntimeReflection::get_type;
    _functions->m_fnGetProperty = &Duckvil::RuntimeReflection::get_property;

    return _functions;
}