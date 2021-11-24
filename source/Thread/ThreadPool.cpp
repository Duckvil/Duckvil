#include "Thread/ThreadPool.h"

#include "tracy/common/TracySystem.hpp"

namespace Duckvil { namespace Thread {

#ifdef DUCKVIL_PLATFORM_LINUX
    void* pool_worker(void* _pData)
    {
        pool_data* _data = (pool_data*)_pData;

        while(!_data->m_bTerminate)
        {
            task _task;

            pthread_mutex_lock(&(_data->m_lock));

            while(_data->m_aTasks.Empty() && !_data->m_bTerminate)
            {
                pthread_cond_wait(&(_data->m_condition), &(_data->m_lock));
            }

            if(_data->m_bTerminate)
            {
                continue;
            }

            _task = _data->m_aTasks.Begin();

            _data->m_aTasks.Pop();

            pthread_mutex_unlock(&(_data->m_lock));

            try
            {
                _task.m_callback(_task.m_pData);

                _data->m_uiTaskCount--;
            }
            catch(const std::exception& _e)
            {
                printf("%s\n", _e.what());
            }
        }

        return nullptr;
    }
#else
    void pool_worker(pool_data* _pData, const char* _sName)
    {
#ifdef TRACY_ENABLE
        tracy::SetThreadName(_sName);
#endif

        while(!_pData->m_bTerminate)
        {
            task _task;

            {
                std::unique_lock<LockableBase(std::mutex)> _lock(_pData->m_lock);
                LockableBase(std::mutex)& l = *_lock.mutex();
                LockMark(l);

                _pData->m_condition.wait(_lock, [_pData]()
                {
                    return !_pData->m_aTasks.Empty() || _pData->m_bTerminate;
                });

                if(_pData->m_bTerminate)
                {
                    continue;
                }

                _task = _pData->m_aTasks.Begin();

                _pData->m_aTasks.Pop();
            }

            try
            {
                _task.m_callback(_task.m_pData);

                _pData->m_uiTaskCount--;
            }
            catch(std::exception& _e)
            {
                printf("%s\n", _e.what());
            }
        }
    }
#endif

    pool_data* impl_pool_init(const Memory::FreeList& _heap)
    {
        pool_data* _res = _heap.Allocate<pool_data>();

        _res->m_heap = _heap;
        _res->m_uiThreadsCount = std::thread::hardware_concurrency() - 1;
        _heap.Allocate(_res->m_aWorkers, _res->m_uiThreadsCount);
        _heap.Allocate(_res->m_aTasks, 50);

        _res->m_bRunning = false;
        _res->m_bTerminate = false;

#ifdef DUCKVIL_PLATFORM_LINUX
        _res->m_lock =              PTHREAD_MUTEX_INITIALIZER;
        _res->m_threadPoolLock =    PTHREAD_MUTEX_INITIALIZER;
        _res->m_condition =         PTHREAD_COND_INITIALIZER;
#endif

        return _res;
    }

    void impl_pool_start(pool_data* _pData)
    {
        if(_pData->m_bRunning)
        {
            return;
        }

        _pData->m_bRunning = true;

        for(uint32_t i = 0; i < _pData->m_uiThreadsCount; ++i)
        {
#ifdef DUCKVIL_PLATFORM_LINUX
            pthread_t _thread;

            pthread_create(&_thread, nullptr, pool_worker, _pData);
#else
            std::string _threadName = ("t" + std::to_string(i));
            char* _s = _pData->m_heap.AllocateArray<char>(_threadName.size() + 1);
            memset(_s, 0, _threadName.size() + 1);
            memcpy(_s, _threadName.c_str(), _threadName.size());
            std::thread* _thread = _pData->m_heap.Allocate<std::thread>(&pool_worker, _pData, _s);
#endif

            _pData->m_aWorkers.Allocate(_thread);
        }
    }

