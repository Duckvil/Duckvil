#pragma once

#include "RuntimeReflectionStatic/local/property.h"

namespace Duckvil { namespace RuntimeReflection {

    const __property_t& get_property(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _propertyHandle)
    {
        return _pReflection->m_fnGetPropertyByHandle(_pData, _typeHandle, _propertyHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(property_t)> get_properties(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetProperties(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

}}