#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/FreeListAllocator.h"

#include <cstring>

struct _test
{
    int test1;
    int test2;
    int test3;
    int test4;
    int test5;
};

DUCKVIL_TEST(FreeListAllocation)
{
    Duckvil::Memory::__free_list_allocator* _free = __duckvil_global::m_pMemoryInterface->m_fnAllocateFreeListAllocator((Duckvil::Memory::__linear_allocator*)__duckvil_global::m_pMemoryChunk, 256);

    _test _t;

    _test* _d = Duckvil::Memory::free_list_allocate(__duckvil_global::m_pMemoryInterface, _free, _t);

    DUCKVIL_TEST_IS_NOT_NULL(_d, "Failed to allocate memory");

    _d->test1 = 10;
    _d->test5 = 20;

    DUCKVIL_TEST_EQUAL(_d->test1, 10, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d->test5, 20, "Wrong value");

    _test* _d2 = Duckvil::Memory::free_list_allocate(__duckvil_global::m_pMemoryInterface, _free, _t);

    DUCKVIL_TEST_IS_NOT_NULL(_d2, "Failed to allocate memory");

    _d2->test1 = 11;
    _d2->test5 = 22;

    DUCKVIL_TEST_EQUAL(_d2->test1, 11, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d2->test5, 22, "Wrong value");

    _test* _d3 = Duckvil::Memory::free_list_allocate(__duckvil_global::m_pMemoryInterface, _free, _t);

    DUCKVIL_TEST_IS_NOT_NULL(_d3, "Failed to allocate memory");

    _d3->test1 = 111;
    _d3->test5 = 222;

    DUCKVIL_TEST_EQUAL(_d3->test1, 111, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d3->test5, 222, "Wrong value");

    Duckvil::Memory::free_list_free(__duckvil_global::m_pMemoryInterface, _free, _d2);

// TODO: Relates to FreeListAllocator:23
    // DUCKVIL_TEST_IS_NULL(_d2, "Failed to free memory");
    DUCKVIL_TEST_EQUAL(_d->test1, 10, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d->test5, 20, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d3->test1, 111, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d3->test5, 222, "Wrong value");

    Duckvil::Memory::free_list_free(__duckvil_global::m_pMemoryInterface, _free, _d3);

    // DUCKVIL_TEST_IS_NULL(_d3, "Failed to free memory");
    DUCKVIL_TEST_EQUAL(_d->test1, 10, "Wrong value");
    DUCKVIL_TEST_EQUAL(_d->test5, 20, "Wrong value");

    Duckvil::Memory::free_list_free(__duckvil_global::m_pMemoryInterface, _free, _d);

    // DUCKVIL_TEST_IS_NULL(_d, "Failed to free memory");

    DUCKVIL_TEST_SUCCESS_PASS;
}