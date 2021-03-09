#include "Thread/ThreadPool.h"

namespace Duckvil { namespace Thread {

    void pool_worker(pool_data* _pData)
    {
        while(!_pData->m_bTerminate)
        {
            TaskCallback _task;

            {
                std::unique_lock<std::mutex> _lock(_pData->m_lock);

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

            _task();
        }

        printf("ENDED!\n");
    }

    pool_data* impl_pool_init(const Memory::FreeList& _heap)
    {
        pool_data* _res = _heap.Allocate<pool_data>();

        _res->m_heap = _heap;
        _res->m_uiThreadsCount = std::thread::hardware_concurrency() - 1;
        _heap.Allocate(_res->m_aWorkers, _res->m_uiThreadsCount);
        _heap.Allocate(_res->m_aTasks, 10);

        _res->m_bRunning = false;
        _res->m_bTerminate = false;

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
            std::thread* _thread = _pData->m_heap.Allocate<std::thread>(pool_worker, _pData);

            _pData->m_aWorkers.Allocate(_thread);
        }
    }

    void impl_pool_terminate(pool_data* _pData)
    {
        if(!_pData->m_bRunning)
        {
            return;
        }

        std::unique_lock<std::mutex> lock(_pData->m_threadPoolLock);

        _pData->m_bTerminate = true;

        _pData->m_condition.notify_all();

        for(std::thread* _worker : _pData->m_aWorkers)
        {
            _worker->join();

            _pData->m_heap.Free(_worker);
        }

        _pData->m_aWorkers.Clear();

        _pData->m_bRunning = false;
    }

    void impl_pool_order_task(pool_data* _pData, TaskCallback _task)
    {
        if(!_pData->m_bRunning)
        {
            return;
        }

        {
            std::unique_lock<std::mutex> lock(_pData->m_lock);
            
            _pData->m_aTasks.Allocate(_task);
        }

        _pData->m_condition.notify_one();
    }

}}

Duckvil::Thread::pool_ftable* duckvil_thread_pool_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Thread::pool_ftable* _ftable = (Duckvil::Thread::pool_ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Thread::pool_ftable), alignof(Duckvil::Thread::pool_ftable));

    _ftable->m_fnInit = &Duckvil::Thread::impl_pool_init;
    _ftable->m_fnStart = &Duckvil::Thread::impl_pool_start;
    _ftable->m_fnTerminate = &Duckvil::Thread::impl_pool_terminate;
    _ftable->m_fnOrderTask = &Duckvil::Thread::impl_pool_order_task;

    return _ftable;
}