#pragma once

#include "xbyak/xbyak.h"

#include "DependencyInjection/IDependencyInjector.h"

namespace Duckvil {

    // TODO: Move it to dependency injection module

    class RuntimeDependencyInjector : public Xbyak::CodeGenerator, public DependencyInjection::IDependencyInjector
    {
    private:
        uint32_t m_uiIndex = 0;
        bool m_aFloats[4];
        uint32_t m_uiCount;

    public:
        explicit RuntimeDependencyInjector(uint32_t _uiCount);
        ~RuntimeDependencyInjector() override;

        void Push(const void* _pData) override;
        void Push(const int& _data) override;
        void Push(const size_t& _data) override;
        void Push(const float& _data) override;
        void Push(const double& _data) override;
        void Push(const bool& _data) override;

        void Call(const void* func) override;
        void Call(const void** func) override;

        void* Execute() override;
    };

}