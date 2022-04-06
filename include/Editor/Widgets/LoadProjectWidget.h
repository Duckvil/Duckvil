#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Engine/Time.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "HotReloader/ReflectionFlags.h"

#include "Editor/ReflectionFlag.h"

#include "Event/ImmediatePool.h"

#include "Editor/Widgets/LoadProjectWidget.generated.h"

namespace Duckvil { namespace Editor {

    DUCKVIL_CLASS(ReflectionFlag::Spwanable)
    class LoadProjectWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;

        const Event::Pool<Event::mode::immediate>* m_pEditorEventPool;
        const Event::Pool<Event::mode::immediate>* m_pEngineEventPool;

        int m_iSelectedProject;

        Memory::Vector<Utils::string> m_aProjects;

    public:
        LoadProjectWidget();
        LoadProjectWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Editor") const Event::Pool<Event::mode::immediate>* _pEventPool, DUCKVIL_ARGUMENT("Engine") const Event::Pool<Event::mode::immediate>* _pEngineEventPool);
        ~LoadProjectWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}