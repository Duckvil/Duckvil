#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename... Args>
    DUCKVIL_RESOURCE(constructor_t) get_constructor_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetConstructorHandleByTypeID(_pData, _typeHandle, typeid(void*(Args...)).hash_code());
    }

    const __constructor_t& get_constructor(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);

}}