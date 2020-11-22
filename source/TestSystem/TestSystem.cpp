#include "TestSystem/TestSystem.h"

#include <cstdio>

namespace Duckvil { namespace LOL {

    Test::Test(int a, const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
        m_iA(a)
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

        Event::Pool _pool(_heap, _pReflectionData);

        _pool.Add<test>(this);
        _pool.Add<test2>(this);

        _pool.Remove<test>();

        _pool.Broadcast(test{});
        _pool.Broadcast(test2{});
    }

    Test::~Test()
    {

    }

    bool Test::Init()
    {
        return true;
    }

    void Test::Update()
    {
        // DUCKVIL_LOG_INFO_(m_pLogger, m_pLoggerData, "AAAAAA");
        // printf("%i\n", m_iA);
    }

    void Test::OnEvent(const test& _event)
    {
        printf("AAAAA\n");
    }

    void Test::OnEvent(const test2& _event)
    {
        printf("AAAAA2\n");
    }

}}