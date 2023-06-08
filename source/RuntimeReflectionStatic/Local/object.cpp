#pragma once

#include "RuntimeReflectionStatic/local/object.h"

namespace Duckvil { namespace RuntimeReflection {

    void destroy(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);

        for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_destructors.m_data); ++j)
        {
            const __destructor_t& _destructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_destructors, j);
            auto _destructorCallback = reinterpret_cast<void (*)(const Memory::ftable*, Memory::free_list_allocator*, const __ftable*, __data*, bool, void*)>(_destructor.m_pData);

            _destructorCallback(_pMemoryInterface, _pAllocator, _pReflection, _pData, _bTracked, _pObject);

            return;
        }
    }

    void destroy(const Memory::FreeList& _memory, const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject)
    {
        destroy(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pReflection, _pData, _typeHandle, _bTracked, _pObject);
    }

}}