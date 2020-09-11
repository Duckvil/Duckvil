#include "test/PlugNPlay/Module.h"

DUCKVIL_TEST(Module)
{
    static Duckvil::PlugNPlay::module _module;
    static Duckvil::PlugNPlay::__module_information _memoryModule("Memory.dll");
    static void* _fnInit;

    Duckvil::PlugNPlay::module_init(&_module);

    DUCKVIL_TEST_IS_NOT_NULL((void*)_module.load, "Module load function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)_module.get, "Module get function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)_module.free, "Module free function is not loaded");

    _module.load(&_memoryModule);
    DUCKVIL_TEST_IS_NOT_NULL(_memoryModule.m_pModule, "Memory module is NULL");

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&_fnInit);
    DUCKVIL_TEST_IS_NOT_NULL((void*)_fnInit, "Memory module is NULL");

    _module.free(&_memoryModule);
    DUCKVIL_TEST_IS_NULL(_memoryModule.m_pModule, "Failed to free memory module");

    DUCKVIL_TEST_SUCCESS_PASS;
}