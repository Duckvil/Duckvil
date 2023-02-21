#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#include "DependencyInjection/IDependencyInjector.h"

namespace Duckvil { namespace DependencyInjection {

    class IDependencyResolver
    {
    public:
        enum class Scope
        {
            SINGLETON,
            SCOPED,
            TRANSIENT
        };

        struct Dependency
        {
            typedef bool (*OnInjectCallback)(const RuntimeReflection::__duckvil_resource_type_t&, const RuntimeReflection::__duckvil_resource_constructor_t&, const RuntimeReflection::__duckvil_resource_argument_t&);

            size_t m_ullTypeID;
            Scope m_scope;
            const void* m_pData;
            OnInjectCallback m_fnOnInject;
        };

        class DependencyNotFound : public std::exception
        {
        private:

        public:
            DependencyNotFound() = default;
            ~DependencyNotFound() override = default;
        };

    private:

    public:
        virtual ~IDependencyResolver()
        {

        }

        virtual bool Register(const Dependency& _dependency) = 0;

        // TODO: Rename to Resolve
        virtual bool Resolve(const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle) = 0;

        template <typename Type>
        bool Register(const Type& _pData, Dependency::OnInjectCallback _fnOnInject = nullptr, Scope _scope = Scope::SINGLETON)
        {
            Dependency _dependency{ .m_scope = _scope, .m_fnOnInject = _fnOnInject };

            _dependency.m_ullTypeID = typeid(Type).hash_code();

            if(_scope == Scope::SINGLETON)
            {
                _dependency.m_pData = &_pData;
            }

            return this->Register(_dependency);
        }

        template <typename Type>
        bool Register(Type* _pData, Dependency::OnInjectCallback _fnOnInject = nullptr, Scope _scope = Scope::SINGLETON)
        {
            Dependency _dependency{ .m_scope = _scope, .m_fnOnInject = _fnOnInject };

            _dependency.m_ullTypeID = typeid(Type).hash_code();

            if(_scope == Scope::SINGLETON)
            {
                _dependency.m_pData = _pData;
            }

            return this->Register(_dependency);
        }
    };

}}