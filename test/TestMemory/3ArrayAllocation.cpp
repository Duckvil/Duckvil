#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Array.h"

#include <cstring>

DUCKVIL_TEST(ArrayAllocation)
{
    {
        Duckvil::Memory::Array<int> _array;

        __duckvil_global::m_freeList.Allocate(_array, 4);

        DUCKVIL_TEST_EXP(_array.Empty(), "Array is not empty");

        _array.Allocate(1);
        _array.Allocate(2);
        _array.Allocate(3);
        _array.Allocate(4);

        DUCKVIL_TEST_EQUAL(_array.Begin(), 1, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array.Back(), 4, "Wrong value");

        DUCKVIL_TEST_EXP(_array.Full(), "Array is not full");

        DUCKVIL_TEST_EQUAL(_array[0], 1, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array[1], 2, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array[2], 3, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array[3], 4, "Wrong value");

        Duckvil::Memory::Array<int>::Iterator it = _array.begin();

        DUCKVIL_TEST_EQUAL(*it++, 1, "Wrong value");
        DUCKVIL_TEST_EQUAL(*it++, 2, "Wrong value");
        DUCKVIL_TEST_EQUAL(*it++, 3, "Wrong value");
        DUCKVIL_TEST_EQUAL(*it, 4, "Wrong value");

        // Duckvil::Memory::fixed_array_clear(__duckvil_global::m_freeList.GetMemoryInterface(), _array.GetAllocator());

        /*_array.Clear();

        DUCKVIL_TEST_EXP(_array.Empty(), "Array is not empty");

        DUCKVIL_TEST_EQUAL(_array[0], 0, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array[1], 0, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array[2], 0, "Wrong value");
        DUCKVIL_TEST_EQUAL(_array[3], 0, "Wrong value");*/
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}