#include "PlugNPlay/Module.h"

#include "Memory/LinearAllocator.h"

#include "UniTestFramework/UniTestFramework.h"

#include <cstring>

DUCKVIL_TEST(MainMemoryAllocation)
{
    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::Memory::IMemory* (*_fnInit)();

    Duckvil::PlugNPlay::module_init(&_module);

    DUCKVIL_TEST_IS_NOT_NULL((void*)_module.load, "Module load function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)_module.get, "Module get function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)_module.free, "Module free function is not loaded");

    printf("Trying to load: %s\n", _memoryModule.m_sName);

    _module.load(&_memoryModule);
    DUCKVIL_TEST_IS_NOT_NULL(_memoryModule.m_pModule, "Memory module is NULL");

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&_fnInit);
    DUCKVIL_TEST_IS_NOT_NULL((void*)_fnInit, "Memory module is NULL");

    Duckvil::Memory::IMemory* memory = _fnInit();
    Duckvil::Memory::__linear_allocator memoryChunk;

    _module.free(&_memoryModule);

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    return DUCKVIL_TEST_FAILED;
}