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

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE_DECLARE(type_t);

    slot(__type_t,
    {
        std::size_t m_ullTypeID;
        char m_sTypeName[32];
    });

    struct __data
    {
        DUCKVIL_SLOT_ARRAY(__type_t) m_aTypes;
    };

    struct __functions
    {
        __data* m_pData;
        bool (*m_fnInit)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions);

        DUCKVIL_RESOURCE(type_t) (*m_fnRecordType)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char* _sTypeName);
    };

    DUCKVIL_RESOURCE(type_t) record_type(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char _sTypeName[32]);

    bool init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __functions* _pFunctions);

}}

typedef Duckvil::RuntimeReflection::__functions* (*duckvil_runtime_reflection_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__functions* duckvil_runtime_reflection_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);