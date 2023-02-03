#pragma once

#include <typeinfo>

namespace Duckvil { namespace RuntimeReflection {

    struct __ifunction
    {
        virtual ~__ifunction() { }

        virtual void* GetRawPointer() const = 0;
    };

    template <typename ReturnType, typename... Args>
    struct __proxy_static_function : public __ifunction
    {
        virtual ~__proxy_static_function() { }

        virtual ReturnType Invoke(Args... _vArgs) = 0;

        virtual void* GetRawPointer() const override = 0;
    };

    template <typename ReturnType>
    struct __proxy_static_function<ReturnType> : public __ifunction
    {
        virtual ~__proxy_static_function() { }

        virtual ReturnType Invoke() = 0;

        virtual void* GetRawPointer() const override = 0;
    };

    template <typename ReturnType, typename... Args>
    struct __proxy_member_function : public __ifunction
    {
        virtual ~__proxy_member_function() { }

        virtual ReturnType Invoke(void* _pObject, Args... _vArgs) = 0;

        virtual void* GetRawPointer() const override = 0;
    };

    template <typename ReturnType>
    struct __proxy_member_function<ReturnType> : public __ifunction
    {
        virtual ~__proxy_member_function() { }

        virtual ReturnType Invoke(void* _pObject) = 0;

        virtual void* GetRawPointer() const override = 0;
    };

    template <typename ReturnType, typename... Args>
    struct __proxy_member_const_function : public __ifunction
    {
        virtual ~__proxy_member_const_function() { }

        virtual ReturnType Invoke(const void* _pObject, Args... _vArgs) = 0;

        virtual void* GetRawPointer() const override = 0;
    };

    template <typename ReturnType>
    struct __proxy_member_const_function<ReturnType> : public __ifunction
    {
        virtual ~__proxy_member_const_function() { }

        virtual ReturnType Invoke(const void* _pObject) = 0;

        virtual void* GetRawPointer() const override = 0;
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

        inline void Invoke(void* _pObject, Args... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))(_vArgs...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <typename Type>
    struct __function<void(Type::*)()> : public __proxy_member_function<void>
    {
        typedef void (Type::* FunctionCallback)();

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke(void* _pObject) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))();
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
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

        inline ReturnType Invoke(void* _pObject, Args... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))(_vArgs...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <typename Type, typename ReturnType, typename... Args>
    struct __function<ReturnType(Type::*)(Args&&...)> : public __proxy_member_function<ReturnType, Args&&...>
    {
        typedef ReturnType(Type::* FunctionCallback)(Args&&...);

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(void* _pObject, Args&&... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))(std::forward<Args>(_vArgs)...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <typename Type, typename ReturnType>
    struct __function<ReturnType(Type::*)()> : public __proxy_member_function<ReturnType>
    {
        typedef ReturnType(Type::* FunctionCallback)();

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(void* _pObject) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))();
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
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

        inline void Invoke(Args... _vArgs) final override
        {
            m_fnFunction(_vArgs...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <>
    struct __function<void(*)()> : public __proxy_static_function<void>
    {
        typedef void (*FunctionCallback)();

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke() final override
        {
            m_fnFunction();
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
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

        inline ReturnType Invoke(Args... _vArgs) final override
        {
            return m_fnFunction(_vArgs...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <typename ReturnType>
    struct __function<ReturnType(*)()> : public __proxy_static_function<ReturnType>
    {
        typedef ReturnType(*FunctionCallback)();

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke() final override
        {
            return m_fnFunction();
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

// Member const function with specific arguments which returns void
    template <typename Type, typename... Args>
    struct __function<void(Type::*)(Args...) const> : public __proxy_member_const_function<void, Args...>
    {
        typedef void (Type::*FunctionCallback)(Args...) const;

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke(const void* _pObject, Args... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))(_vArgs...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <typename Type>
    struct __function<void(Type::*)() const> : public __proxy_member_const_function<void>
    {
        typedef void (Type::* FunctionCallback)() const;

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline void Invoke(const void* _pObject) final override
        {
            Type* _object = (Type*)_pObject;

            (_object->*(m_fnFunction))();
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

// Member const function with specific arguments which returns specific type
    template <typename Type, typename ReturnType, typename... Args>
    struct __function<ReturnType(Type::*)(Args...) const> : public __proxy_member_const_function<ReturnType, Args...>
    {
        typedef ReturnType (Type::*FunctionCallback)(Args...) const;

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(const void* _pObject, Args... _vArgs) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))(_vArgs...);
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

    template <typename Type, typename ReturnType>
    struct __function<ReturnType(Type::*)() const> : public __proxy_member_const_function<ReturnType>
    {
        typedef ReturnType(Type::* FunctionCallback)() const;

        FunctionCallback m_fnFunction;

        __function(FunctionCallback _fnCallback) :
            m_fnFunction(_fnCallback)
        {

        }

        ~__function() { }

        inline ReturnType Invoke(const void* _pObject) final override
        {
            Type* _object = (Type*)_pObject;

            return (_object->*(m_fnFunction))();
        }

        inline void* GetRawPointer() const final override
        {
            return (void*&)m_fnFunction;
        }
    };

}}