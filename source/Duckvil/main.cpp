#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "Memory/Memory.h"
#include "Memory/Allocator.h"

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
    Duckvil::Memory::__linear_allocator memoryChunk = { 0 };

    int a = 10;

    memory->m_fnBasicAllocate(&memoryChunk, 1024);

    Duckvil::Memory::__linear_allocator* otherLinear = memory->m_fnAllocateLinearAllocator(&memoryChunk, 64);
    Duckvil::Memory::__linear_allocator* otherLinear2 = memory->m_fnAllocateLinearAllocator(&memoryChunk, 64);

    Duckvil::Memory::linear_allocate(memory, otherLinear, true);
    Duckvil::Memory::linear_allocate(memory, otherLinear, true);
    Duckvil::Memory::linear_allocate(memory, otherLinear, true);
    Duckvil::Memory::linear_allocate(memory, otherLinear, true);
    int* res = (int*)memory->m_fnLinearAllocate_(otherLinear, &a, sizeof(int), alignof(int));
    memory->m_fnLinearAllocate_(otherLinear, &a, sizeof(int), alignof(int));
    int* res4 = (int*)memory->m_fnLinearAllocate_(otherLinear2, &a, sizeof(int), alignof(int));
    const char* res3 = Duckvil::Memory::linear_allocate(memory, memoryChunk, "aaaa");
    int* res2 = Duckvil::Memory::linear_allocate(memory, memoryChunk, 20);

    for(uint32_t i = 0; i < 1024;)
    {
        for(uint32_t j = 0; j < 8; i++, j++)
        {
            printf("%02x ", memoryChunk.memory[i]);
        }

        printf("\n");
    }

    memory->m_fnLinearClear(memoryChunk);

    Duckvil::PlugNPlay::free(&memoryModule);

    return 0;
}