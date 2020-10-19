#include "TestRuntimeReflectionType/Test.h"

#include <stdio.h>

namespace Duckvil { namespace Test {

    TestType::TestType()
    {

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
        printf("Member function no arguments\n");
    }

    void TestType::test2(float a, float b)
    {
        printf("Member function 2 arguments %f %f\n", a, b);
    }

    void TestType::test3()
    {
        printf("Static function no arguments\n");
    }

    void TestType::test4(float a, float b)
    {
        printf("Static function 2 arguments %f %f\n", a, b);
    }

}}