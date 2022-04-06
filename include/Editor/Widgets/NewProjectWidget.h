#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Engine/Time.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "HotReloader/ReflectionFlags.h"

#include "Editor/ReflectionFlag.h"

#include "Event/ImmediatePool.h"

#include "Engine/ISystem.h"

#include "ProjectManager/ProjectManager.h"

#include "Editor/Widgets/NewProjectWidget.generated.h"

namespace Duckvil { namespace Editor {

    DUCKVIL_CLASS(ReflectionFlag::Spwanable)
    class NewProjectWidget : public Widget, public ISystem
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;

        const Event::Pool<Event::mode::immediate>* m_pEditorEventPool;

        char* m_sProjectName;

        int m_iSelectedType;

        static const char* ms_saTypes[];
        static const uint32_t ms_uiTypesCount;

        ProjectManager::ftable* m_pProjectManager;
        ProjectManager::data* m_pProjectManagerData;

    public:
        NewProjectWidget();
        NewProjectWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Editor") const Event::Pool<Event::mode::immediate>* _pEditorEventPool, ProjectManager::ftable* _pProjectManager, ProjectManager::data* _pProjectManagerData);
        ~NewProjectWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        bool Init();
        void Update(double _dDelta);

        // void SetEditorEventPool(const Event::Pool<Event::mode::immediate>* _pEventPool);
    };

}}