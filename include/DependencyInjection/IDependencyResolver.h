#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#include "DependencyInjection/IDependencyInjector.h"
#include "DependencyInjection/Scope.h"

namespace Duckvil { namespace DependencyInjection {

    class IDependencyResolver
    {
    public:
        struct Dependency
        {
            typedef bool (*OnInjectCallback)(const RuntimeReflection::__duckvil_resource_type_t&, const RuntimeReflection::__duckvil_resource_constructor_t&, const RuntimeReflection::__duckvil_resource_argument_t&);
            typedef void* (*DependencyFactory)(const RuntimeReflection::__duckvil_resource_type_t&, const RuntimeReflection::__duckvil_resource_constructor_t&, const RuntimeReflection::__duckvil_resource_argument_t&);

            size_t m_ullTypeID = 0;
            Scope m_scope;
            const void* m_pData = nullptr;
            OnInjectCallback m_fnOnInject = nullptr;
            DependencyFactory m_fnFactory = nullptr;
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
        virtual bool Resolve(const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle, void** _ppResolvedObject = nullptr) = 0;

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

        template <typename Type>
        bool Register(Dependency::DependencyFactory _fnFactory, Scope _scope = Scope::SINGLETON)
        {
            Dependency _dependency{ .m_scope = _scope, .m_fnFactory = _fnFactory };

            _dependency.m_ullTypeID = typeid(Type).hash_code();

            return this->Register(_dependency);
        }
    };

}}