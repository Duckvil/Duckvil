#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Recorder.h"

#include "Memory/FreeList.h"

#include "Event/AnyPool.h"

#include "HotReloader/HotObject.h"
#include "HotReloader/ReflectionFlags.h"

#include "HotReloader/Events/HotReloadEvent.h"

#include "imgui/imgui.h"

#include "Editor/Widget.h"

#include "TestSystem/TestSystem.generated.h"

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

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_UserSystem, HotReloader::ReflectionFlags_Hot)
    class Test : public ISystem, public Editor::Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        int m_iA;

    public:
        Test();
        Test(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData, RuntimeReflection::__recorder_ftable* _pReflectionRecorderFTable, RuntimeReflection::__ftable* _pReflectionFTable);
        ~Test();

        bool Init();
        void Update();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void OnEvent(const test& _event);
        void OnEvent(const test2& _event);
        void OnEvent(const HotReloader::HotReloadedEvent& _event);
    };

}}