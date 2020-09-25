#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/FixedQueueAllocator.h"

#include <cstring>

DUCKVIL_TEST(FixedQueueAllocation)
{
    {
        Duckvil::Memory::Queue<int> _queue(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::__allocator*)__duckvil_global::m_memoryChunk, 4);

        DUCKVIL_TEST_EXP(_queue.Empty(), "Queue is not empty");
        DUCKVIL_TEST_EQUAL(_queue.GetCapacity(), 4 * sizeof(int), "Wrong capacity");

    // Contiguous allocation
        {
            _queue.Allocate(1);
            _queue.Allocate(2);
            _queue.Allocate(3);
            _queue.Allocate(4);

            DUCKVIL_TEST_EXP(_queue.Full(), "Queue is not full");
            DUCKVIL_TEST_EQUAL(_queue.GetSize(), 4 * sizeof(int), "Wrong size");

            DUCKVIL_TEST_EQUAL(_queue.Begin(), 1, "Wrong value");
            _queue.Pop();

            DUCKVIL_TEST_EQUAL(_queue.Begin(), 2, "Wrong value");
            _queue.Pop();

            DUCKVIL_TEST_EQUAL(_queue.Begin(), 3, "Wrong value");
            _queue.Pop();

            DUCKVIL_TEST_EQUAL(_queue.Begin(), 4, "Wrong value");
            _queue.Pop();

            DUCKVIL_TEST_EXP(_queue.Empty(), "Queue is not empty");
        }

    // Commutative allocation
        {
            {
                _queue.Allocate(1);

                DUCKVIL_TEST_EQUAL(_queue.Begin(), 1, "Wrong value");
                _queue.Pop();

                _queue.Allocate(2);

                DUCKVIL_TEST_EQUAL(_queue.Begin(), 2, "Wrong value");
                _queue.Pop();

                _queue.Allocate(3);

                DUCKVIL_TEST_EQUAL(_queue.Begin(), 3, "Wrong value");
                _queue.Pop();

                _queue.Allocate(4);

                DUCKVIL_TEST_EQUAL(_queue.Begin(), 4, "Wrong value");
                _queue.Pop();

                DUCKVIL_TEST_EXP(_queue.Empty(), "Queue is not empty");
            }

            {
                _queue.Allocate(1);
                DUCKVIL_TEST_EQUAL(_queue.Begin(), 1, "Wrong value");

                _queue.Allocate(2);
                DUCKVIL_TEST_EQUAL(_queue.Begin(), 1, "Wrong value");

                _queue.Pop();

                DUCKVIL_TEST_EQUAL(_queue.Begin(), 2, "Wrong value");

                _queue.Pop();

                DUCKVIL_TEST_EXP(_queue.Empty(), "Queue is not empty");
            }

            {
                for(int i = 0; i < 100; i++)
                {
                    _queue.Allocate(1 * i);
                    _queue.Allocate(2 * i);
                    _queue.Allocate(3 * i);

                    DUCKVIL_TEST_EQUAL(_queue.Begin(), 1 * i, "Wrong value");
                    _queue.Pop();

                    DUCKVIL_TEST_EQUAL(_queue.Begin(), 2 * i, "Wrong value");
                    _queue.Pop();

                    DUCKVIL_TEST_EQUAL(_queue.Begin(), 3 * i, "Wrong value");
                    _queue.Pop();

                    DUCKVIL_TEST_EXP(_queue.Empty(), "Queue is not empty");
                }
            }
        }

    // TODO: String allocation test
        {

        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}