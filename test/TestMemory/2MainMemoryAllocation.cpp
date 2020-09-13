#include "test/TestMemory/Module.h"

#include "UniTestFramework/UniTestFramework.h"

#include <cstring>

DUCKVIL_TEST(MainMemoryAllocation)
{
    Duckvil::Memory::__linear_allocator** _main = (Duckvil::Memory::__linear_allocator**)&__duckvil_global::m_memoryChunk;

    *_main = new Duckvil::Memory::__linear_allocator();

    __duckvil_global::m_pMemoryInterface->m_fnBasicAllocate(*_main, 1024);

    DUCKVIL_TEST_SUCCESS_PASS;
}