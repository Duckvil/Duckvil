#include "UniTestFramework/UniTestFramework.h"

#include "PlugNPlay/Module.h"

#include <cstring>

static Duckvil::PlugNPlay::__module g_module;
static Duckvil::PlugNPlay::__module_information g_memoryModule("Memory");
static void* g_fnInit;

DUCKVIL_TEST(ModuleLoad)
{
    Duckvil::PlugNPlay::module_init(&g_module);

    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.load, "Module load function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.get, "Module get function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.free, "Module free function is not loaded");

    printf("Trying to load: %s\n", g_memoryModule.m_sName.m_sText);

    g_module.load(&g_memoryModule);
    DUCKVIL_TEST_IS_NOT_NULL(g_memoryModule.m_pModule, "Memory __module is NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(ModuleFunctionPointers)
{
    g_module.get(g_memoryModule, "duckvil_memory_init", (void**)&g_fnInit);
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_fnInit, "Memory init function is NULL");

    DUCKVIL_TEST_EXP(g_module.get(g_memoryModule, "not_valid_function", (void**)&g_fnInit) == false, "Should not happen");
    DUCKVIL_TEST_IS_NULL((void*)g_fnInit, "Memory init function is not NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(ModuleRelease)
{
    g_module.free(&g_memoryModule);
    DUCKVIL_TEST_IS_NULL(g_memoryModule.m_pModule, "Failed to free memory __module");

    DUCKVIL_TEST_SUCCESS_PASS;
}

Duckvil::PlugNPlay::__module_information g_memoryFailModule("NotExists");

DUCKVIL_TEST(ModuleLoadFail)
{
    printf("Trying to load: %s\n", g_memoryFailModule.m_sName.m_sText);

    g_module.load(&g_memoryFailModule);
    DUCKVIL_TEST_IS_NULL(g_memoryFailModule.m_pModule, "Memory __module is not NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(ModuleGetFail)
{
    void* _fnMemoryInit;

    DUCKVIL_TEST_EXP(g_module.get(g_memoryFailModule, "whatever", (void**)&_fnMemoryInit) == false, "Loaded function, but how!?");
    DUCKVIL_TEST_IS_NULL((void*)_fnMemoryInit, "Memory __module is not NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(ModuleFreeFail)
{
    DUCKVIL_TEST_EXP(g_module.free(&g_memoryFailModule) == false, "LOL");
    DUCKVIL_TEST_IS_NULL(g_memoryModule.m_pModule, "Failed to free memory __module");

    DUCKVIL_TEST_SUCCESS_PASS;
}