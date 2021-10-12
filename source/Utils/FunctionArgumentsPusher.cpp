#include "Utils/FunctionArgumentsPusher.h"

namespace Duckvil {

    FunctionArgumentsPusher::FunctionArgumentsPusher(uint32_t _uiCount) :
        m_uiCount(_uiCount)
    {
        if(_uiCount > 4)
        {
            sub(rsp, 24 + (8 * (_uiCount - 4)));
        }

        push(r10);
    }

    FunctionArgumentsPusher::~FunctionArgumentsPusher()
    {

    }

    void FunctionArgumentsPusher::Push(const int& _data)
    {
        if(m_uiIndex < 4)
        {
            m_aFloats[m_uiIndex] = false;
        }

        m_uiIndex++;

        if(m_uiIndex == 1)
        {
            push(rcx);
            mov(rcx, _data);
        }
        else if(m_uiIndex == 2)
        {
            push(rdx);
            mov(rdx, _data);
        }
        else if(m_uiIndex == 3)
        {
            push(r8);
            mov(r8, _data);
        }
        else if(m_uiIndex == 4)
        {
            push(r9);
            mov(r9, _data);
        }
        else
        {
            mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], _data);
        }
    }

    void FunctionArgumentsPusher::Push(const size_t& _data)
    {
        if(m_uiIndex < 4)
        {
            m_aFloats[m_uiIndex] = false;
        }

        m_uiIndex++;

        if(m_uiIndex == 1)
        {
            push(rcx);
            mov(rcx, _data);
        }
        else if(m_uiIndex == 2)
        {
            push(rdx);
            mov(rdx, _data);
        }
        else if(m_uiIndex == 3)
        {
            push(r8);
            mov(r8, _data);
        }
        else if(m_uiIndex == 4)
        {
            push(r9);
            mov(r9, _data);
        }
        else
        {
            mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], _data);
        }
    }

    void FunctionArgumentsPusher::Push(const float& _data)
    {
        if(m_uiIndex < 4)
        {
            m_aFloats[m_uiIndex] = true;
        }

        m_uiIndex++;

        if(m_uiIndex == 1)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm0, ptr[r10]);
        }
        else if(m_uiIndex == 2)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm1, ptr[r10]);
        }
        else if(m_uiIndex == 3)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm2, ptr[r10]);
        }
        else if(m_uiIndex == 4)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm3, ptr[r10]);
        }
        else
        {
            mov(r10, size_t(&_data));
            mov(r10, ptr[r10]);
            mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], r10);
        }
    }

    void FunctionArgumentsPusher::Push(const double& _data)
    {
        if(m_uiIndex < 4)
        {
            m_aFloats[m_uiIndex] = true;
        }

        m_uiIndex++;

        if(m_uiIndex == 1)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm0, ptr[r10]);
        }
        else if(m_uiIndex == 2)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm1, ptr[r10]);
        }
        else if(m_uiIndex == 3)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm2, ptr[r10]);
        }
        else if(m_uiIndex == 4)
        {
            mov(r10, size_t(&_data));
            movdqu(xmm3, ptr[r10]);
        }
        else
        {
            mov(r10, size_t(&_data));
            mov(r10, ptr[r10]);
            mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], r10);
        }
    }

    void FunctionArgumentsPusher::Push(const bool& _data)
    {
        if(m_uiIndex < 4)
        {
            m_aFloats[m_uiIndex] = false;
        }

        m_uiIndex++;

        if(m_uiIndex == 1)
        {
            push(rcx);
            mov(rcx, _data);
        }
        else if(m_uiIndex == 2)
        {
            push(rdx);
            mov(rdx, _data);
        }
        else if(m_uiIndex == 3)
        {
            push(r8);
            mov(r8, _data);
        }
        else if(m_uiIndex == 4)
        {
            push(r9);
            mov(r9, _data);
        }
        else
        {
            mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], _data);
        }
    }

    void FunctionArgumentsPusher::Call(void* func)
    {
        push(rbx);
        mov(rbx, size_t(func));
        call(rbx);
        pop(rbx);

        uint32_t _index = m_uiIndex > 4 ? 4 : m_uiIndex;

        if(m_uiIndex >= 4)
        {
            _index--;

            if(m_aFloats[_index] == false)
            {
                pop(r9);
            }
        }

        if(m_uiIndex >= 3)
        {
            _index--;

            if(m_aFloats[_index] == false)
            {
                pop(r8);
            }
        }

        if(m_uiIndex >= 2)
        {
            _index--;

            if(m_aFloats[_index] == false)
            {
                pop(rdx);
            }
        }

        if(m_uiIndex >= 1)
        {
            _index--;

            if(m_aFloats[_index] == false)
            {
                pop(rcx);
            }
        }

        if(m_uiCount > 4)
        {
            add(rsp, 24 + (8 * (m_uiCount - 4)));
        }

        pop(r10);

        ret();
    }

}