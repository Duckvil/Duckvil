#pragma once

#include "Memory/Queue.h"
#include "Memory/FreeList.h"

#include <mutex>

namespace Duckvil { namespace Memory {

    template <typename Type>
    class ThreadsafeQueue
    {
    private:
        Queue<Type> m_queue;
        mutable std::mutex m_mutex;

    public:
        ThreadsafeQueue()
        {

        }

        ThreadsafeQueue(const FreeList& _heap)
        {
            _heap.Allocate(m_queue, 1);
        }

        ThreadsafeQueue(const ThreadsafeQueue& _queue)
        {
            std::unique_lock<std::mutex> lock(_queue.m_mutex);

            m_queue = _queue.m_queue;
        }

        ThreadsafeQueue(ThreadsafeQueue&& _queue)
        {
            std::unique_lock<std::mutex> lock(_queue.m_mutex);

            m_queue = std::move(_queue.m_queue);
        }

        ~ThreadsafeQueue()
        {

        }

        ThreadsafeQueue& operator=(ThreadsafeQueue&& _queue)
        {
            if(&_queue == this)
            {
                return *this;
            }

            std::unique_lock<std::mutex> lock(_queue.m_mutex);

            this->m_queue = std::move(_queue.m_queue);

            return *this;
        }

        ThreadsafeQueue& operator=(const ThreadsafeQueue& _queue)
        {
            if(this == &_queue)
            {
                return *this;
            }

            std::unique_lock<std::mutex> lock(_queue.m_mutex);

            m_queue = _queue.m_queue;

            return *this;
        }

        inline Type* Allocate(const Type& _data)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.Allocate(_data);
        }

        inline Type* Allocate(Type&& _data)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.Allocate(std::move(_data));
        }

        inline const Type& Begin() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.Begin();
        }

        inline Type& Begin()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.Begin();
        }

        inline bool Empty() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.Empty();
        }

        inline bool Full() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.Full();
        }

        inline void Pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_queue.Pop();
        }

        void Resize(std::size_t _ullNewSize)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_queue.Resize(_ullNewSize);
        }

        inline std::size_t GetSize() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.GetSize();
        }

        inline std::size_t GetCapacity() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_queue.GetCapacity();
        }
    };

}}