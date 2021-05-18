#include "UniTestFramework/UniTestFramework.h"

#include "PlugNPlay/Module.h"

#include "Thread/ThreadPool.h"

#include "State.h"

#ifdef DUCKVIL_PLATFORM_LINUX
#include <unistd.h>
#endif

Duckvil::PlugNPlay::__module duckvil_global::m_module;
Duckvil::PlugNPlay::__module_information duckvil_global::m_memoryModule = Duckvil::PlugNPlay::__module_information("Memory");
Duckvil::Memory::ftable* duckvil_global::m_pMemoryInterface = nullptr;
Duckvil::Memory::linear_allocator* duckvil_global::m_pMemoryChunk = nullptr;
Duckvil::Memory::free_list_allocator* duckvil_global::m_pHeap = nullptr;

DUCKVIL_TEST(Init)
{
    Duckvil::PlugNPlay::module_init(&duckvil_global::m_module);

    duckvil_global::m_module.load(&duckvil_global::m_memoryModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    duckvil_global::m_module.get(duckvil_global::m_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    duckvil_global::m_pMemoryInterface = duckvil_memory_init();

    duckvil_global::m_pMemoryInterface->m_fnBasicAllocate(&duckvil_global::m_pMemoryChunk, 32 * 1024);

    duckvil_global::m_pHeap = duckvil_global::m_pMemoryInterface->m_fnLinearAllocateFreeListAllocator(duckvil_global::m_pMemoryChunk, 16 * 1024);

    DUCKVIL_TEST_SUCCESS_PASS;
}

void some_expensive_task(void* _pData)
{
    std::atomic<uint32_t>* _counter = (std::atomic<uint32_t>*)_pData;
    uint32_t _index = 0;

    for(; _index < 1000000; ++_index)
    {
        _index++;
    }

    (*_counter) += _index;

#ifdef DUCKVIL_PLATFORM_LINUX
    sleep(1);
#else
    _sleep(1000);
#endif
}

DUCKVIL_TEST(ThreadPool)
{
    Duckvil::PlugNPlay::__module_information _memoryModule("Thread");

    duckvil_global::m_module.load(&_memoryModule);

    Duckvil::Thread::pool_ftable* (*_threadPoolinit)();

    duckvil_global::m_module.get(_memoryModule, "duckvil_thread_pool_init", (void**)&_threadPoolinit);

    Duckvil::Thread::pool_ftable* _thread = _threadPoolinit();
    Duckvil::Thread::pool_data* _threadData = _thread->m_fnInit({ duckvil_global::m_pMemoryInterface, duckvil_global::m_pHeap });

    _thread->m_fnStart(_threadData);

    std::atomic<uint32_t> _counter = 0;

    for(uint32_t i = 0; i < 10; ++i)
    {
        _thread->m_fnOrderDataTask(_threadData, &some_expensive_task, &_counter);
    }

    while(_thread->m_fnGetTaskCount(_threadData) > 0)
    {

    }

    DUCKVIL_TEST_EQUAL(_counter.load(), (uint32_t)10000000, "Wrong value");

    _thread->m_fnTerminate(_threadData);

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    duckvil_global::m_pMemoryInterface->m_fnFreeListClear_(duckvil_global::m_pHeap);
    duckvil_global::m_pMemoryInterface->m_fnLinearClear(duckvil_global::m_pMemoryChunk);
    duckvil_global::m_pMemoryInterface->m_fnBasicFree(&duckvil_global::m_pMemoryChunk);

    return DUCKVIL_TEST_FAILED;
}