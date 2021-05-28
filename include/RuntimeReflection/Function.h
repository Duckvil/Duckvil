#pragma once

#include <typeinfo>

namespace Duckvil { namespace RuntimeReflection {

    struct __ifunction
    {
        virtual ~__ifunction() { }
    };

    template <typename ReturnType, typename... Args>
    struct __proxy_static_function : public __ifunction
    {
        virtual ~__proxy_static_function() { }

        virtual ReturnType Invoke(const Args&... _vArgs) = 0;
    };

    template <typename ReturnType, typename... Args>
    struct __proxy_member_function : public __ifunction
    {
        virtual ~__proxy_member_function() { }

        virtual ReturnType Invoke(void* _pObject, const Args&... _vArgs) = 0;
    };

    template <typename T>
    struct __function;

// Member function with specific arguments which returns void
    template <typename Type, typename... Args>
    struct __function<void(Type::*)(Args...)> : public __proxy_member_function<void, Args...>
    {
        typedef void (Type::*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke(void* _pObject, const Args&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))(_vArgs...);
        }
    };

// Member function with specific arguments which returns specific type
    template <typename Type, typename ReturnType, typename... Args>
    struct __function<ReturnType(Type::*)(Args...)> : public __proxy_member_function<ReturnType, Args...>
    {
        typedef ReturnType (Type::*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(void* _pObject, const Args&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))(_vArgs...);
        }
    };

// Static function with specific arguments which returns void
    template <typename... Args>
    struct __function<void(*)(Args...)> : public __proxy_static_function<void, Args...>
    {
        typedef void (*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke(const Args&... _vArgs) final override
        {
            m_fnFunction(_vArgs...);
        }
    };

// Static function with specific arguments which returns specific type
    template <typename ReturnType, typename... Args>
    struct __function<ReturnType(*)(Args...)> : public __proxy_static_function<ReturnType, Args...>
    {
        typedef ReturnType (*FunctionCallback)(Args...);

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(const Args&... _vArgs) final override
        {
            return m_fnFunction(_vArgs...);
        }
    };

// Member const function with specific arguments which returns void
    template <typename Type, typename... Args>
    struct __function<void(Type::*)(Args...) const> : public __proxy_member_function<void, Args...>
    {
        typedef void (Type::*FunctionCallback)(Args...) const;

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke(void* _pObject, const Args&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))(_vArgs...);
        }
    };

// Member function with specific arguments which returns specific type
    template <typename Type, typename ReturnType, typename... Args>
    struct __function<ReturnType(Type::*)(Args...) const> : public __proxy_member_function<ReturnType, Args...>
    {
        typedef ReturnType (Type::*FunctionCallback)(Args...) const;

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(void* _pObject, const Args&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))(_vArgs...);
        }
    };

}}