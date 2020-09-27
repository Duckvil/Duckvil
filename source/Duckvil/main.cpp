#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"

#include "Memory/Vector.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::AutoLoader _auto(DUCKVIL_OUTPUT);

    _auto.LoadAll();

    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _dbModule("RuntimeDatabase");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);
    _module.load(&_dbModule);

    Duckvil::Memory::init_callback duckvil_memory_init;
    Duckvil::RuntimeDatabase::__data* (*duckvil_runtimedatabase_init)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);
    _module.get(_dbModule, "duckcvil_runtimedatabase_init", (void**)&duckvil_runtimedatabase_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024);
    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512);

    Duckvil::RuntimeDatabase::__data* _db = duckvil_runtimedatabase_init(_memoryInterface, _free_list);

    return 0;
}