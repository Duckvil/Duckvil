#pragma once

#include "Memory/FreeList.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class SharedPointer
    {
    private:
        Type* m_p;
        FreeList m_heap;
        uint32_t* m_puiCount;

    public:
        SharedPointer()
        {
            m_p = nullptr;
        }

        template <typename... Args>
        SharedPointer(const FreeList& _heap, const Args&... _vArgs) :
            m_heap(_heap)
        {
            m_p = _heap.Allocate<Type, Args...>(_vArgs...);
            m_puiCount =_heap.Allocate<uint32_t>(0);

            (*m_puiCount)++;
        }

        SharedPointer(const SharedPointer& _sharedPointer) :
            m_p(_sharedPointer.m_p),
            m_puiCount(_sharedPointer.m_puiCount),
            m_heap(_sharedPointer.m_heap)
        {
            (*m_puiCount)++;
        }

        SharedPointer(SharedPointer&& _sharedPointer) :
            m_p(std::move(_sharedPointer.m_p)),
            m_heap(std::move(_sharedPointer.m_heap)),
            m_puiCount(std::move(_sharedPointer.m_puiCount))
        {
            _sharedPointer.m_p = nullptr;
            _sharedPointer.m_puiCount = nullptr;
        }

        ~SharedPointer()
        {
            (*m_puiCount)--;

            if(m_p == nullptr || *m_puiCount > 0)
            {
                return;
            }

            m_heap.Free(m_p);
        }

        SharedPointer& operator=(const SharedPointer& _sharedPointer)
        {
            m_p = _sharedPointer.m_p;
            m_heap = _sharedPointer.m_heap;
            m_puiCount = _sharedPointer.m_puiCount;

            (*m_puiCount)++;
        }

        Type& operator*()
        {
            return *m_p;
        }

        Type* operator->()
        {
            return m_p;
        }

        operator Type*()
        {
            return m_p;
        }
    };

}}