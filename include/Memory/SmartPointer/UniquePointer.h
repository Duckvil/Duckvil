#pragma once

#include "Memory/FreeList.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class UniquePointer
    {
    private:
        Type* m_p;
        FreeList m_heap;

    public:
        UniquePointer()
        {
            m_p = nullptr;
        }

        template <typename... Args>
        UniquePointer(const FreeList& _heap, const Args&... _vArgs) :
            m_heap(_heap)
        {
            m_p = _heap.Allocate<Type, Args...>(_vArgs...);
        }

        UniquePointer(const UniquePointer& _uniquePointer) = delete;

        UniquePointer(UniquePointer&& _uniquePointer) :
            m_p(std::move(_uniquePointer.m_p)),
            m_heap(std::move(_uniquePointer.m_heap))
        {
            _uniquePointer.m_p = nullptr;
        }

        ~UniquePointer()
        {
            if(m_p == nullptr)
            {
                return;
            }

            m_heap.Free(m_p);
        }

        UniquePointer& operator=(const UniquePointer& _uniquePointer) = delete;

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