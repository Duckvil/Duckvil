#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Vector.h"

#include <cstring>

DUCKVIL_TEST(VectorAllocation)
{
    {
        Duckvil::Memory::__free_list_allocator* _free_list = (Duckvil::Memory::__free_list_allocator*)__duckvil_global::m_pHeap;

        DUCKVIL_TEST_IS_NOT_NULL(_free_list, "Free list allocator allocation failed");

        Duckvil::Memory::Vector<float> _vec(__duckvil_global::m_pMemoryInterface, _free_list, 2);

        DUCKVIL_TEST_EXP(_vec.Empty(), "Vector is not empty");

        _vec.Allocate(10.f);
        _vec.Allocate(20.f);

        DUCKVIL_TEST_EXP(_vec.Full(), "Vector is not full");

        DUCKVIL_TEST_EQUAL(_vec.At(0), 10.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(1), 20.f, "Wrong value");

        _vec.Resize(4);

        DUCKVIL_TEST_EXP(!_vec.Full(), "Vector is full");

        _vec.Allocate(30.f);
        _vec.Allocate(40.f);

        DUCKVIL_TEST_EXP(_vec.Full(), "Vector is not full");

        DUCKVIL_TEST_EQUAL(_vec.At(2), 30.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(3), 40.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(0), 10.f, "Wrong value");
        DUCKVIL_TEST_EQUAL(_vec.At(1), 20.f, "Wrong value");

        {
            Duckvil::Memory::Vector<float>::Iterator it = _vec.begin();

            DUCKVIL_TEST_EQUAL(*it++, 10.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(*it++, 20.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(*it++, 30.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(*it, 40.f, "Wrong value");
        }

        {
            _vec.Erase(0);

            DUCKVIL_TEST_EQUAL(_vec.Size(), (std::size_t)3, "Wrong size");

            DUCKVIL_TEST_EQUAL(_vec.At(0), 20.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(_vec.At(1), 30.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(_vec.At(2), 40.f, "Wrong value");

            Duckvil::Memory::Vector<float>::Iterator it = _vec.begin();

            DUCKVIL_TEST_EQUAL(*it++, 20.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(*it++, 30.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(*it, 40.f, "Wrong value");

            _vec.Erase(2);

            DUCKVIL_TEST_EQUAL(_vec.Size(), (std::size_t)2, "Wrong size");

            DUCKVIL_TEST_EQUAL(_vec.At(0), 20.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(_vec.At(1), 30.f, "Wrong value");

            it = _vec.begin();

            DUCKVIL_TEST_EQUAL(*it++, 20.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(*it, 30.f, "Wrong value");

            DUCKVIL_TEST_EQUAL(_vec.Back(), 30.f, "Wrong value");
            DUCKVIL_TEST_EQUAL(_vec.Begin(), 20.f, "Wrong value");
        }

        {
            DUCKVIL_TEST_EXP(_vec.begin() != _vec.end(), "Begin is equal End, lol");
        }

        {
            int i = 0;

            for(Duckvil::Memory::Vector<float>::Iterator it = _vec.begin(); it != _vec.end(); ++it, i++)
            {

            }

            DUCKVIL_TEST_EQUAL(i, 2, "Wrong value");
        }

        {
            _vec.Clear();

            DUCKVIL_TEST_EQUAL(_vec.Empty(), true, "Vector is not empty");
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}