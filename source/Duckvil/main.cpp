#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "Memory/Memory.h"

#include "PlugNPlay/Module.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::__module memoryModule("Memory.dll");
    Duckvil::Memory::IMemory* (*init)();

    Duckvil::PlugNPlay::load(&memoryModule);
    Duckvil::PlugNPlay::get(memoryModule, "duckvil_memory_init", (void**)&init);

    Duckvil::Memory::IMemory* memory = init();
    Duckvil::Memory::__memory memoryChunk = { 0 };

    int a = 10;
    int b = 20;

    memory->m_fnBasicAllocate(&memoryChunk, 1024);
    int* res = (int*)memory->m_fnLinearAllocate(&memoryChunk, &a, sizeof(int));
    int* res2 = (int*)memory->m_fnLinearAllocate(&memoryChunk, &b, sizeof(int));

    Duckvil::PlugNPlay::free(&memoryModule);

    return 0;
}