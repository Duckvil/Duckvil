#include "test/TestModule/Module.h"

#include <cstring>

char buffer[256] = { 0 };

static Duckvil::PlugNPlay::module g_module;
static Duckvil::PlugNPlay::__module_information g_memoryModule(buffer);
static void* g_fnInit;

DUCKVIL_TEST(ModuleLoad)
{
    strncpy(buffer, DUCKVIL_OUTPUT, sizeof(buffer));
    strncat(buffer, "/Memory.dll", sizeof(buffer));

    Duckvil::PlugNPlay::module_init(&g_module);

    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.load, "Module load function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.get, "Module get function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.free, "Module free function is not loaded");

    g_module.load(&g_memoryModule);
    DUCKVIL_TEST_IS_NOT_NULL(g_memoryModule.m_pModule, "Memory module is NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(ModuleFunctionPointers)
{
    g_module.get(g_memoryModule, "duckvil_memory_init", (void**)&g_fnInit);
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_fnInit, "Memory module is NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(ModuleRelease)
{
    g_module.free(&g_memoryModule);
    DUCKVIL_TEST_IS_NULL(g_memoryModule.m_pModule, "Failed to free memory module");

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    return DUCKVIL_TEST_FAILED;
}