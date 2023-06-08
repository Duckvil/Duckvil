#pragma once

#include "RuntimeReflectionStatic/local/function.h"

namespace Duckvil { namespace RuntimeReflection {

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const __ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetFunctions(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator(), _typeHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        const Memory::free_list_context& _heapContext = Memory::heap_get_current();

        return get_functions(_pReflection, _pData, _heapContext.m_heap, _typeHandle);
    }

    const __function_t& get_function(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        return _pReflection->m_fnGetFunctionByHandle(_pData, _typeHandle, _functionHandle);
    }

}}