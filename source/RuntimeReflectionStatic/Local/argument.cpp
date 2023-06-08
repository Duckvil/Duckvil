#pragma once

#include "RuntimeReflectionStatic/local/argument.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const __ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        return _pReflection->m_fnGetConstructorArguments(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle, _constructorHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_arguments(_pReflection, _pData, _heapContext.m_heap, _typeHandle, _constructorHandle);
    }

    __argument_t get_argument(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _constructorHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        return _pReflection->m_fnGetConstructorArgument(_pData, _typeHandle, _constructorHandle, _argumentHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const __ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        return _pReflection->m_fnGetFunctionArguments(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle, _functionHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(argument_t)> get_arguments(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_arguments(_pReflection, _pData, _heapContext.m_heap, _typeHandle, _functionHandle);
    }

    const __argument_t& get_argument(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, DUCKVIL_RESOURCE(argument_t) _argumentHandle)
    {
        return _pReflection->m_fnGetFunctionArgument(_pData, _typeHandle, _functionHandle, _argumentHandle);
    }

}}