    void impl_pool_terminate(pool_data* _pData)
    {
        if(!_pData->m_bRunning)
        {
            return;
        }

#ifdef DUCKVIL_PLATFORM_LINUX
        pthread_mutex_lock(&(_pData->m_threadPoolLock));
#else
        std::unique_lock<LockableBase(std::mutex)> _lock(_pData->m_threadPoolLock);
        LockableBase(std::mutex)& l = *_lock.mutex();
        LockMark(l);
#endif

        _pData->m_bTerminate = true;

#ifdef DUCKVIL_PLATFORM_LINUX
        pthread_cond_broadcast(&(_pData->m_condition));
#else
        _pData->m_condition.notify_all();

        for(std::thread* _worker : _pData->m_aWorkers)
        {
            _worker->join();

            _pData->m_heap.Free(_worker);
        }
#endif

        _pData->m_aWorkers.Clear();

        _pData->m_bRunning = false;

#ifdef DUCKVIL_PLATFORM_LINUX
        pthread_mutex_unlock(&(_pData->m_threadPoolLock));
#endif
    }

    void impl_pool_order_task(pool_data* _pData, TaskCallback _task)
    {
        if(!_pData->m_bRunning)
        {
            return;
        }

#ifdef DUCKVIL_PLATFORM_LINUX
        pthread_mutex_lock(&(_pData->m_lock));

        _pData->m_aTasks.Allocate({ _task });

        _pData->m_uiTaskCount++;

        pthread_mutex_unlock(&(_pData->m_lock));
        pthread_cond_signal(&(_pData->m_condition));
#else
        {
            std::unique_lock<LockableBase(std::mutex)> _lock(_pData->m_lock);
            LockableBase(std::mutex)& l = *_lock.mutex();
            LockMark(l);

            _pData->m_aTasks.Allocate({ _task });

            _pData->m_uiTaskCount++;
        }

        _pData->m_condition.notify_one();
#endif
    }

    void impl_pool_order_data_task(pool_data* _pData, TaskCallback _task, void* _pTaskData)
    {
        if(!_pData->m_bRunning)
        {
            return;
        }

#ifdef DUCKVIL_PLATFORM_LINUX
        pthread_mutex_lock(&(_pData->m_lock));

        _pData->m_aTasks.Allocate({ _task, _pTaskData });

        _pData->m_uiTaskCount++;

        pthread_mutex_unlock(&(_pData->m_lock));
        pthread_cond_signal(&(_pData->m_condition));
#else
        {
            std::unique_lock<LockableBase(std::mutex)> _lock(_pData->m_lock);
            LockableBase(std::mutex)& l = *_lock.mutex();
            LockMark(l);
            
            _pData->m_aTasks.Allocate({ _task, _pTaskData });

            _pData->m_uiTaskCount++;
        }

        _pData->m_condition.notify_one();
#endif
    }

    bool impl_pool_remaining_tasks(pool_data* _pData)
    {
        return !_pData->m_aTasks.Empty();
    }

    uint32_t impl_pool_get_task_count(pool_data* _pData)
    {
        // std::unique_lock<std::mutex> lock(_pData->m_lock);

        return _pData->m_uiTaskCount.load();
    }

}}

Duckvil::Thread::pool_ftable* duckvil_thread_pool_init()
{
    static Duckvil::Thread::pool_ftable _ftable = { 0 };

    _ftable.m_fnInit =              &Duckvil::Thread::impl_pool_init;
    _ftable.m_fnStart =             &Duckvil::Thread::impl_pool_start;
    _ftable.m_fnTerminate =         &Duckvil::Thread::impl_pool_terminate;
    _ftable.m_fnOrderTask =         &Duckvil::Thread::impl_pool_order_task;
    _ftable.m_fnOrderDataTask =     &Duckvil::Thread::impl_pool_order_data_task;
    _ftable.m_fnRemainingTasks =    &Duckvil::Thread::impl_pool_remaining_tasks;
    _ftable.m_fnGetTaskCount =      &Duckvil::Thread::impl_pool_get_task_count;

    return &_ftable;
}