#include "RuntimeDatabase/RuntimeDatabase.h"

namespace Duckvil { namespace RuntimeDatabase {

    

}}

Duckvil::RuntimeDatabase::__data* duckcvil_runtimedatabase_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeDatabase::__data _data = {};
    Duckvil::RuntimeDatabase::__data* _result = nullptr;

    _result = (Duckvil::RuntimeDatabase::__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, &_data, sizeof(Duckvil::RuntimeDatabase::__data), alignof(Duckvil::RuntimeDatabase::__data));

    _result->m_pAllocator = _pAllocator;

    return _result;
}