#include "Utils/RuntimeDependencyInjector.h"

namespace Duckvil {

    RuntimeDependencyInjector::RuntimeDependencyInjector(uint32_t _uiCount) :
        m_uiCount(_uiCount)
    {
        push(rbp);
        mov(rbp, rsp);

        if(_uiCount > 4)
        {
            sub(rsp, 24 + (8 * (_uiCount - 4)));
        }

        and_(rsp, -16);

        push(r10);
    }

    RuntimeDependencyInjector::~RuntimeDependencyInjector()
    {
        
    }

    void RuntimeDependencyInjector::Push(const void* _pData)
    {
	    if(m_uiIndex < 4)
        {
            m_aFloats[m_uiIndex] = false;
        }

        m_uiIndex++;

        if(m_uiIndex == 1)
        {
            push(rcx);
            mov(rcx, (size_t)_pData);
        }
        else if(m_uiIndex == 2)
        {
            push(rdx);
            mov(rdx, (size_t)_pData);
        }
        else if(m_uiIndex == 3)
        {
            push(r8);
            mov(r8, (size_t)_pData);
        }
        else if(m_uiIndex == 4)
        {
            push(r9);
            mov(r9, (size_t)_pData);
        }
        else
        {
            mov(r10, size_t(_pData));
            mov(qword[rsp + 24 + (8 * (m_uiIndex - 5))], r10);
        }
    }

    void RuntimeDependencyInjector::Push(const int& _data)
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

    void RuntimeDependencyInjector::Push(const size_t& _data)
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

    void RuntimeDependencyInjector::Push(const float& _data)
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

    void RuntimeDependencyInjector::Push(const double& _data)
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

    void RuntimeDependencyInjector::Push(const bool& _data)
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

    void RuntimeDependencyInjector::Call(const void* func)
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

        pop(r10);

        if(m_uiCount > 4)
        {
            add(rsp, 24 + (8 * (m_uiCount - 4)));
        }

        mov(rsp, rbp);
        pop(rbp);

        ret();
    }

    void RuntimeDependencyInjector::Call(const void** func)
    {
        push(rbx);
        push(r10);
        mov(r10, size_t(func));
        mov(rbx, ptr [r10]);
        call(rbx);
        pop(r10);
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

        pop(r10);

        if(m_uiCount > 4)
        {
            add(rsp, 24 + (8 * (m_uiCount - 4)));
        }

        mov(rsp, rbp);
        pop(rbp);

        ret();
    }

    void* RuntimeDependencyInjector::Execute()
    {
    	return getCode<void* (*)()>()();
    }

}