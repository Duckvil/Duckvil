#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Engine/Time.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "RuntimeReflection/Markers.h"

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

        int m_iSelectedProject;

        Memory::Vector<Utils::string> m_aProjects;

    public:
        LoadProjectWidget();
        LoadProjectWidget(const Memory::FreeList& _heap);
        ~LoadProjectWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void SetEditorEventPool(const Event::Pool<Event::mode::immediate>* _pEventPool);
    };

}}