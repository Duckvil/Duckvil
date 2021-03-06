#include "TestSystem/TestSystem.h"

#include <cstdio>

#include "Event/ImmediatePool.h"

namespace Duckvil { namespace LOL {

    Test::Test()
    {
        m_iA = 20;
    }

    Test::Test(
        const Memory::FreeList& _heap,
        RuntimeReflection::__data* _pReflectionData,
        RuntimeReflection::__recorder_ftable* _pReflectionRecorderFTable,
        RuntimeReflection::__ftable* _pReflectionFTable
    )
    {
        // Event::Channel<test> aa(_heap, _pReflectionData);
        // // Event::Channel<test2> aa2(_heap, _pReflectionData);

        // aaa* aaaa = new aaa();
        // aaa2* aaaa2 = new aaa2();

        // // aa.Add(this);
        // aa.Add(aaaa);
        // aa.Add(aaaa2);

        // aa.Remove(aaaa);

        // aa.Broadcast(test{  });

        // // aa2.Add(this);

        // // aa2.Broadcast(test2{  });

        Event::Pool<Event::mode::mixed> _pool(_heap, _pReflectionFTable, _pReflectionData);

        _pool.Add<test>(); // When broadcasted, it is invoking the function immediately
        _pool.Add<test2>(this); // When broadcasted, it is added to a buffer, and then we need to get it

        // _pool.Remove<test2>();

        test2 _aa;

        _aa.a = 10;

        test2 _aa2;

        _aa2.a = 40;

        test _aaa;

        _aaa.a = 20;

        test _aaa2;

        _aaa2.a = 30;

        _pool.Broadcast(_aaa);
        _pool.Broadcast(_aa2);
        _pool.Broadcast(_aa);
        _pool.Broadcast(_aaa2);

        test2 _mess;
        test _mess2;

        while(_pool.AnyEvents())
        {
            if(_pool.GetMessage(&_mess))
            {
                _pool.EventHandled<test2>();
            }
            // else if(_pool.GetMessage(&_mess2))
            // {
            //     _pool.EventHandled<test>();
            // }
        }

        _pool.Reset();

        while(_pool.AnyEvents())
        {
            if(_pool.GetMessage(&_mess2))
            {
                if(_mess2.a == 30)
                {
                    _pool.EventHandled<test>();
                    // _pool.EventHandled(_mess2);
                }
                else
                {
                    // _pool.EventHandled(_mess2);
                    _pool.EventHandled<test>();
                }
            }
        }

        _pool.Reset();

        while(_pool.AnyEvents())
        {
            if(_pool.GetMessage(&_mess2))
            {
                _pool.EventHandled<test>();
            }
        }

        printf("VVV\n");

        {
            struct xd
            {
                void lol(const test& _event)
                {

                }
            };

            Event::Pool<Event::mode::immediate> _pool2(_heap, _pReflectionFTable, _pReflectionData);

            _pool2.AddA<test>([](const test& _event)
            {

            });

            _pool2.Add<test>(new xd(), &xd::lol);

            try
            {
                Event::Channel<test, Event::mode::immediate>& c = _pool2.GetChannel<test>();
            }
            catch(std::exception& _what)
            {
                printf("AAAA %s\n", _what.what());
            }
        }
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
        // printf("%i\n", m_iA);
        // printf("AAAAAA\n");
    }

    void Test::InitEditor(void* _pImguiContext, const duckvil_frontend_reflection_context& _context)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void Test::OnDraw()
    {
        ImGui::Begin("XXX");

        ImGui::End();
    }

    void Test::OnEvent(const test& _event)
    {
        printf("AAAAA\n");
    }

    void Test::OnEvent(const test2& _event)
    {
        printf("AAAAA2\n");
    }

    void Test::OnEvent(const HotReloader::HotReloadedEvent& _event)
    {
        printf("AAAAA3\n");
    }

}}