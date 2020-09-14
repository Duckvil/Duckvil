#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/FixedStackAllocator.h"

#include <cstring>

DUCKVIL_TEST(FixedStackAllocation)
{
    Duckvil::Memory::Stack<int> _stack(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::__allocator*)__duckvil_global::m_memoryChunk, 4);

    DUCKVIL_TEST_EXP(_stack.Empty(), "Stack is not empty");

// Contiguous allocation
    {
        _stack.Allocate(1);
        _stack.Allocate(2);
        _stack.Allocate(3);
        _stack.Allocate(4);

        DUCKVIL_TEST_EXP(_stack.Full(), "Stack is not full");

        DUCKVIL_TEST_EQUAL(_stack.Top(), 4, "Wrong value");
        _stack.Pop();

        DUCKVIL_TEST_EQUAL(_stack.Top(), 3, "Wrong value");
        _stack.Pop();

        DUCKVIL_TEST_EQUAL(_stack.Top(), 2, "Wrong value");
        _stack.Pop();

        DUCKVIL_TEST_EQUAL(_stack.Top(), 1, "Wrong value");
        _stack.Pop();

        DUCKVIL_TEST_EXP(_stack.Empty(), "Stack is not empty");
    }

// Commutative allocation
    {
        {
            _stack.Allocate(1);

            DUCKVIL_TEST_EQUAL(_stack.Top(), 1, "Wrong value");
            _stack.Pop();

            _stack.Allocate(2);

            DUCKVIL_TEST_EQUAL(_stack.Top(), 2, "Wrong value");
            _stack.Pop();

            _stack.Allocate(3);

            DUCKVIL_TEST_EQUAL(_stack.Top(), 3, "Wrong value");
            _stack.Pop();

            _stack.Allocate(4);

            DUCKVIL_TEST_EQUAL(_stack.Top(), 4, "Wrong value");
            _stack.Pop();
        }

        {
            _stack.Allocate(1);
            _stack.Allocate(2);

            DUCKVIL_TEST_EQUAL(_stack.Top(), 2, "Wrong value");
            _stack.Pop();

            DUCKVIL_TEST_EQUAL(_stack.Top(), 1, "Wrong value");
            _stack.Pop();

            DUCKVIL_TEST_EXP(_stack.Empty(), "Stack is not empty");
        }

        {
            for(int i = 0; i < 100; i++)
            {
                _stack.Allocate(1 * i);
                _stack.Allocate(2 * i);
                _stack.Allocate(3 * i);

                DUCKVIL_TEST_EQUAL(_stack.Top(), 3 * i, "Wrong value");
                _stack.Pop();

                DUCKVIL_TEST_EQUAL(_stack.Top(), 2 * i, "Wrong value");
                _stack.Pop();

                DUCKVIL_TEST_EQUAL(_stack.Top(), 1 * i, "Wrong value");
                _stack.Pop();

                DUCKVIL_TEST_EXP(_stack.Empty(), "Stack is not empty");
            }
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}