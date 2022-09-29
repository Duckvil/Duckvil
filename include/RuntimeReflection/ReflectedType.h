#pragma once

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "HotReloader/ITrackKeeper.h"

#include <stdexcept>

namespace Duckvil { namespace RuntimeReflection {

    class ReflectedArgument
    {
        template <typename ReturnType, typename... Args>
        friend class ReflectedFunction;
        friend class IReflectedFunction;
    private:
        __duckvil_resource_argument_t m_handle;
        __duckvil_resource_function_t m_functionHandle;
        __duckvil_resource_type_t m_typeHandle;
        const RuntimeReflection::__argument_t* m_argument;

        ReflectedArgument(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_function_t& _functionHandle, const __duckvil_resource_argument_t& _handle) :
            m_typeHandle(_typeHandle),
            m_functionHandle(_functionHandle),
            m_handle(_handle),
            m_argument(&RuntimeReflection::get_argument(_typeHandle, _functionHandle, _handle))
        {

        }

    public:
        ~ReflectedArgument()
        {

        }

        size_t GetTypeID() const
        {
            return m_argument->m_ullTypeID;
        }
    };

    class ReflectedProperty
    {
        friend class ReflectedType;
    private:
        __duckvil_resource_property_t m_handle;
        __duckvil_resource_type_t m_typeHandle;
        const RuntimeReflection::__property_t* m_property;

        ReflectedProperty(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_property_t& _handle) :
            m_typeHandle(_typeHandle),
            m_handle(_handle),
            m_property(&RuntimeReflection::get_property(_typeHandle, _handle))
        {

        }

    public:
        ~ReflectedProperty()
        {

        }

        inline const __duckvil_resource_property_t& GetHandle() const
        {
            return m_handle;
        }

        inline size_t GetTypeID() const
        {
            return m_property->m_ullTypeID;
        }

        inline const char* GetName() const
        {
            return m_property->m_sName;
        }

        inline size_t GetAddress() const
        {
            return m_property->m_ullAddress;
        }
    };

    class IReflectedFunction
    {
        friend class ReflectedType;
    protected:
        __duckvil_resource_function_t m_handle;
        __duckvil_resource_type_t m_typeHandle;
        const RuntimeReflection::__function_t* m_pFunction;

        IReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_function_t& _handle) :
            m_typeHandle(_typeHandle),
            m_handle(_handle),
            m_pFunction(&RuntimeReflection::get_function(_typeHandle, _handle))
        {

        }

    public:
        template <size_t Length>
        IReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const char(&_sName)[Length]) :
            m_typeHandle(_typeHandle),
            m_handle(RuntimeReflection::get_function_handle(_typeHandle, _sName)),
            m_pFunction(&RuntimeReflection::get_function(_typeHandle, m_handle))
        {

        }

        virtual ~IReflectedFunction()
        {

        }

        inline const __duckvil_resource_function_t& GetHandle() const
        {
            return m_handle;
        }

        Memory::Vector<ReflectedArgument> GetArguments(const Memory::FreeList& _heap) const
        {
            const auto& _args = RuntimeReflection::get_arguments(_heap, m_typeHandle, m_handle);

            Memory::Vector<ReflectedArgument> _res;

            _heap.Allocate(_res, _args.Size());

            for (uint32_t _i = 0; _i < _args.Size(); _i++)
            {
                _res.Allocate(ReflectedArgument(m_typeHandle, m_handle, _args[_i]));
            }

            return _res;
        }

        inline size_t GetReturnTypeID() const
        {
            return m_pFunction->m_ullReturnTypeID;
        }

        inline const char* GetName() const
        {
            return m_pFunction->m_sFunctionName;
        }

        inline const void* GetPointer() const
        {
            return m_pFunction->m_pRawFunction;
        }

        inline size_t GetArgumentsTypeID() const
        {
            return m_pFunction->m_ullArgumentsTypeID;
        }

