#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename... Args, std::size_t Length>
    DUCKVIL_RESOURCE(function_t) get_function_handle(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return _pReflection->m_fnGetFunctionHandle(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...));
    }

    template <typename Type, typename... Args>
    DUCKVIL_RESOURCE(function_t) get_function_handle(const __ftable* _pReflection, __data* _pData, void (Type::*_fnCallback)(Args...))
    {
        const auto& _typeHandle = get_type<Type>(_pReflection, _pData);

        return _pReflection->m_fnGetFunctionHandleByPointer(_pData, _typeHandle, _fnCallback);
    }

    template <typename Type, typename... Args>
    DUCKVIL_RESOURCE(function_t) get_function_handle(const __ftable* _pReflection, __data* _pData, void (*_fnCallback)(Args...))
    {
        const auto& _typeHandle = get_type<Type>(_pReflection, _pData);

        return _pReflection->m_fnGetFunctionHandleByPointer(_pData, _typeHandle, _fnCallback);
    }

// TODO: Check it
    template <typename... Args, std::size_t Length>
    __function<void(*)(Args...)>* get_static_function_callback(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return static_cast<__function<void(*)(Args...)>*>(_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));
    }

    template <typename Type, typename... Args, std::size_t Length>
    __function<void(Type::*)(Args...)>* get_function_callback_m(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return static_cast<__function<void(Type::*)(Args...)>*>(_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));
    }

    template <typename Type, typename... Args>
    __function<void(Type::*)(Args...)>* get_function_callback_m(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _argsTypeID = DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...);
        const __function_t& _function = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, _functionHandle.m_ID);

        return static_cast<__function<void(Type::*)(Args...)>*>(_function.m_pFunction);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    __function<ReturnType(Type::*)(Args...)>* get_function_callback_mr(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, const char (&_sName)[Length])
    {
        return static_cast<__function<ReturnType(Type::*)(Args...)>*>(_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));
    }






    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const __ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle);

    Memory::Vector<DUCKVIL_RESOURCE(function_t)> get_functions(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle);

    const __function_t& get_function(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle);

}}