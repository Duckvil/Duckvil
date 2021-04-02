#include "TestSystem/TestSystem.h"

#include <cstdio>

#include "Event/ImmediatePool.h"

#include "Thread/ThreadPool.h"

namespace Duckvil { namespace LOL {

    Test::Test()
    {
        m_iA = 20;
    }

    Test::Test(const Memory::FreeList& _heap)
    {
        // PlugNPlay::__module _module;

        // PlugNPlay::module_init(&_module);

        // _module.load(&_threadModule);

        // Thread::pool_ftable* (*init)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator);

        // _module.get(_threadModule, "duckvil_thread_pool_init", (void**)&init);

        // Thread::pool_ftable* _thread = init(_heap.GetMemoryInterface(), _heap.GetAllocator());
        // Thread::pool_data* _threadData = _thread->m_fnInit(_heap);

        // _thread->m_fnStart(_threadData);

        // auto _res = Thread::order_task(_thread, _threadData, [](int a)
        // {
        //     int _res = 0;

        //     for(uint32_t i = 0; i < 1000000000; ++i)
        //     {
        //         // _res += i * a;
        //     }

        //     printf("aaaaaaaaaaaaaaaaaa %d", _res);

        //     return _res;
        // }, 2);

        // _res = Thread::order_task(_thread, _threadData, [](int a)
        // {
        //     int _res = 0;

        //     for(uint32_t i = 0; i < 1000000000; ++i)
        //     {
        //         // _res += i * a;
        //     }

        //     printf("cccccccccccccc %d", _res);

        //     return _res;
        // }, 2);

        // _res = Thread::order_task(_thread, _threadData, [](int a)
        // {
        //     int _res = 0;

        //     for(uint32_t i = 0; i < 1000000000; ++i)
        //     {
        //         // _res += i * a;
        //     }

        //     printf("ddddddddddddddddd %d", _res);

        //     return _res;
        // }, 2);

        // _res = Thread::order_task(_thread, _threadData, [](int a)
        // {
        //     int _res = 0;

        //     for(uint32_t i = 0; i < 1000000000; ++i)
        //     {
        //         // _res += i * a;
        //     }

        //     printf("azzzzzzzzzzzzzzzzzzzzz %d", _res);

        //     return _res;
        // }, 2);

        // _res = Thread::order_task(_thread, _threadData, [](int a)
        // {
        //     int _res = 0;

        //     for(uint32_t i = 0; i < 1000000000; ++i)
        //     {
        //         // _res += i * a;
        //     }

        //     printf("ggggggggggggggggggg %d", _res);

        //     return _res;
        // }, 2);

        // _res = Thread::order_task(_thread, _threadData, [](int a)
        // {
        //     int _res = 0;

        //     for(uint32_t i = 0; i < 2000000000; ++i)
        //     {
        //         // _res += i * a;
        //     }

        //     printf("hhhhhhhhhhhhhhhh %d", _res);

        //     return _res;
        // }, 2);

        // _res.wait();

        // // printf("%d bbbbbbbbbbbbbbbbbbbbb", _res.get());

        // _thread->m_fnTerminate(_threadData);
    }

    Test::~Test()
    {

    }

    bool Test::Init()
    {
        m_iA = 100;

        return true;
    }

    void Test::Update()
    {
        // DUCKVIL_LOG_INFO(m_pLogger, m_pLoggerData, "axxxa %i", m_iA);
        // printf("%ii\n", m_iA);
        // printf("AAAAAA\n");

        // DUCKVIL_LOG_INFO("a %i", m_iA);
    }

    void Test::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void Test::OnDraw()
    {
        ImGui::Begin("XXX");

        ImGui::Text("AAA");

        ImGui::End();
    }

}}