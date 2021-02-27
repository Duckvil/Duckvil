#include "TestType.h"

#include <stdio.h>

namespace Duckvil { namespace Test {

    TestType::TestType()
    {
        m_fA = 6.66f;
    }

    TestType::TestType(float a) :
        m_fA(a)
    {

    }

    TestType::~TestType()
    {

    }

    void TestType::test()
    {
        // printf("Member function no arguments\n");
    }

    void TestType::test2(float a, float b)
    {
        // printf("Member function 2 arguments %f %f\n", a, b);
    }

    void TestType::test3()
    {
        // printf("Static function no arguments\n");
    }

    void TestType::test4(float a, float b)
    {
        // printf("Static function 2 arguments %f %f\n", a, b);
    }

    int TestType::test5(float a, float b)
    {
        return (int)a + (int)b;
    }

    int TestType::test6()
    {
        return 666;
    }

    void TestType::test7()
    {

    }

    void TestType::test7(int a)
    {

    }

    void TestType::test8()
    {

    }

    void TestType::test8(int a)
    {

    }

}}