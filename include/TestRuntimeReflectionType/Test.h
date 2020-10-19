#pragma once

// #include "RuntimeReflection/OOP/Recorder.h"

namespace Duckvil { namespace Test {

    class TestType
    {
    private:

    public:
        TestType();
        TestType(float a);
        ~TestType();

        float m_fA;

        void test();
        void test2(float a, float b);
        static void test3();
        static void test4(float a, float b);
    };

}}