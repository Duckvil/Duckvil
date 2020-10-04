#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Vector.h"

#include <cstring>

DUCKVIL_TEST(VectorAllocation)
{
    {
        Duckvil::Memory::__free_list_allocator* _free_list = __duckvil_global::m_pMemoryInterface->m_fnAllocateFreeListAllocator((Duckvil::Memory::__allocator*)__duckvil_global::m_pMemoryChunk, 256);

        DUCKVIL_TEST_IS_NOT_NULL(_free_list, "Free list allocator allocation failed");

        Duckvil::Memory::Vector<float> _vec(__duckvil_global::m_pMemoryInterface, _free_list, 2);

        DUCKVIL_TEST_EXP(_vec.Empty(), "Vector is not empty");

        _vec.Allocate(10.f);
        _vec.Allocate(20.f);

        DUCKVIL_TEST_EXP(_vec.Full(), "Vector is not full");

        DUCKVIL_TEST_EQUAL(_vec.At(0), 10.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(1), 20.f, "Wrong value");

        _vec.Resize(_free_list, 16);

        DUCKVIL_TEST_EXP(!_vec.Full(), "Vector is full");

        _vec.Allocate(30.f);
        _vec.Allocate(40.f);

        DUCKVIL_TEST_EXP(_vec.Full(), "Vector is not full");

        DUCKVIL_TEST_EQUAL(_vec.At(2), 30.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(3), 40.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(0), 10.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(1), 20.f, "Wrong value");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}