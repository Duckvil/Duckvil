#include "UniTestFramework/UniTestFramework.h"

DUCKVIL_TEST(UniTestFramework)
{
    // using namespace Duckvil::UniTestFramework;
    namespace UTF = Duckvil::UniTestFramework;

    if(!UTF::Assert::AreEqual(10, 10))
    {
        return false;
    }

    if(UTF::Assert::AreEqual(10, 11))
    {
        return false;
    }

    if(!UTF::Assert::NotEqual(10, 11))
    {
        return false;
    }

    if(UTF::Assert::NotEqual(10, 10))
    {
        return false;
    }

    int* _p = nullptr;

    if(!UTF::Assert::IsNULL(_p))
    {
        return false;
    }

    if(UTF::Assert::IsNotNULL(_p))
    {
        return false;
    }

    int _v = 10;

    _p = &_v;

    if(!UTF::Assert::IsNotNULL(_p))
    {
        return false;
    }

    if(UTF::Assert::IsNULL(_p))
    {
        return false;
    }

    Duckvil::UniTestFramework::test_failed("Test", "Test", __FILE__, __LINE__);

    if(DUCKVIL_TEST_FAILED != true)
    {
        return false;
    }

    DUCKVIL_TEST_FAILED = false;

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    return DUCKVIL_TEST_FAILED;
}