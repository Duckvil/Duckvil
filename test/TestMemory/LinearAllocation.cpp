#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/LinearAllocator.h"

#include <cstring>

DUCKVIL_TEST(LinearAllocation)
{
    {
        Duckvil::Memory::__linear_allocator* _linear = __duckvil_global::m_pMemoryInterface->m_fnAllocateLinearAllocator((Duckvil::Memory::__allocator*)__duckvil_global::m_memoryChunk, 1024 * 1024 * 1024);

        DUCKVIL_TEST_IS_NULL(_linear, "Hmm, allocated too much memory");
    }

    {
        Duckvil::Memory::__linear_allocator* _linear = __duckvil_global::m_pMemoryInterface->m_fnAllocateLinearAllocator((Duckvil::Memory::__allocator*)__duckvil_global::m_memoryChunk, 16);

        DUCKVIL_TEST_EXP(_linear->used == 0, "Linear allocator is not empty");

        DUCKVIL_TEST_EQUAL(Duckvil::Memory::linear_allocate(__duckvil_global::m_pMemoryInterface, _linear, 1), 1, "Wrong value");
        DUCKVIL_TEST_EQUAL(Duckvil::Memory::linear_allocate(__duckvil_global::m_pMemoryInterface, _linear, 2), 2, "Wrong value");
        DUCKVIL_TEST_EQUAL(Duckvil::Memory::linear_allocate(__duckvil_global::m_pMemoryInterface, _linear, 3), 3, "Wrong value");
        DUCKVIL_TEST_EQUAL(Duckvil::Memory::linear_allocate(__duckvil_global::m_pMemoryInterface, _linear, 4), 4, "Wrong value");

        DUCKVIL_TEST_EXP(_linear->used == 16, "Should be full");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}