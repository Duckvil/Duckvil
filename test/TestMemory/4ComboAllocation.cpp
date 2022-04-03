#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/Vector.h"
#include "Memory/Stack.h"

#include <cstring>

DUCKVIL_TEST(ComboAllocation)
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

    Duckvil::Memory::Vector<Duckvil::Memory::Stack<test_struct>> _vec;

    __duckvil_global::m_freeList.Allocate(_vec, 1);

    Duckvil::Memory::Stack<test_struct> _stack;

    __duckvil_global::m_freeList.Allocate(_stack, 1);

    _stack.Allocate(test_struct());

    _vec.Allocate(_stack);

    DUCKVIL_TEST_NOT_EQUAL(_vec[0].Top().m_pValue, _stack.Top().m_pValue, "Pointers are the same");

    DUCKVIL_TEST_SUCCESS_PASS;
}