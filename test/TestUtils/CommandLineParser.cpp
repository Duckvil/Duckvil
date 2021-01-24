#include "UniTestFramework/UniTestFramework.h"

#include "Utils/CommandArgumentsParser.h"

DUCKVIL_TEST(CommandLineParser)
{
    enum Commands
    {
        TEST,
        TEST2,
        NOT_SET
    };

    Duckvil::Utils::CommandArgumentsParser::Descriptor _desc[] =
    {
        Duckvil::Utils::CommandArgumentsParser::Descriptor(Commands::TEST, "test"),
        Duckvil::Utils::CommandArgumentsParser::Descriptor(Commands::TEST2, "test2"),
        Duckvil::Utils::CommandArgumentsParser::Descriptor(Commands::NOT_SET, "not_set"),
    };

    {
        {
            const char* commands[] =
            {
                "test.exe",
                "-test",
                "-test2 lol",
                "does_not_exists"
            };

            Duckvil::Utils::CommandArgumentsParser _parser(3, commands);

            DUCKVIL_TEST_EQUAL(_parser.Parse(_desc, DUCKVIL_ARRAY_SIZE(_desc)), true, "Parse failed");

            DUCKVIL_TEST_EQUAL(_parser[Commands::TEST].m_bIsSet, true, "Should be turned on");
            DUCKVIL_TEST_EQUAL(_parser[Commands::TEST2].m_sResult, "lol", "Should be turned on");
            DUCKVIL_TEST_EQUAL(_parser[Commands::NOT_SET].m_bIsSet, false, "Should be turned off");
        }

        {
            const char* commands[] =
            {
                "test.exe",
                "does_not_exists"
            };

            Duckvil::Utils::CommandArgumentsParser _parser(2, commands);

            DUCKVIL_TEST_EQUAL(_parser.Parse(_desc, DUCKVIL_ARRAY_SIZE(_desc)), false, "Parse passed");
        }

        {
            const char* commands[] =
            {
                "test.exe",
                "-test",
                "-test2 lol",
                "does_not_exists"
            };

            Duckvil::Utils::CommandArgumentsParser _parser(3, commands);

            DUCKVIL_TEST_EQUAL(_parser.Parse(0, 0), true, "Nothing to parse");
        }
    }

    {
        {
            const size_t _s1 = sizeof("test.exe");
            const size_t _s2 = sizeof("-test");
            const size_t _s3 = sizeof("-test2 lol");
            const size_t _s4 = sizeof( "does_not_exists");
            char _c1[_s1] = { 0 };
            char _c2[_s2] = { 0 };
            char _c3[_s3] = { 0 };
            char _c4[_s4] = { 0 };

            memcpy(_c1, "test.exe", _s1);
            memcpy(_c2, "-test", _s2);
            memcpy(_c3, "-test2 lol", _s3);
            memcpy(_c4, "does_not_exists", _s4);

            char* commands[] =
            {
                _c1,
                _c2,
                _c3,
                _c4
            };

            Duckvil::Utils::CommandArgumentsParser _parser(3, commands);

            DUCKVIL_TEST_EQUAL(_parser.Parse(_desc, DUCKVIL_ARRAY_SIZE(_desc)), true, "Parse failed");

            DUCKVIL_TEST_EQUAL(_parser[Commands::TEST].m_bIsSet, true, "Should be turned on");
            DUCKVIL_TEST_EQUAL(_parser[Commands::TEST2].m_sResult, "lol", "Should be turned on");
            DUCKVIL_TEST_EQUAL(_parser[Commands::NOT_SET].m_bIsSet, false, "Should be turned off");
        }

        {
            const size_t _s1 = sizeof("test.exe");
            const size_t _s2 = sizeof("does_not_exists");
            char _c1[_s1] = { 0 };
            char _c2[_s2] = { 0 };

            memcpy(_c1, "test.exe", _s1);
            memcpy(_c2, "does_not_exists", _s2);

            char* commands[] =
            {
               _c1,
               _c2
            };

            Duckvil::Utils::CommandArgumentsParser _parser(2, commands);

            DUCKVIL_TEST_EQUAL(_parser.Parse(_desc, DUCKVIL_ARRAY_SIZE(_desc)), false, "Parse passed");
        }

        {
            const size_t _s1 = sizeof("test.exe");
            const size_t _s2 = sizeof("-test");
            const size_t _s3 = sizeof("-test2 lol");
            const size_t _s4 = sizeof( "does_not_exists");
            char _c1[_s1] = { 0 };
            char _c2[_s2] = { 0 };
            char _c3[_s3] = { 0 };
            char _c4[_s4] = { 0 };

            memcpy(_c1, "test.exe", _s1);
            memcpy(_c2, "-test", _s2);
            memcpy(_c3, "-test2 lol", _s3);
            memcpy(_c4, "does_not_exists", _s4);

            char* commands[] =
            {
                _c1,
                _c2,
                _c3,
                _c4
            };

            Duckvil::Utils::CommandArgumentsParser _parser(3, commands);

            DUCKVIL_TEST_EQUAL(_parser.Parse(0, 0), true, "Nothing to parse");
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}