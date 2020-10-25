#include "test/TestMemory/Module.h"

#include "UniTestFramework/UniTestFramework.h"

#include <cstring>

DUCKVIL_TEST(MemoryAlignmentCalculating)
{
    {
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000000, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000000, 2), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000000, 4), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000000, 8), (uint8_t)0, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000001, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000001, 2), (uint8_t)1, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000001, 4), (uint8_t)3, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000001, 8), (uint8_t)7, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000002, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000002, 2), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000002, 4), (uint8_t)2, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000002, 8), (uint8_t)6, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000003, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000003, 2), (uint8_t)1, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000003, 4), (uint8_t)1, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000003, 8), (uint8_t)5, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000004, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000004, 2), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000004, 4), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000004, 8), (uint8_t)4, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000005, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000005, 2), (uint8_t)1, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000005, 4), (uint8_t)3, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000005, 8), (uint8_t)3, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000006, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000006, 2), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000006, 4), (uint8_t)2, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000006, 8), (uint8_t)2, "Wrong padding");

        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000007, 1), (uint8_t)0, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000007, 2), (uint8_t)1, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000007, 4), (uint8_t)1, "Wrong padding");
        DUCKVIL_TEST_EQUAL(__duckvil_global::m_pMemoryInterface->m_fnCalculatePadding_((const void*)0x0000000000000007, 8), (uint8_t)1, "Wrong padding");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(MainMemoryAllocation)
{
    {
        Duckvil::Memory::__linear_allocator** _main = (Duckvil::Memory::__linear_allocator**)&__duckvil_global::m_pMemoryChunk;

        *_main = 0;

        DUCKVIL_TEST_EXP(__duckvil_global::m_pMemoryInterface->m_fnBasicAllocate(*_main, 1024) == false, "Should not happen");
    }

    Duckvil::Memory::__linear_allocator** _main = (Duckvil::Memory::__linear_allocator**)&__duckvil_global::m_pMemoryChunk;

    *_main = new Duckvil::Memory::__linear_allocator();

    DUCKVIL_TEST_EXP(__duckvil_global::m_pMemoryInterface->m_fnBasicAllocate(*_main, std::numeric_limits<std::size_t>::max()) == false, "Could not allocate memory");
    DUCKVIL_TEST_EXP(__duckvil_global::m_pMemoryInterface->m_fnBasicAllocate(*_main, 2048) == true, "Could not allocate memory");

    __duckvil_global::m_pHeap = __duckvil_global::m_pMemoryInterface->m_fnAllocateFreeListAllocator(*_main, 1024);

    DUCKVIL_TEST_SUCCESS_PASS;
}