#pragma once

namespace Duckvil { namespace DependencyInjection {

    class IDependencyInjector
    {
    public:
        virtual ~IDependencyInjector()
        {

        }

        virtual void Push(const void* _pData) = 0;
        virtual void Push(const int& _data) = 0;
        virtual void Push(const size_t& _data) = 0;
        virtual void Push(const float& _data) = 0;
        virtual void Push(const double& _data) = 0;
        virtual void Push(const bool& _data) = 0;

        virtual void Call(const void* func) = 0;
        virtual void Call(const void** func) = 0;

        virtual void Execute() = 0;

        template <typename Type>
        void Push(const Type& _data)
        {
            Push(static_cast<const void*>(&_data));
        }

        template <typename Type>
        void Push(Type* _data)
        {
            Push(static_cast<const void*>(_data));
        }

        template <typename R, typename C, typename... Args>
        void Call(R(C::* _pFunc)(Args...))
        {
            Call((void*&)_pFunc);
        }
    };

}}