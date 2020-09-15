#include "UniTestFramework/UniTestFramework.h"

#include "Utils/Utils.h"

DUCKVIL_TEST(Split)
{
    {
        std::vector<std::string> _splitted = Duckvil::Utils::split("a.b.c.d", '.');

        DUCKVIL_TEST_EXP(_splitted.size() == 4, "Wrong splitted size");
        DUCKVIL_TEST_EQUAL(_splitted[0], "a", "Wrong value");
        DUCKVIL_TEST_EQUAL(_splitted[1], "b", "Wrong value");
        DUCKVIL_TEST_EQUAL(_splitted[2], "c", "Wrong value");
        DUCKVIL_TEST_EQUAL(_splitted[3], "d", "Wrong value");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}