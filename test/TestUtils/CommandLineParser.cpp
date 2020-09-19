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
        char* commands[] =
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
        char* commands[] =
        {
            "test.exe",
            "does_not_exists"
        };

        Duckvil::Utils::CommandArgumentsParser _parser(3, commands);

        DUCKVIL_TEST_EQUAL(_parser.Parse(_desc, DUCKVIL_ARRAY_SIZE(_desc)), false, "Parse passed");
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}