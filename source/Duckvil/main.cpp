#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "Memory/LinearAllocator.h"
#include "Memory/Internal/FreeListAllocator.h"

#include "PlugNPlay/Module.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

struct _test
{
    std::size_t test1;
    std::size_t test2;
    std::size_t test3;
    std::size_t test4;
    std::size_t test5;
};

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::Memory::IMemory* (*_fnMemoryInit)();
    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::Memory::IMemory* _pMemoryInterface;

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);
    _module.get(_memoryModule, "duckvil_memory_init", (void**)&_fnMemoryInit);

    _pMemoryInterface = _fnMemoryInit();

    Duckvil::Memory::__linear_allocator _main;

    _pMemoryInterface->m_fnBasicAllocate(&_main, 1024);

    Duckvil::Memory::__free_list_allocator* _free = _pMemoryInterface->m_fnAllocateFreeListAllocator(&_main, 512);

    _test _t;

    void* _d = _pMemoryInterface->m_fnFreeListAllocate_(_free, &_t, sizeof(_test), alignof(_test));

    return 0;
}