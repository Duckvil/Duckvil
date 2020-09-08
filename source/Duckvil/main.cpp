#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv, g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors));

    return 0;
}