#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    template <typename Type, typename... Args, std::size_t Length>
    void invoke(const __ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Args&... _vArgs)
    {
        const auto& _typeHandle = _pReflection->m_fnGetTypeHandleByTypeID(_pData, typeid(Type).hash_code());
        auto _func = static_cast<__function<void(*)(Args...)>*>(_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));

        (*(_func->m_fnFunction))(_vArgs...);
    }

    template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
    ReturnType invoke(const __ftable* _pReflection, __data* _pData, const char (&_sName)[Length], const Args&... _vArgs)
    {
        const auto& _typeHandle = _pReflection->m_fnGetTypeHandleByTypeID(_pData, typeid(Type).hash_code());
        auto _func = static_cast<__function<ReturnType(*)(Args...)>*>(_pReflection->m_fnGetFunctionCallback(_pData, _typeHandle, _sName, Length, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));

        return (*(_func->m_fnFunction))(_vArgs...);
    }

    template <typename... Args>
    void invoke_static(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        auto _func = static_cast<__proxy_static_function<void, Args...>*>(_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));

        _func->Invoke(_vArgs...);
    }

    template <typename ReturnType, typename... Args>
    ReturnType invoke_static_result(const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(function_t) _functionHandle, const Args&... _vArgs)
    {
        auto _func = static_cast<__proxy_static_function<ReturnType, Args...>*>(_pReflection->m_fnGetFunctionCallbackByHandle(_pData, _typeHandle, _functionHandle, DUCKVIL_RUNTIME_REFLECTION_ARGS_TYPE_ID(Args...)));

        return _func->Invoke(_vArgs...);
    }

}}