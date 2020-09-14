#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/FixedArrayAllocator.h"

#include <cstring>

DUCKVIL_TEST(ArrayAllocation)
{
    {
        Duckvil::Memory::Array<int> _array(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::__allocator*)__duckvil_global::m_memoryChunk, 4);

        _array.Allocate(1);
        _array.Allocate(2);
        _array.Allocate(3);
        _array.Allocate(4);

        DUCKVIL_TEST_EQUAL(_array[0], 1, "Wrong value");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}