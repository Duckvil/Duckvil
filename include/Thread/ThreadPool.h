#pragma once

#include "Utils/Macro.h"

#include "Memory/FreeList.h"
#include "Memory/Vector.h"
#include "Memory/Queue.h"

#include <mutex>
#include <future>
#include <type_traits>

#include "Event/ImmediateChannel.h"

#ifdef DUCKVIL_PLATFORM_LINUX
#include <pthread.h>
#else
#include <thread>
#endif

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace Thread {

    typedef void (*TaskCallback)(void*);

    struct task
    {
        TaskCallback m_callback;
        void* m_pData;
    };

#ifdef DUCKVIL_PLATFORM_LINUX
    struct pool_data
    {
        uint32_t m_uiThreadsCount;
        bool m_bRunning;
        bool m_bTerminate;
        pthread_mutex_t  m_lock;
        pthread_mutex_t  m_threadPoolLock;
        pthread_cond_t   m_condition;
        Memory::Queue<task> m_aTasks;
        Memory::Vector<pthread_t> m_aWorkers;
        Memory::FreeList m_heap;
        std::atomic<uint32_t> m_uiTaskCount;

    };
#else
    struct pool_data
    {
        Memory::Vector<std::thread*> m_aWorkers;
        Memory::Queue<task> m_aTasks;
        Memory::FreeList m_heap;
        uint32_t m_uiThreadsCount;
        TracyLockable(std::mutex, m_lock);
        TracyLockable(std::mutex, m_threadPoolLock);
        std::condition_variable_any m_condition;
        std::atomic<uint32_t> m_uiTaskCount;
        bool m_bRunning;
        bool m_bTerminate;
    };
#endif

    struct pool_ftable
    {
        pool_data* (*m_fnInit)(const Memory::FreeList& _heap);
        void (*m_fnStart)(pool_data*);
        void (*m_fnTerminate)(pool_data*);
        void (*m_fnOrderTask)(pool_data*, TaskCallback _task);
        void (*m_fnOrderDataTask)(pool_data*, TaskCallback _task, void* _pTaskData);
        bool (*m_fnRemainingTasks)(pool_data*);
        uint32_t (*m_fnGetTaskCount)(pool_data*);
    };

    template <typename F, typename... Args>
    auto order_task(pool_ftable* _pThreadPool, pool_data* _pThreadPoolData, F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        _pThreadPool->m_fnOrderTask(_pThreadPoolData, Utils::lambda([task](){ (*task)(); }));

        return res;
    }

}}

DUCKVIL_EXPORT Duckvil::Thread::pool_ftable* duckvil_thread_pool_init();