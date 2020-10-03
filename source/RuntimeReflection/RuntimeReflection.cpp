#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace RuntimeReflection {

    bool init(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _pFunctions->m_pData = _data;
        _data->m_aTypes = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __type_t);

        return true;
    }

    DUCKVIL_RESOURCE(type_t) record_type(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[32])
    {
        __type_t _type = {};

        _type.m_ullTypeID = _ullTypeID;
        memcpy(_type.m_sTypeName, _sTypeName, 32);

        uint32_t _type_handle = DUCKVIL_SLOT_ARRAY_INSERT(_pMemoryInterface, _pAllocator, _pData->m_aTypes, _type);

        DUCKVIL_RESOURCE(type_t) _handle = {};

        _handle.m_ID = _type_handle;

        return _handle;
    }

}}

Duckvil::RuntimeReflection::__functions* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__functions* _functions = (Duckvil::RuntimeReflection::__functions*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__functions), alignof(Duckvil::RuntimeReflection::__functions));

    _functions->m_fnInit = &Duckvil::RuntimeReflection::init;
    _functions->m_fnRecordType = &Duckvil::RuntimeReflection::record_type;

    return _functions;
}