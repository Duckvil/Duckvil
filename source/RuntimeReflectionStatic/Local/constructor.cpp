#pragma once

#include "RuntimeReflectionStatic/local/constructor.h"

namespace Duckvil { namespace RuntimeReflection {

    const __constructor_t& get_constructor(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        return _pReflection->m_fnGetConstructor(_pData, _typeHandle, _constructorHandle);
    }

}}