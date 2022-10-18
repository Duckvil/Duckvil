#pragma once

#include "RuntimeReflectionStatic/local/argument.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);
    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle);
    __argument_t get_argument(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle);
    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);
    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);
    const __argument_t& get_argument(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle);

}}