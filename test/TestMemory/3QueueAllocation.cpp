#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Queue.h"

#include "Memory/QueueAllocator.h"

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

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(QueueAllocation)
{
    {
        Duckvil::Memory::queue_allocator* _queue = static_cast<Duckvil::Memory::queue_allocator*>(__duckvil_global::m_pMemoryInterface->m_fnFreeListAllocate_((Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, sizeof(Duckvil::Memory::queue_allocator) + 128, alignof(Duckvil::Memory::queue_allocator)));

        DUCKVIL_TEST_IS_NOT_NULL(_queue, "Queue allocator is null");

        _queue->m_ullCapacity = 128;
        _queue->m_ullUsed = 0;
        _queue->m_ullTail = 0;
        _queue->m_ullHead = 0;

        {
            int _v = 10;

            int* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, _v, "Wrong first value");
        }

        {
            float _v = 20;

            float* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, 10, "Wrong first value");
        }

        {
            char _v = 'a';

            char* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, 10, "Wrong first value");
        }

        {
            Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

            float* _b = static_cast<float*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");
            DUCKVIL_TEST_EQUAL(*_b, 20.f, "Wrong first value");
        }

        {
            Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

            char* _b = static_cast<char*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");
            DUCKVIL_TEST_EQUAL(*_b, 'a', "Wrong first value");
        }

        Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

        {
            int _v = 10;

            int* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, _v, "Wrong first value");
        }

        {
            float _v = 20;

            float* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, 10, "Wrong first value");
        }

        {
            char _v = 'a';

            char* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, 10, "Wrong first value");
        }

        {
            Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

            float* _b = static_cast<float*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");
            DUCKVIL_TEST_EQUAL(*_b, 20.f, "Wrong first value");
        }

        {
            Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

            char* _b = static_cast<char*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");
            DUCKVIL_TEST_EQUAL(*_b, 'a', "Wrong first value");
        }

        Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);
    }

    {
        Duckvil::Memory::queue_allocator* _queue = static_cast<Duckvil::Memory::queue_allocator*>(__duckvil_global::m_pMemoryInterface->m_fnFreeListAllocate_((Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, sizeof(Duckvil::Memory::queue_allocator) + 128, alignof(Duckvil::Memory::queue_allocator)));

        DUCKVIL_TEST_IS_NOT_NULL(_queue, "Queue allocator is null");

        _queue->m_ullCapacity = 128;
        _queue->m_ullUsed = 0;
        _queue->m_ullTail = 0;
        _queue->m_ullHead = 0;

        {
            int _v = 10;

            int* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, _v, "Wrong first value");
        }

        Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

        {
            float _v = 20;

            float* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            float* _b = static_cast<float*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, _v, "Wrong first value");
        }

        Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

        {
            char _v = 'a';

            char* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            char* _b = static_cast<char*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, _v, "Wrong first value");
        }
    }

    {
        Duckvil::Memory::queue_allocator* _queue = static_cast<Duckvil::Memory::queue_allocator*>(__duckvil_global::m_pMemoryInterface->m_fnFreeListAllocate_((Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap, sizeof(Duckvil::Memory::queue_allocator) + 64, alignof(Duckvil::Memory::queue_allocator)));

        DUCKVIL_TEST_IS_NOT_NULL(_queue, "Queue allocator is null");

        _queue->m_ullCapacity = 64;
        _queue->m_ullUsed = 0;
        _queue->m_ullTail = 0;
        _queue->m_ullHead = 0;

        {
            int _v = 10;

            int* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, _v, "Wrong first value");
        }

        {
            float _v = 20;

            float* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_a, "Allocated value is null");
            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");

            DUCKVIL_TEST_EQUAL(*_a, _v, "Wrong allocated value");
            DUCKVIL_TEST_EQUAL(*_b, 10, "Wrong first value");
        }

        {
            char _v = 'c';
            bool _exception = false;

            try
            {
                char* _a = Duckvil::Memory::queue_allocate(__duckvil_global::m_pMemoryInterface, _queue, _v);
            }
            catch(const std::exception& _e)
            {
                _exception = true;
            }

            DUCKVIL_TEST_EQUAL(_exception, true, "Exception was not thrown");

            int* _b = static_cast<int*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");
            DUCKVIL_TEST_EQUAL(*_b, 10, "Wrong first value");
        }

        {
            Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

            float* _b = static_cast<float*>(Duckvil::Memory::queue_begin(__duckvil_global::m_pMemoryInterface, _queue));

            DUCKVIL_TEST_IS_NOT_NULL(_b, "First value is null");
            DUCKVIL_TEST_EQUAL(*_b, 20.f, "Wrong first value");
        }

        {
            Duckvil::Memory::queue_pop(__duckvil_global::m_pMemoryInterface, _queue);

            DUCKVIL_TEST_EQUAL(_queue->m_ullUsed, static_cast<std::size_t>(0), "Size not equal to 0");
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
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