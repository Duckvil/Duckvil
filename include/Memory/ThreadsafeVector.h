#pragma once

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include <mutex>

namespace Duckvil { namespace Memory {

    template <typename Type>
    class ThreadsafeVector
    {
    private:
        Vector<Type> m_vector;
        mutable std::mutex m_mutex;

    public:
        ThreadsafeVector()
        {

        }

        ThreadsafeVector(const FreeList& _heap)
        {
            _heap.Allocate(m_vector, 1);
        }

        ThreadsafeVector(const ThreadsafeVector& _vector)
        {
            std::unique_lock<std::mutex> lock(_vector.m_mutex);

            m_vector = _vector.m_vector;
        }

        ThreadsafeVector(ThreadsafeVector&& _vector)
        {
            std::unique_lock<std::mutex> lock(_vector.m_mutex);

            m_vector = std::move(_vector.m_vector);
        }

        ~ThreadsafeVector()
        {

        }

        ThreadsafeVector& operator=(ThreadsafeVector&& _vector)
        {
            std::unique_lock<std::mutex> lock(_vector.m_mutex);

            if(&_vector == this)
            {
                return *this;
            }

            this->m_vector = std::move(_vector.m_vector);

            return *this;
        }

        ThreadsafeVector& operator=(const ThreadsafeVector& _vector)
        {
            std::unique_lock<std::mutex> lock(_vector.m_mutex);

            if(this == &_vector)
            {
                return *this;
            }

            m_vector = _vector.m_vector;

            return *this;
        }

        void* Allocate(const Type& _value)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Allocate(_value);
        }

        void* Allocate(Type&& _value)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Allocate(std::move(_value));
        }

        const Type& At(std::size_t _ullIndex) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.At(_ullIndex);
        }

        Type& At(std::size_t _ullIndex)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.At(_ullIndex);
        }

        std::size_t Size() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Size();
        }

        std::size_t Capacity() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Capacity();
        }

        const Type& operator[](std::size_t _ullIndex) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector[_ullIndex];
        }

        Type& operator[](std::size_t _ullIndex)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector[_ullIndex];
        }

        const Type& Begin() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Begin();
        }

        Type& Begin()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Begin();
        }

        const Type& Back() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Back();
        }

        Type& Back()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Back();
        }

        void Clear()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_vector.Clear();
        }

        bool Empty() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Empty();
        }

        bool Full() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.Full();
        }

        void Resize(std::size_t _ullNewSize)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_vector.Resize(_ullNewSize);
        }

        void Erase(uint32_t _uiIndex)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_vector.Erase(_uiIndex);
        }

        Vector<Type>::Iterator begin() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.begin();
        }

        Vector<Type>::Iterator end() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.end();
        }

        Vector<Type>::ConstIterator cbegin() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.cbegin();
        }

        Vector<Type>::ConstIterator cend() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_vector.cend();
        }
    };

}}