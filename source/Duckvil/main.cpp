#include <cstdio>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "Memory/Memory.h"
#include "Memory/LinearAllocator.h"
#include "Memory/FixedStackAllocator.h"
#include "Memory/StackAllocator.h"
#include "Memory/FixedQueueAllocator.h"

#include "PlugNPlay/Module.h"

#include "UniTestFramework/UniTestFramework.h"

Duckvil::PlugNPlay::module g_module;
Duckvil::Memory::IMemory* (*g_fnInit)();
Duckvil::PlugNPlay::__module_information g_memoryModule("Memory.dll");

DUCKVIL_TEST(Module)
{
    Duckvil::PlugNPlay::module_init(&g_module);

    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.load, "Module load function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.get, "Module get function is not loaded");
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_module.free, "Module free function is not loaded");

    g_module.load(&g_memoryModule);
    DUCKVIL_TEST_IS_NOT_NULL(g_memoryModule.m_pModule, "Memory module is NULL");

    g_module.get(g_memoryModule, "duckvil_memory_init", (void**)&g_fnInit);
    DUCKVIL_TEST_IS_NOT_NULL((void*)g_fnInit, "Memory module is NULL");

    DUCKVIL_TEST_SUCCESS_PASS;
}

DUCKVIL_TEST(Memory)
{
    Duckvil::Memory::IMemory* memory = g_fnInit();
    Duckvil::Memory::__linear_allocator memoryChunk = { 0 };

    int a = 10;

    memory->m_fnBasicAllocate(&memoryChunk, 1024);

    Duckvil::Memory::__linear_allocator* otherLinear = memory->m_fnAllocateLinearAllocator(&memoryChunk, 16);
    DUCKVIL_TEST_IS_NOT_NULL(otherLinear, "Failed to allocate linear allocator");
    Duckvil::Memory::__linear_allocator* otherLinear2 = memory->m_fnAllocateLinearAllocator(&memoryChunk, 64);
    DUCKVIL_TEST_IS_NOT_NULL(otherLinear, "Failed to allocate linear allocator");

    DUCKVIL_TEST_IS_NOT_NULL(Duckvil::Memory::linear_allocate(memory, otherLinear, true), "Failed to allocate bool");
    DUCKVIL_TEST_IS_NOT_NULL(Duckvil::Memory::linear_allocate(memory, otherLinear, true), "Failed to allocate bool");
    DUCKVIL_TEST_IS_NOT_NULL(Duckvil::Memory::linear_allocate(memory, otherLinear, true), "Failed to allocate bool");
    DUCKVIL_TEST_IS_NOT_NULL(Duckvil::Memory::linear_allocate(memory, otherLinear, true), "Failed to allocate bool");

    int* res = (int*)memory->m_fnLinearAllocate_(otherLinear, &a, sizeof(int), alignof(int));
    DUCKVIL_TEST_IS_NOT_NULL(res, "Failed to allocate memory");
    DUCKVIL_TEST_EQUAL(res, a, "Wrong variable in memory");

    memory->m_fnLinearAllocate_(otherLinear, &a, sizeof(int), alignof(int));
    memory->m_fnLinearAllocate_(otherLinear, &a, sizeof(int), alignof(int));

    res = (int*)memory->m_fnLinearAllocate_(otherLinear, &a, sizeof(int), alignof(int));
    DUCKVIL_TEST_IS_NULL(res, "Pointer should be NULL, overflow detected");

    int* res4 = Duckvil::Memory::linear_allocate(memory, otherLinear2, a);
    const char* res3 = Duckvil::Memory::linear_allocate(memory, memoryChunk, "aaaa");
    int* res2 = Duckvil::Memory::linear_allocate(memory, memoryChunk, 20);

    {
        Duckvil::Memory::FixedStack<int> _stack_allocator(memory, &memoryChunk, 2);

        bool res = _stack_allocator.Empty();
        _stack_allocator.Allocate(10);
        res = _stack_allocator.Empty();
        _stack_allocator.Allocate(33);
        res = _stack_allocator.Full();

        Duckvil::Memory::__fixed_stack_allocator* stackAllocator = memory->m_fnAllocateFixedStackAllocator(&memoryChunk, 64, sizeof(int));

        Duckvil::Memory::fixed_stack_allocate(memory, stackAllocator, 10);
        Duckvil::Memory::fixed_stack_allocate(memory, stackAllocator, 33);

        int* top = (int*)memory->m_fnFixedStackTop_(stackAllocator);

        DUCKVIL_TEST_IS_NOT_NULL(top, "Failed to allocate on stack");
        DUCKVIL_TEST_EQUAL(*top, 33, "Wrong allocation");

        memory->m_fnFixedStackPop_(stackAllocator);
        top = (int*)memory->m_fnFixedStackTop_(stackAllocator);

        DUCKVIL_TEST_IS_NOT_NULL(top, "Failed to allocate on stack");
        DUCKVIL_TEST_EQUAL(*top, 10, "Wrong allocation");

        memory->m_fnFixedStackPop_(stackAllocator);
        top = (int*)memory->m_fnFixedStackTop_(stackAllocator);
        memory->m_fnFixedStackPop_(stackAllocator);

        DUCKVIL_TEST_EXP(stackAllocator->used == 0 && stackAllocator->memory[0] == 0, "Pop gone wrong");
    }

    {
        Duckvil::Memory::__stack_allocator* stackAllocator2 = memory->m_fnAllocateStackAllocator(&memoryChunk, 128);
        int b = 33;

        const char* check = (const char*)memory->m_fnStackAllocateCStr_(stackAllocator2, "xd");
        int* top = (int*)memory->m_fnStackAllocate_(stackAllocator2, &a, sizeof(int), alignof(int));
        check = (const char*)memory->m_fnStackAllocateCStr_(stackAllocator2, "aaaa");
        top = (int*)memory->m_fnStackAllocate_(stackAllocator2, &b, sizeof(int), alignof(int));

        int* top2 = (int*)memory->m_fnStackTop_(stackAllocator2);

        memory->m_fnStackPop_(stackAllocator2);
        const char* top3 = (const char*)memory->m_fnStackTop_(stackAllocator2);

        memory->m_fnStackPop_(stackAllocator2);
        top = (int*)memory->m_fnStackTop_(stackAllocator2);

        memory->m_fnStackPop_(stackAllocator2);
        top3 = (const char*)memory->m_fnStackTop_(stackAllocator2);

        memory->m_fnStackPop_(stackAllocator2);
        top = (int*)memory->m_fnStackAllocate_(stackAllocator2, &a, sizeof(int), alignof(int));

        top = (int*)memory->m_fnStackTop_(stackAllocator2);
        DUCKVIL_TEST_IS_NOT_NULL(top, "Stack allocation failed");
    }

    {
        Duckvil::Memory::FixedQueue<int> alloc(memory, &memoryChunk, 2);

        int x = 10;
        int xx = 20;
        int xxx = 30;
        int xxxx = 40;

        bool xc = alloc.Empty();

        alloc.Allocate(10);
        xc = alloc.Empty();
        alloc.Allocate(20);
        xc = alloc.Full();
        int res = alloc.Begin();
        alloc.Pop();
        alloc.Allocate(30);
        res = alloc.Begin();
        alloc.Pop();
        res = alloc.Begin();

        printf("AAAA\n");
    }

    {
        Duckvil::Memory::__fixed_queue_allocator* _queueAllocator = memory->m_fnAllocateFixedQueueAllocator(&memoryChunk, 64, sizeof(int));

        int x = 10;
        int xx = 20;

        memory->m_fnFixedQueueAllocate_(_queueAllocator, &x, sizeof(int), alignof(int));
        memory->m_fnFixedQueueAllocate_(_queueAllocator, &xx, sizeof(int), alignof(int));
        int* res = (int*)memory->m_fnFixedQueueBegin_(_queueAllocator);
        memory->m_fnFixedQueuePop_(_queueAllocator);
        res = (int*)memory->m_fnFixedQueueBegin_(_queueAllocator);
        memory->m_fnFixedQueuePop_(_queueAllocator);
        memory->m_fnFixedQueueAllocate_(_queueAllocator, &x, sizeof(int), alignof(int));
        memory->m_fnFixedQueueAllocate_(_queueAllocator, &xx, sizeof(int), alignof(int));
        res = (int*)memory->m_fnFixedQueueBegin_(_queueAllocator);
        memory->m_fnFixedQueuePop_(_queueAllocator);
        memory->m_fnFixedQueueAllocate_(_queueAllocator, &x, sizeof(int), alignof(int));
        res = (int*)memory->m_fnFixedQueueBegin_(_queueAllocator);
        memory->m_fnFixedQueuePop_(_queueAllocator);
        res = (int*)memory->m_fnFixedQueueBegin_(_queueAllocator);

        printf("AAAA\n");
    }

    {
        Duckvil::Memory::__queue_allocator* _queueAllocator = memory->m_fnAllocateQueueAllocator(&memoryChunk, 64);

        int x = 10;
        double xx = 20.f;

        memory->m_fnQueueAllocate_(_queueAllocator, &x, sizeof(int), alignof(int));
        memory->m_fnQueueAllocate_(_queueAllocator, &xx, sizeof(double), alignof(double));
        int* res = (int*)memory->m_fnQueueBegin_(_queueAllocator);
        memory->m_fnQueuePop_(_queueAllocator);
        double* res2 = (double*)memory->m_fnQueueBegin_(_queueAllocator);
        memory->m_fnQueuePop_(_queueAllocator);
        memory->m_fnQueueAllocate_(_queueAllocator, &x, sizeof(int), alignof(int));
        memory->m_fnQueueAllocate_(_queueAllocator, &xx, sizeof(double), alignof(double));
        res = (int*)memory->m_fnQueueBegin_(_queueAllocator);
        memory->m_fnQueuePop_(_queueAllocator);
        memory->m_fnQueueAllocate_(_queueAllocator, &x, sizeof(int), alignof(int));
        res2 = (double*)memory->m_fnQueueBegin_(_queueAllocator);
        memory->m_fnQueuePop_(_queueAllocator);
        res = (int*)memory->m_fnQueueBegin_(_queueAllocator);

        printf("AAAA\n");
    }

    for(uint32_t i = 0; i < 1024;)
    {
        for(uint32_t j = 0; j < 8; i++, j++)
        {
            printf("%02x ", memoryChunk.memory[i]);
        }

        printf("\n");
    }

    memory->m_fnLinearClear(memoryChunk);
    DUCKVIL_TEST_EXP(memoryChunk.used == 0 && memoryChunk.memory[0] == 0, "Failed to clear memory");

    g_module.free(&g_memoryModule);
    DUCKVIL_TEST_IS_NULL(g_memoryModule.m_pModule, "Failed to free loaded module");

    DUCKVIL_TEST_SUCCESS_PASS;
}

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    return 0;
}