#pragma once

#include "Duckvil/ISystem.h"
#include "Duckvil/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"

#include "Memory/FreeList.h"

#include "Event/Pool.h"

namespace Duckvil { namespace LOL {

    struct test
    {
        int a;
    };

    struct test2
    {
        int a;
    };

    struct aaa
    {
        void OnEvent(const test& _event)
        {
            printf("NOOOOOOO\n");
        }
    };

    struct aaa2
    {
        void OnEvent(const test& _event)
        {
            printf("NOOOOOOO2\n");
        }
    };

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_EngineSystem)
    class Test : public ISystem
    {
    private:
        int m_iA;

    public:
        Test(int a, const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData);
        ~Test();

        bool Init();
        void Update();

        void OnEvent(const test& _event);
        void OnEvent(const test2& _event);
    };

}}