        template <typename... Args>
        void InvokeM(void* _pObject, Args... _vArgs)
        {
            if(((uint8_t)m_pFunction->m_traits & (uint8_t)RuntimeReflection::function_traits::is_static))
            {
                throw std::runtime_error("ReflectedFunction is static!");
            }

            RuntimeReflection::invoke_member<Args...>(m_typeHandle, m_handle, _pObject, _vArgs...);
        }

        template <typename... Args>
        void InvokeS(Args... _vArgs)
        {
            if(!((uint8_t)m_pFunction->m_traits & (uint8_t)function_traits::is_static))
            {
                throw std::runtime_error("ReflectedFunction isn't static!");
            }

            RuntimeReflection::invoke_static<Args...>(m_typeHandle, m_handle, _vArgs...);
        }

        template <typename ReturnType, typename... Args>
        ReturnType InvokeMR(void* _pObject, Args... _vArgs)
        {
            if(((uint8_t)m_pFunction->m_traits & (uint8_t)RuntimeReflection::function_traits::is_static))
            {
                throw std::runtime_error("ReflectedFunction is static!");
            }

            return RuntimeReflection::invoke_member_result<ReturnType, Args...>(m_typeHandle, m_handle, _pObject, _vArgs...);
        }

        template <typename ReturnType, typename... Args>
        ReturnType InvokeSR(Args... _vArgs)
        {
            if(!((uint8_t)m_pFunction->m_traits & (uint8_t)function_traits::is_static))
            {
                throw std::runtime_error("ReflectedFunction isn't static!");
            }

            return RuntimeReflection::invoke_static_result<ReturnType, Args...>(m_typeHandle, m_handle, _vArgs...);
        }
    };

    template <typename ReturnType = void, typename... Args>
    class ReflectedFunction : public IReflectedFunction
    {
        friend class ReflectedType;
    private:
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_function_t& _handle) :
            IReflectedFunction(_typeHandle, _handle)
        {

        }

    public:
        template <size_t Length>
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const char(&_sName)[Length]) :
            IReflectedFunction(_typeHandle, _sName)
        {

        }

        ~ReflectedFunction()
        {

        }

        ReturnType InvokeM(void* _pObject, Args... _vArgs)
        {
            return InvokeMR<ReturnType, Args...>(_pObject, _vArgs...);
        }

        ReturnType InvokeS(Args... _vArgs)
        {
            return InvokeSR<ReturnType, Args...>(_vArgs...);
        }
    };

    template <typename ReturnType>
    class ReflectedFunction<ReturnType, void> : public IReflectedFunction
    {
        friend class ReflectedType;
    private:
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_function_t& _handle) :
            IReflectedFunction(_typeHandle, _handle)
        {

        }

    public:
        template <size_t Length>
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const char(&_sName)[Length]) :
            IReflectedFunction(_typeHandle, _sName)
        {
            
        }

        ~ReflectedFunction()
        {

        }

        ReturnType InvokeM(void* _pObject)
        {
            return IReflectedFunction::InvokeMR<ReturnType>(_pObject);
        }

        ReturnType InvokeS()
        {
            return IReflectedFunction::InvokeSR<ReturnType>();
        }
    };

    template <>
    class ReflectedFunction<void, void> : public IReflectedFunction
    {
        friend class ReflectedType;
    private:
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_function_t& _handle) :
            IReflectedFunction(_typeHandle, _handle)
        {

        }

    public:
        template <size_t Length>
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const char(&_sName)[Length]) :
            IReflectedFunction(_typeHandle, _sName)
        {

        }

        ~ReflectedFunction()
        {

        }

        void InvokeM(void* _pObject)
        {
            IReflectedFunction::InvokeM(_pObject);
        }

        void InvokeS()
        {
            IReflectedFunction::InvokeS();
        }
    };

    template <typename... Args>
    class ReflectedFunction<void, Args...> : public IReflectedFunction
    {
        friend class ReflectedType;
    private:
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const __duckvil_resource_function_t& _handle) :
            IReflectedFunction(_typeHandle, _handle)
        {

        }

    public:
        template <size_t Length>
        ReflectedFunction(const __duckvil_resource_type_t& _typeHandle, const char(&_sName)[Length]) :
            IReflectedFunction(_typeHandle, _sName)
        {

        }

        ~ReflectedFunction()
        {

        }

        void InvokeM(void* _pObject, Args... vArgs)
        {
            IReflectedFunction::InvokeM<Args...>(_pObject, vArgs...);
        }

        void InvokeS(Args... vArgs)
        {
            IReflectedFunction::InvokeS<Args...>(vArgs...);
        }
    };

    class ReflectedType
    {
    public:
        template<typename T>
        struct Tag
        {
            using type = T;
        };

    private:
        __duckvil_resource_type_t m_typeHandle;
        const RuntimeReflection::__type_t* m_type;

    public:
        ReflectedType(const __duckvil_resource_type_t& _typeHandle) :
            m_type(&RuntimeReflection::get_type(_typeHandle)),
            m_typeHandle(_typeHandle)
        {

        }

        template <typename T>
        ReflectedType(ReflectedType::Tag<T>) :
            m_typeHandle(RuntimeReflection::get_type<T>()),
            m_type(&RuntimeReflection::get_type(m_typeHandle))
        {

        }

        template <size_t Length>
        ReflectedType(const char (&_sName)[Length], const std::vector<const char*>& _aNamespaces = {}) :
            m_typeHandle(RuntimeReflection::get_type(_sName, _aNamespaces)),
            m_type(&RuntimeReflection::get_type(m_typeHandle))
        {

        }

        ~ReflectedType()
        {

        }

        inline const char* GetName() const
        {
            return m_type->m_sTypeName;
        }

        inline size_t GetTypeID() const
        {
            return m_type->m_ullTypeID;
        }

        template <typename KeyType>
        const __variant& GetMeta(const KeyType& _key) const
        {
            return RuntimeReflection::get_meta(m_typeHandle, _key);
        }

        inline const __duckvil_resource_type_t& GetHandle() const
        {
            return m_typeHandle;
        }

        template <typename T>
        inline bool Inherits() const
        {
            return RuntimeReflection::inherits<T>(m_typeHandle);
        }

        template <typename... Args, size_t Length>
        const RuntimeReflection::__duckvil_resource_function_t& GetFunctionHandle(const char (&_sName)[Length]) const
        {
            return RuntimeReflection::get_function_handle<Args...>(m_typeHandle, _sName);
        }

        Memory::Vector<ReflectedProperty> GetProperties(const Memory::FreeList& _heap) const
        {
            const auto& _props = RuntimeReflection::get_properties(_heap, m_typeHandle);

            Memory::Vector<ReflectedProperty> _res;

            _heap.Allocate(_res, _props.Size());

            for(uint32_t _i = 0; _i < _props.Size(); _i++)
            {
                _res.Allocate(ReflectedProperty(m_typeHandle, _props[_i]));
            }

            return _res;
        }

        Memory::Vector<IReflectedFunction> GetFunctions(const Memory::FreeList& _heap) const
        {
            const auto& _funcs = RuntimeReflection::get_functions(_heap, m_typeHandle);

            Memory::Vector<IReflectedFunction> _res;

            _heap.Allocate(_res, _funcs.Size());

            for(uint32_t _i = 0; _i < _funcs.Size(); _i++)
            {
                _res.Allocate(IReflectedFunction(m_typeHandle, _funcs[_i]));
            }

            return _res;
        }

         template <typename Type, typename... Args, std::size_t Length>
        __function<void(Type::*)(Args...)>* GetFunctionCallbackM(const char (&_sName)[Length])
        {
            return get_function_callback_m<Type, Args...>(m_typeHandle, _sName);
        }

        template <typename ReturnType, typename Type, typename... Args, std::size_t Length>
        __function<ReturnType(Type::*)(Args...)>* GetFunctionCallbackMR(const char (&_sName)[Length])
        {
            return get_function_callback_mr<ReturnType, Type, Args...>(m_typeHandle, _sName);
        }

        template <typename ReturnType, typename Type, typename... Args>
        __function<ReturnType(Type::*)(Args...)>* GetFunctionCallbackMR(DUCKVIL_RESOURCE(function_t) _functionHandle)
        {
            return get_function_callback_mr<ReturnType, Type, Args...>(m_typeHandle, _functionHandle);
        }

        template <typename Type, typename... Args>
        __function<void(Type::*)(Args...)>* GetFunctionCallbackM(DUCKVIL_RESOURCE(function_t) _functionHandle)
        {
            return get_function_callback_m<Type, Args...>(m_typeHandle, _functionHandle);
        }

        template <typename... Args>
        void* Create(const Memory::FreeList& _heap, Args... _vArgs)
        {
            return create<Args...>(_heap, m_typeHandle, false, _vArgs...);
        }

        template <typename... Args>
        HotReloader::ITrackKeeper* CreateTracked(const Memory::FreeList& _heap, Args... _vArgs)
        {
            return static_cast<HotReloader::ITrackKeeper*>(create<Args...>(_heap, m_typeHandle, true, _vArgs...));
        }

        template <typename... Args, size_t Length>
        void InvokeM(const char (&_sName)[Length], void* _pObject, Args... _vArgs)
        {
            const auto& _funcHandle = RuntimeReflection::get_function_handle<Args...>(m_typeHandle, _sName);

            RuntimeReflection::invoke_member<Args...>(m_typeHandle, _funcHandle, _pObject, _vArgs...);
        }

        template <typename R, typename... Args, size_t Length>
        R InvokeMR(const char(&_sName)[Length], void* _pObject, Args... _vArgs)
        {
            const auto& _funcHandle = RuntimeReflection::get_function_handle<Args...>(m_typeHandle, _sName);

            return RuntimeReflection::invoke_member_result<R, Args...>(m_typeHandle, _funcHandle, _pObject, _vArgs...);
        }

        template <typename... Args, size_t Length>
        void InvokeS(const char(&_sName)[Length], Args... _vArgs)
        {
            const auto& _funcHandle = RuntimeReflection::get_function_handle<Args...>(m_typeHandle, _sName);

            RuntimeReflection::invoke_static<Args...>(m_typeHandle, _funcHandle, _vArgs...);
        }

        template <typename R, typename... Args, size_t Length>
        R InvokeSR(const char(&_sName)[Length], Args... _vArgs)
        {
            const auto& _funcHandle = RuntimeReflection::get_function_handle<Args...>(m_typeHandle, _sName);

            return RuntimeReflection::invoke_static_result<R, Args...>(m_typeHandle, _funcHandle, _vArgs...);
        }

        template <typename ReturnType, typename... Args, size_t Length>
        ReflectedFunction<ReturnType, Args...> GetFunction(const char (&_sName)[Length])
        {
            const auto& _handle = RuntimeReflection::get_function_handle<Args...>(m_typeHandle, _sName);

            return ReflectedFunction<ReturnType, Args...>(m_typeHandle, _handle);
        }

        template <typename ReturnType, typename... Args>
        ReflectedFunction<ReturnType, Args...> GetFunction(const RuntimeReflection::__duckvil_resource_function_t& _handle)
        {
            return ReflectedFunction<ReturnType, Args...>(m_typeHandle, _handle);
        }

        IReflectedFunction GetFunction(const RuntimeReflection::__duckvil_resource_function_t& _handle)
        {
            return IReflectedFunction(m_typeHandle, _handle);
        }

        Utils::string GetFullName() const
        {
            return combine(*m_type);
        }

        Memory::Vector<Utils::string> GetNamespaces() const
        {
            Memory::Vector<Utils::string> _namespaces;

            for(uint32_t _i = 0; _i < DUCKVIL_SLOT_ARRAY_SIZE(m_type->m_namespaces); ++_i)
            {
                const RuntimeReflection::__namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(m_type->m_namespaces, _i);

                _namespaces.Allocate(_namespace.m_sNamespaceName);
            }

            return _namespaces;
        }
    };

}}