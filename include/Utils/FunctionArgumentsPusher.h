#pragma once

#include "xbyak/xbyak.h"

namespace Duckvil {

    struct FunctionArgumentsPusher : Xbyak::CodeGenerator
    {
        uint32_t m_uiIndex = 0;
        bool m_aFloats[4];
        uint32_t m_uiCount;

        explicit FunctionArgumentsPusher(uint32_t _uiCount);
        ~FunctionArgumentsPusher();

        void Push(const int& _data);
        void Push(const size_t& _data);
        void Push(const float& _data);
        void Push(const double& _data);
        void Push(const bool& _data);

        template <typename Type>
        void Push(const Type& _data)
        {
            if(m_uiIndex < 4)
            {
                m_aFloats[m_uiIndex] = false;
            }

            m_uiIndex++;

            if(m_uiIndex == 1)
            {
                push(rcx);
                mov(rcx, (size_t)&_data);
            }
            else if(m_uiIndex == 2)
            {
                push(rdx);
                mov(rdx, (size_t)&_data);
            }
            else if(m_uiIndex == 3)
            {
                push(r8);
                mov(r8, (size_t)&_data);
            }
            else if(m_uiIndex == 4)
            {
                push(r9);
                mov(r9, (size_t)&_data);
            }
            else
            {
                mov(r10, size_t(&_data));
                mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], r10);
            }
        }

        template <typename Type>
        void Push(Type* _data)
        {
            if(m_uiIndex < 4)
            {
                m_aFloats[m_uiIndex] = false;
            }

            m_uiIndex++;

            if(m_uiIndex == 1)
            {
                push(rcx);
                mov(rcx, (size_t)_data);
            }
            else if(m_uiIndex == 2)
            {
                push(rdx);
                mov(rdx, (size_t)_data);
            }
            else if(m_uiIndex == 3)
            {
                push(r8);
                mov(r8, (size_t)_data);
            }
            else if(m_uiIndex == 4)
            {
                push(r9);
                mov(r9, (size_t)_data);
            }
            else
            {
                mov(r10, size_t(_data));
                mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], r10);
            }
        }

        void Call(const void* func);
        void Call(const void** func);

        template <typename R, typename C, typename... Args>
        void Call(R (C::*_pFunc)(Args...))
        {
            Call((void*&)_pFunc);
        }
    };

}