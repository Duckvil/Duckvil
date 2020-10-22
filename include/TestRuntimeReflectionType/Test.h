#pragma once

// #include "RuntimeReflection/OOP/Recorder.h"

#include "RuntimeReflection/Markers.h"

namespace Duckvil { namespace Test {

    DUCKVIL_CLASS("a" = 10, "b" = 20, 1 = 30, 1.f = 40)
    class TestType
    {
    private:

    public:
        DUCKVIL_CONSTRUCTOR("a" = 1)
        TestType();
        TestType(float a);
        ~TestType();

        DUCKVIL_VARIABLE(1 = 11, "a" = 22)
        float m_fA;

        void test();
        void test2(float a, float b);
        static void test3();
        static void test4(float a, float b);
    };

}}