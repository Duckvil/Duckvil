#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Vector.h"

#include <cstring>

DUCKVIL_TEST(VectorAllocation)
{
    {
        Duckvil::Memory::free_list_allocator* _free_list = (Duckvil::Memory::free_list_allocator*)__duckvil_global::m_pHeap;

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

DUCKVIL_TEST(VectorCopyConstructor)
{
    {
        struct test_struct
        {
            int* m_pValue = nullptr;

            test_struct()
            {
                m_pValue = new int(10);
            }

            test_struct(const test_struct& _t)
            {
                m_pValue = new int(*_t.m_pValue);
            }

            ~test_struct()
            {
                delete m_pValue;

                m_pValue = nullptr;
            }
        };

        Duckvil::Memory::Vector<test_struct> _vec;

        __duckvil_global::m_freeList.Allocate(_vec, 1);

        {
            test_struct _t;

            _vec.Allocate(_t);
        }

        const test_struct& _r = _vec[0];

        DUCKVIL_TEST_NOT_EQUAL(_r.m_pValue, (int*)nullptr, "Pointer is null!");
        DUCKVIL_TEST_EQUAL(*_r.m_pValue, 10, "Wrong value!");
    }

    {
        struct test_struct
        {
            int m_pValue;

            test_struct()
            {
                m_pValue = 10;
            }
        };

        Duckvil::Memory::Vector<test_struct> _vec;

        __duckvil_global::m_freeList.Allocate(_vec, 1);

        {
            test_struct _t;

            _vec.Allocate(_t);
        }

        const test_struct& _r = _vec[0];

        DUCKVIL_TEST_EQUAL(_r.m_pValue, 10, "Wrong value!");
    }

    {
        struct test_struct
        {
            int* m_iV;
            bool m_bCopyConstructorCalled;

            test_struct(int _iV) :
                m_bCopyConstructorCalled(false)
            {
                m_iV = new int(_iV);
            }

            test_struct(const test_struct& _)
            {
                m_iV = new int(*_.m_iV);
                m_bCopyConstructorCalled = true;
            }
        };

        struct test_struct2
        {
            test_struct m_value;

            test_struct2(int _iV) :
                m_value(_iV)
            {

            }
        };

        Duckvil::Memory::Vector<test_struct2> _vec;

        __duckvil_global::m_freeList.Allocate(_vec, 1);

        test_struct2 _v(22);

        _vec.Allocate(std::move(_v));

        // DUCKVIL_TEST_EQUAL(_vec[0].m_value.m_sValue, "test", "");
        auto& a = _vec[0].m_value;
        auto& b = _v.m_value;

        DUCKVIL_TEST_EQUAL(a.m_bCopyConstructorCalled, true, "Copy constructor was not called");
        DUCKVIL_TEST_NOT_EQUAL(a.m_iV, b.m_iV, "Pointers are the same");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(VectorMoveConstructor)
{
    struct test_struct
    {
        int* m_pValue = nullptr;
        bool m_bMoveConstructorCalled;

        test_struct() :
            m_bMoveConstructorCalled(false)
        {
            m_pValue = new int(10);
        }

        test_struct(test_struct&& _t)
        {
            // m_pValue = new int(*_t.m_pValue);

            m_bMoveConstructorCalled = true;

            m_pValue = std::move(_t.m_pValue);

            _t.m_pValue = nullptr;
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

    Duckvil::Memory::Vector<test_struct> _vec;

    __duckvil_global::m_freeList.Allocate(_vec, 1);

    {
        _vec.Allocate(test_struct());
    }

    const test_struct& _r = _vec[0];

    DUCKVIL_TEST_EQUAL(_r.m_bMoveConstructorCalled, true, "Move constructor was not called!");
    DUCKVIL_TEST_NOT_EQUAL(_r.m_pValue, (int*)nullptr, "Pointer is null!");
    DUCKVIL_TEST_EQUAL(*_r.m_pValue, 10, "Wrong value!");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(Test)
{
    {
        Duckvil::Memory::Vector<Duckvil::Memory::Vector<int>> _vec;

        __duckvil_global::m_freeList.Allocate(_vec, 1);

        Duckvil::Memory::Vector<int> _vec2;

        __duckvil_global::m_freeList.Allocate(_vec2, 1);

        _vec.Allocate(_vec2);

        auto& _a = _vec[0];

        DUCKVIL_TEST_EQUAL(_a.Empty(), true, "Vector is not empty");

        _a.Allocate(10);

        DUCKVIL_TEST_EQUAL(_a.Full(), true, "Vector is not full");
        DUCKVIL_TEST_EQUAL(_vec2.Empty(), true, "Vector is not empty");
    }

    {
        Duckvil::Memory::Vector<Duckvil::Memory::Vector<int>> _vec;

        __duckvil_global::m_freeList.Allocate(_vec, 1);

        Duckvil::Memory::Vector<int> _vec2;

        __duckvil_global::m_freeList.Allocate(_vec2, 1);

        _vec2.Allocate(10);

        _vec.Allocate(_vec2);

        auto& _a = _vec[0];

        DUCKVIL_TEST_EQUAL(_a.Full(), true, "Vector is not full");
    }

    {
        struct test_struct
        {
            int* m_pValue;

            test_struct(const int& _v)
            {
                m_pValue = new int(_v);
            }

            // test_struct(test_struct&& _v)
            // {
            //     m_pValue = std::move(_v.m_pValue);

            //     _v.m_pValue = nullptr;
            // }

            ///
            test_struct(const test_struct& _v)
            {
                m_pValue = new int(*_v.m_pValue);
            }

            ~test_struct()
            {
                if(!m_pValue)
                {
                    return;
                }

                delete m_pValue;

                m_pValue = nullptr;
            }
        };

        // {
        //     std::vector<std::vector<test_struct>> _a;
        //     std::vector<test_struct> _b;

        //     _b.push_back(10);

        //     _a.push_back(_b);

        //     auto& _c = _a[0];
        // }

        {
            Duckvil::Memory::Vector<Duckvil::Memory::Vector<test_struct>> _vec;

            __duckvil_global::m_freeList.Allocate(_vec, 1);

            Duckvil::Memory::Vector<test_struct> _vec2;

            __duckvil_global::m_freeList.Allocate(_vec2, 1);

            _vec2.Allocate(test_struct(10));

            _vec.Allocate(_vec2);

            auto& _a = _vec[0];

            DUCKVIL_TEST_EQUAL(_a[0].m_pValue, _vec[0][0].m_pValue, "Pointers are not the same");
        }

        {
            Duckvil::Memory::Vector<std::vector<test_struct>> _vec;

            __duckvil_global::m_freeList.Allocate(_vec, 1);

            std::vector<test_struct> _vec2;

            _vec2.push_back(test_struct(10));

            _vec.Allocate(_vec2);

            auto& _a = _vec[0];

            DUCKVIL_TEST_EQUAL(_a[0].m_pValue, _vec[0][0].m_pValue, "Pointers are not the same");
        }
    }

    {
        struct test_struct
        {
            int* m_pValue;

            test_struct(const int& _v)
            {
                m_pValue = new int(_v);
            }

            test_struct(const test_struct& _v)
            {
                m_pValue = new int(*_v.m_pValue);
            }

            ~test_struct()
            {
                delete m_pValue;

                m_pValue = nullptr;
            }
        };

        Duckvil::Memory::Vector<Duckvil::Memory::Vector<test_struct>> _vec;

        __duckvil_global::m_freeList.Allocate(_vec, 1);

        Duckvil::Memory::Vector<test_struct> _vec2;

        __duckvil_global::m_freeList.Allocate(_vec2, 1);

        _vec2.Allocate(test_struct(10));

        _vec.Allocate(_vec2);

        auto _a = _vec[0];

        DUCKVIL_TEST_NOT_EQUAL(_a[0].m_pValue, _vec[0][0].m_pValue, "Pointers are the same");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}