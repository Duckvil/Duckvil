#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Queue.h"

#include <cstring>

DUCKVIL_TEST(FixedQueueAllocation)
{
    {
        Duckvil::Memory::Queue<int> _queue(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, 4);

        DUCKVIL_TEST_EXP(_queue.Empty(), "Queue is not empty");
        DUCKVIL_TEST_EQUAL(_queue.GetCapacity(), (size_t)4, "Wrong capacity");

    // Contiguous allocation
        {
            _queue.Allocate(1);
            _queue.Allocate(2);
            _queue.Allocate(3);
            _queue.Allocate(4);

            auto _q = _queue;

            DUCKVIL_TEST_EXP(_queue.Full(), "Queue is not full");
            DUCKVIL_TEST_EQUAL(_queue.GetSize(), (size_t)4, "Wrong size");

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

DUCKVIL_TEST(FixedQueueMove)
{
    {
        struct test_struct
        {
            int* m_pValue;
            bool m_bMoveCalled;

            test_struct(int _iValue) :
                m_bMoveCalled(false)
            {
                m_pValue = new int(_iValue);
            }

            test_struct(test_struct&& _other)
            {
                m_pValue = std::move(_other.m_pValue);
                m_bMoveCalled = true;

                _other.m_pValue = nullptr;
                _other.m_bMoveCalled = true;
            }

            ~test_struct()
            {
                if(m_pValue == nullptr)
                {
                    return;
                }

                delete m_pValue;

                m_pValue = nullptr;
            }
        };

        Duckvil::Memory::Queue<test_struct> _queue(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, 4);

        {
            _queue.Allocate(test_struct(10));

            const auto& _v = _queue.Begin();

            DUCKVIL_TEST_EQUAL(_v.m_bMoveCalled, true, "Move contructor not called");

            _queue.Pop();
        }

        {
            test_struct _ts(20);

            _queue.Allocate(std::move(_ts));

            DUCKVIL_TEST_EQUAL(_ts.m_bMoveCalled, true, "Move contructor not called");
            DUCKVIL_TEST_EQUAL(_ts.m_pValue, (int*)nullptr, "Pointer is not null");

            const auto& _v = _queue.Begin();

            DUCKVIL_TEST_EQUAL(_v.m_bMoveCalled, true, "Move contructor not called");
        }
    }
}

DUCKVIL_TEST(FixedQueueCopy)
{
    {
        Duckvil::Memory::Queue<int> _queue(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, 4);

        _queue.Allocate(1);
        _queue.Allocate(2);
        _queue.Allocate(3);

        auto _q = _queue;

        DUCKVIL_TEST_EQUAL(_q.Begin(), _queue.Begin(), "Values are not equal");

        _queue.Pop();

        DUCKVIL_TEST_EQUAL(_queue.Begin(), 2, "Wrong value");
        DUCKVIL_TEST_EQUAL(_q.Begin(), 1, "Wrong value");

        _q.Pop();

        DUCKVIL_TEST_EQUAL(_q.Begin(), 2, "Wrong value");

        _queue.Allocate(4);
        _q.Allocate(4);

        DUCKVIL_TEST_EQUAL(_q.Begin(), 2, "Wrong value");
        DUCKVIL_TEST_EQUAL(_queue.Begin(), 2, "Wrong value");
    }

    {
        struct test_struct
        {
            int* m_pValue;
            bool m_bCopyConstructorCalled;

            test_struct(int _iValue) :
                m_bCopyConstructorCalled(false)
            {
                m_pValue = new int(_iValue);
            }

            test_struct(const test_struct& _other)
            {
                m_pValue = new int(*_other.m_pValue);
                m_bCopyConstructorCalled = true;
            }

            ~test_struct()
            {
                if(m_pValue == nullptr)
                {
                    return;
                }

                delete m_pValue;

                m_pValue = nullptr;
            }
        };

        Duckvil::Memory::Queue<test_struct> _queue(__duckvil_global::m_pMemoryInterface, (Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, 4);

        test_struct _ts(10);

        _queue.Allocate(_ts);

        const auto& _v = _queue.Begin();

        DUCKVIL_TEST_EQUAL(_v.m_bCopyConstructorCalled, true, "Copy constructor not called");
        DUCKVIL_TEST_NOT_EQUAL(_v.m_pValue, _ts.m_pValue, "Pointers are not different");
    }

    // DUCKVIL_TEST_EQUAL(_q.m_pContainer->m_ullTail, _queue.m_pContainer->m_ullTail, "Tail is different");
    // DUCKVIL_TEST_EQUAL(_q.m_pContainer->m_ullHead, _queue.m_pContainer->m_ullHead, "Head is different");

    // _q.Allocate(4);
    // _queue.Allocate(4);

    // DUCKVIL_TEST_EQUAL(_q.m_pContainer->m_ullTail, _queue.m_pContainer->m_ullTail, "Tail is different");
    // DUCKVIL_TEST_EQUAL(_q.m_pContainer->m_ullHead, _queue.m_pContainer->m_ullHead, "Head is different");

    DUCKVIL_TEST_SUCCESS_PASS;
}