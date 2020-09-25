#include "UniTestFramework/UniTestFramework.h"

DUCKVIL_TEST(UniTestFramework)
{
    using namespace Duckvil::UniTestFramework;

    if(!Assert::AreEqual(10, 10))
    {
        return false;
    }

    if(Assert::AreEqual(10, 11))
    {
        return false;
    }

    if(!Assert::NotEqual(10, 11))
    {
        return false;
    }

    if(Assert::NotEqual(10, 10))
    {
        return false;
    }

    int* _p = nullptr;

    if(!Assert::IsNULL(_p))
    {
        return false;
    }

    if(Assert::IsNotNULL(_p))
    {
        return false;
    }

    int _v = 10;

    _p = &_v;

    if(!Assert::IsNotNULL(_p))
    {
        return false;
    }

    if(Assert::IsNULL(_p))
    {
        return false;
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    return DUCKVIL_TEST_FAILED;
}