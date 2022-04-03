#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Recorder.h"

#include "Memory/FreeList.h"

#include "Event/AnyPool.h"

#include "HotReloader/HotObject.h"
#include "HotReloader/ReflectionFlags.h"

#include "imgui/imgui.h"

#include "Editor/Widget.h"

#include "TestSystem/TestSystem.generated.h"

namespace Duckvil { namespace LOL {

    // struct test
    // {
    //     int a;
    // };

    // struct test2
    // {
    //     int a;
    // };

    // struct aaa
    // {
    //     void OnEvent(const test& _event)
    //     {
    //         printf("NOOOOOOO\n");
    //     }
    // };

    // struct aaa2
    // {
    //     void OnEvent(const test& _event)
    //     {
    //         printf("NOOOOOOO2\n");
    //     }
    // };

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_UserSystem)
    class Test : public ISystem, public Editor::Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        int m_iA;

    public:
        Test();
        Test(const Memory::FreeList& _heap);
        ~Test();

        bool Init();
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}