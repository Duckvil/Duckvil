#pragma once

// #include "RuntimeReflection/OOP/Recorder.h"

#include "RuntimeReflection/Markers.h"

namespace Duckvil { namespace Test {

    DUCKVIL_CLASS(1 = 10)
    class TestType
    {
    private:

    public:
        TestType();
        TestType(float a);
        ~TestType();

        DUCKVIL_VARIABLE(2 = 20)
        float m_fA;

        void test();
        void test2(float a, float b);
        static void test3();
        static void test4(float a, float b);
    };

}}