#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Stack.h"

#include "Memory/StackAllocator.h"

#include <cstring>

DUCKVIL_TEST(FixedStackAllocation)
{
    {
        Duckvil::Memory::Stack<int> _stack(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::linear_allocator*)__duckvil_global::m_pMemoryChunk, 1024 * 1024);
    }

    {
        // TODO: Allocation size of size_t return 0 or exception
        // Duckvil::Memory::Stack<int> _stack(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::allocator*)__duckvil_global::m_pMemoryChunk, std::numeric_limits<std::size_t>::max());
    }

    Duckvil::Memory::Stack<int> _stack(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::linear_allocator*)__duckvil_global::m_pMemoryChunk, 4);

    DUCKVIL_TEST_EXP(_stack.Empty(), "Stack is not empty");
    DUCKVIL_TEST_EQUAL(_stack.GetCapacity(), (size_t)4, "Wrong capacity");

// Overflow detection
    {
        _stack.Allocate(1);
        _stack.Allocate(2);
        _stack.Allocate(3);
        _stack.Allocate(4);

        bool _overflow = false;

        try
        {
            _stack.Allocate(5);
        }
        catch(const std::exception& _e)
        {
            _overflow = true;
        }

        DUCKVIL_TEST_EQUAL(_stack.GetSize(), (size_t)4, "Wrong size");
        DUCKVIL_TEST_EXP(_overflow, "Overflow not detected");
        DUCKVIL_TEST_EXP(_stack.Full(), "Overflow not detected");
        DUCKVIL_TEST_EQUAL(_stack.Top(), 4, "Overflow not detected");

        _stack.Pop();
        _stack.Pop();
        _stack.Pop();
        _stack.Pop();
    }

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

DUCKVIL_TEST(StackAllocation)
{
    Duckvil::Memory::stack_allocator* _stack = static_cast<Duckvil::Memory::stack_allocator*>(__duckvil_global::m_pMemoryInterface->m_fnFreeListAllocate_((Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, sizeof(Duckvil::Memory::stack_allocator) + 128, alignof(Duckvil::Memory::stack_allocator)));

    DUCKVIL_TEST_IS_NOT_NULL(_stack, "Stack allocator is null");

    _stack->m_ullUsed = 0;
    _stack->m_ullCapacity = 128;

    // {
        int _v1 = 10;

        int* _a1 = Duckvil::Memory::stack_allocate(__duckvil_global::m_pMemoryInterface, _stack, _v1);
        int* _t1 = static_cast<int*>(Duckvil::Memory::stack_top(__duckvil_global::m_pMemoryInterface, _stack));

        DUCKVIL_TEST_IS_NOT_NULL(_a1, "Allocated value is null");
        DUCKVIL_TEST_IS_NOT_NULL(_t1, "Top value is null");

        DUCKVIL_TEST_EQUAL(*_a1, _v1, "Wrong allocated value");
        DUCKVIL_TEST_EQUAL(*_t1, _v1, "Wrong top value");
    // }

    // {
        float _v2 = 20;

        float* _a2 = Duckvil::Memory::stack_allocate(__duckvil_global::m_pMemoryInterface, _stack, _v2);
        float* _t2 = static_cast<float*>(Duckvil::Memory::stack_top(__duckvil_global::m_pMemoryInterface, _stack));

        DUCKVIL_TEST_IS_NOT_NULL(_a2, "Allocated value is null");
        DUCKVIL_TEST_IS_NOT_NULL(_t2, "Top value is null");

        DUCKVIL_TEST_EQUAL(*_a2, _v2, "Wrong allocated value");
        DUCKVIL_TEST_EQUAL(*_t2, _v2, "Wrong top value");
    // }

    // {
        char _v3 = 'a';

        char* _a3 = Duckvil::Memory::stack_allocate(__duckvil_global::m_pMemoryInterface, _stack, _v3);
        char* _t3 = static_cast<char*>(Duckvil::Memory::stack_top(__duckvil_global::m_pMemoryInterface, _stack));

        DUCKVIL_TEST_IS_NOT_NULL(_a3, "Allocated value is null");
        DUCKVIL_TEST_IS_NOT_NULL(_t3, "Top value is null");

        DUCKVIL_TEST_EQUAL(*_a3, _v3, "Wrong allocated value");
        DUCKVIL_TEST_EQUAL(*_t3, _v3, "Wrong top value");
    // }

    Duckvil::Memory::stack_pop(__duckvil_global::m_pMemoryInterface, _stack);

    float* _p1 = static_cast<float*>(Duckvil::Memory::stack_top(__duckvil_global::m_pMemoryInterface, _stack));

    DUCKVIL_TEST_EQUAL(*_p1, 20.f, "Wrong top value");

    Duckvil::Memory::stack_pop(__duckvil_global::m_pMemoryInterface, _stack);

    int* _p2 = static_cast<int*>(Duckvil::Memory::stack_top(__duckvil_global::m_pMemoryInterface, _stack));

    DUCKVIL_TEST_EQUAL(*_p2, 10, "Wrong top value");

    DUCKVIL_TEST_SUCCESS_PASS;
}