#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"
#include "PlugNPlay/Plugin.h"

#include "Memory/Vector.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "Memory/SlotArray.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

#define slot(T, ...) \
    struct T \
        __VA_ARGS__ \
    ; \
    DUCKVIL_SLOT_ARRAY_DECLARE(T)

slot(test, 
{
    int a;
});

struct test_t
{
    DUCKVIL_SLOT_ARRAY(test) m_test;
};

bool duckvil_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    test_t* _data = (test_t*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, 0, sizeof(test_t), alignof(test_t));

    _data->m_test = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, test);

    return true;
}

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

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 2048);
    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 1024);

    duckvil_init(_memoryInterface, _free_list);

    // Duckvil::RuntimeDatabase::__data* _db = (Duckvil::RuntimeDatabase::__data*)Duckvil::PlugNPlay::instantiate_plugin(_module, _dbModule);

    // _db->m_pAllocator = (Duckvil::Memory::__free_list_allocator*)1;
    // Duckvil::RuntimeDatabase::__data* _db2 = (Duckvil::RuntimeDatabase::__data*)Duckvil::PlugNPlay::instantiate_plugin(_module, _dbModule);

    return 0;
}