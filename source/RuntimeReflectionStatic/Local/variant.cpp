#pragma once

#include "RuntimeReflectionStatic/local/variant.h"

namespace Duckvil { namespace RuntimeReflection {

    const __variant_t& get_variant_key(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle)
    {
        return _pReflection->m_fnGetVariantKeyByHandle(_pData, _typeHandle, _handle);
    }

    const __variant_t& get_variant_value(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle)
    {
        return _pReflection->m_fnGetVariantValueByHandle(_pData, _typeHandle, _handle);
    }

}}