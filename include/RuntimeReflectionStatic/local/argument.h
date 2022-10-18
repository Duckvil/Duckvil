#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);
    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);
    __argument_t get_argument(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle);
    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);
    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);
    const __argument_t& get_argument(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle);

}}