#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/FreeListAllocator.h"

#include <cstring>

struct _test
{
    std::size_t test1;
    std::size_t test2;
    std::size_t test3;
    std::size_t test4;
    std::size_t test5;
};

DUCKVIL_TEST(FreeListAllocation)
{
    // TODO: Tests
    Duckvil::Memory::__free_list_allocator* _free = __duckvil_global::m_pMemoryInterface->m_fnAllocateFreeListAllocator((Duckvil::Memory::__allocator*)__duckvil_global::m_memoryChunk, 256);

    _test _t;

    _test* _d = Duckvil::Memory::free_list_allocate(__duckvil_global::m_pMemoryInterface, _free, _t);

    _d->test1 = 10;
    _d->test5 = 20;

    _test* _d2 = Duckvil::Memory::free_list_allocate(__duckvil_global::m_pMemoryInterface, _free, _t);

    _d2->test1 = 11;
    _d2->test5 = 22;

    _test* _d3 = Duckvil::Memory::free_list_allocate(__duckvil_global::m_pMemoryInterface, _free, _t);

    _d3->test1 = 111;
    _d3->test5 = 222;

    Duckvil::Memory::free_list_free(__duckvil_global::m_pMemoryInterface, _free, _d2);
    Duckvil::Memory::free_list_free(__duckvil_global::m_pMemoryInterface, _free, _d3);
    Duckvil::Memory::free_list_free(__duckvil_global::m_pMemoryInterface, _free, _d);

    DUCKVIL_TEST_SUCCESS_PASS;
}