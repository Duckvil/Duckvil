#include "test/TestMemory/Module.h"

#include "UniTestFramework/UniTestFramework.h"

Duckvil::PlugNPlay::__module __duckvil_global::m_module;
Duckvil::PlugNPlay::__module_information __duckvil_global::m_memoryModule = Duckvil::PlugNPlay::__module_information("Memory");
Duckvil::Memory::IMemory* __duckvil_global::m_pMemoryInterface;
void* __duckvil_global::m_pMemoryChunk;
void* __duckvil_global::m_pHeap;
Duckvil::Memory::FreeList __duckvil_global::m_freeList;

DUCKVIL_TEST(Module)
{
    Duckvil::PlugNPlay::module_init(&__duckvil_global::m_module);

    DUCKVIL_TEST_IS_NOT_NULL((void*)__duckvil_global::m_module.load, "Module load function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)__duckvil_global::m_module.get, "Module get function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)__duckvil_global::m_module.free, "Module free function is not loaded");

    printf("Trying to load: %s\n", __duckvil_global::m_memoryModule.m_sName.m_sText);

    __duckvil_global::m_module.load(&__duckvil_global::m_memoryModule);
    DUCKVIL_TEST_IS_NOT_NULL(__duckvil_global::m_memoryModule.m_pModule, "Memory __module is NULL");

    Duckvil::Memory::IMemory* (*_fnMemoryInit)();

    __duckvil_global::m_module.get(__duckvil_global::m_memoryModule, "duckvil_memory_init", (void**)&_fnMemoryInit);
    DUCKVIL_TEST_IS_NOT_NULL((void*)_fnMemoryInit, "Memory __module is NULL");

    __duckvil_global::m_pMemoryInterface = _fnMemoryInit();

    DUCKVIL_TEST_SUCCESS_PASS;
}