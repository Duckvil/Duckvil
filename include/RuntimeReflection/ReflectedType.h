#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

// #ifndef DUCKVIL_FRONTEND_REFLECTION_API
// #define DUCKVIL_FRONTEND_REFLECTION_API
// #endif

// struct duckvil_frontend_reflection_context;

// #ifndef GImGui
// extern DUCKVIL_FRONTEND_REFLECTION_API duckvil_frontend_reflection_context* g_pDuckvilFrontendReflectionContext;
// #endif

// struct duckvil_frontend_reflection_context
// {
//     Duckvil::RuntimeReflection::__ftable* m_pReflection;
//     Duckvil::RuntimeReflection::__data* m_pReflectionData;
// };

namespace Duckvil { namespace RuntimeReflection {

    template<typename...>
    struct types
    {
        using type = types;
    };

    template<typename T>
    struct tag
    {
        using type = T;
    };

    // template<typename Tag>
    // using type_t = typename Tag::type;

    template <typename Type = void>
    class ReflectedType
    {
    private:
        __data* m_pReflectionData;
        __ftable* m_pReflection;
        Memory::FreeList m_heap;
        DUCKVIL_RESOURCE(type_t) m_typeHandle;

    public:
        ReflectedType(__ftable* _pReflection, __data* _pReflectionData, const Memory::FreeList& _heap) :
            m_pReflectionData(_pReflectionData),
            m_pReflection(_pReflection),
            m_heap(_heap)
        {
            m_typeHandle = get_type<Type>(m_pReflection, _pReflectionData);
        }

        ~ReflectedType()
        {

        }

        template <typename... Args>
        void* Create(Args... _vArgs)
        {
            return create<Args...>(m_heap, m_pReflectionData, m_typeHandle, _vArgs...);
        }

        template <typename Type, typename... Args, std::size_t Length>
        __function<void(Type::*)(Args...)>* GetFunctionCallback(const char (&_sName)[Length])
        {
            return get_function_callback<Type, Args...>(m_pReflectionData, m_typeHandle, _sName);
        }

        template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
        __function<ReturnType(Type::*)(Args...)>* GetFunctionCallback(const char (&_sName)[Length])
        {
            return get_function_callback<ReturnType, Type, Args...>(m_pReflectionData, m_typeHandle, _sName);
        }

        template <typename... Args, std::size_t Length>
        __duckvil_resource_function_t GetFunctionHandle(const char (&_sName)[Length])
        {
            return get_function_handle<Args...>(m_pReflectionData, m_typeHandle, _sName);
        }

        template <typename... Args>
        void Invoke(__duckvil_resource_function_t _functionHandle, void* _pObject, Args... _vArgs)
        {
            invoke_member<Args...>(m_pReflectionData, m_typeHandle, _functionHandle, _pObject, _vArgs...);
        }

        template <typename... Args, std::size_t Length>
        void Invoke(const char (&_sName)[Length], void* _pObject, Args... _vArgs)
        {
            Invoke<Args...>(GetFunctionHandle<Args...>(_sName), _pObject, _vArgs...);
        }

        template <typename ReturnType, typename... Args>
        ReturnType InvokeStatic(__duckvil_resource_function_t _functionHandle, Args... _vArgs)
        {
            return invoke_static_result<ReturnType, Args...>(m_pReflectionData, m_typeHandle, _functionHandle, _vArgs...);
        }

        template <typename ReturnType, typename... Args, std::size_t Length>
        ReturnType InvokeStatic(const char (&_sName)[Length], Args... _vArgs)
        {
            return InvokeStatic<ReturnType, Args...>(GetFunctionHandle<Args...>(_sName), _vArgs...);
        }
    };

    template <>
    class ReflectedType<void>
    {
    private:
        __data* m_pReflectionData;
        __ftable* m_pReflection;
        Memory::FreeList m_heap;
        DUCKVIL_RESOURCE(type_t) m_typeHandle;

    public:
        ReflectedType(__ftable* _pReflection, __data* _pReflectionData, const Memory::FreeList& _heap, DUCKVIL_RESOURCE(type_t) _typeHandle) :
            m_pReflectionData(_pReflectionData),
            m_pReflection(_pReflection),
            m_heap(_heap),
            m_typeHandle(_typeHandle)
        {

        }

        template <typename Type>
        ReflectedType(__ftable* _pReflection, __data* _pReflectionData, const Memory::FreeList& _heap, tag<Type>) :
            m_pReflectionData(_pReflectionData),
            m_pReflection(_pReflection),
            m_heap(_heap)
        {
            m_typeHandle = get_type<Type>(m_pReflection, _pReflectionData);
        }

        ~ReflectedType()
        {

        }

        template <typename... Args>
        void* Create(Args... _vArgs)
        {
            return create<Args...>(m_heap, m_pReflectionData, m_typeHandle, _vArgs...);
        }

// Constructors
        template <typename... Args>
        __duckvil_resource_constructor_t GetConstructorHandle()
        {
            return get_constructor_handle<Args...>(m_pReflectionData, m_typeHandle);
        }

        Memory::Vector<__duckvil_resource_constructor_t> GetConstructors()
        {
            return m_pReflection->m_fnGetConstructors(m_pReflectionData, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_typeHandle); // get_constructors(m_pReflectionData, m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_typeHandle);
        }

// Functions
        template <typename Type, typename... Args, std::size_t Length>
        __function<void(Type::*)(Args...)>* GetFunctionCallback(const char (&_sName)[Length])
        {
            return get_function_callback<Type, Args...>(m_pReflectionData, m_typeHandle, _sName);
        }

        template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
        __function<ReturnType(Type::*)(Args...)>* GetFunctionCallback(const char (&_sName)[Length])
        {
            return get_function_callback<ReturnType, Type, Args...>(m_pReflectionData, m_typeHandle, _sName);
        }

        template <typename ReturnType, typename Type, typename... Args>
        __function<ReturnType(Type::*)(Args...)>* GetFunctionCallback(DUCKVIL_RESOURCE(function_t) _functionHandle)
        {
            return get_function_callback<ReturnType, Type, Args...>(m_pReflectionData, m_typeHandle, _functionHandle);
        }

        template <typename Type, typename... Args>
        __function<void(Type::*)(Args...)>* GetFunctionCallback(DUCKVIL_RESOURCE(function_t) _functionHandle)
        {
            return get_function_callback<Type, Args...>(m_pReflectionData, m_typeHandle, _functionHandle);
        }

        template <typename... Args, std::size_t Length>
        __duckvil_resource_function_t GetFunctionHandle(const char (&_sName)[Length])
        {
            return get_function_handle<Args...>(m_pReflectionData, m_typeHandle, _sName);
        }

        template <typename... Args>
        void Invoke(__duckvil_resource_function_t _functionHandle, void* _pObject, Args... _vArgs)
        {
            invoke_member<Args...>(m_pReflectionData, m_typeHandle, _functionHandle, _pObject, _vArgs...);
        }

        template <typename... Args, std::size_t Length>
        void Invoke(const char (&_sName)[Length], void* _pObject, Args... _vArgs)
        {
            Invoke<Args...>(GetFunctionHandle<Args...>(_sName), _pObject, _vArgs...);
        }

        template <typename ReturnType, typename... Args>
        ReturnType InvokeStatic(__duckvil_resource_function_t _functionHandle, Args... _vArgs)
        {
            return invoke_static_result<ReturnType, Args...>(m_pReflectionData, m_typeHandle, _functionHandle, _vArgs...);
        }

        template <typename ReturnType, typename... Args, std::size_t Length>
        ReturnType InvokeStatic(const char (&_sName)[Length], Args... _vArgs)
        {
            return InvokeStatic<ReturnType, Args...>(GetFunctionHandle<Args...>(_sName), _vArgs...);
        }
    };

}}