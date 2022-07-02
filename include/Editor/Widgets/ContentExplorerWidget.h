#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Event/ImmediatePool.h"

#include "Editor/Widgets/ContentExplorerWidget.Generated.h"

namespace Duckvil { namespace Editor {

    class ContentExplorerWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;

        Utils::string m_sPath;
        bool m_bLoaded;

        bool m_bCreateDirectoryPopup;

        Utils::string m_sSelectedPath;
        Utils::string m_sProjectName;

    public:
        ContentExplorerWidget();
        ContentExplorerWidget(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Engine") Event::Pool<Event::mode::immediate>* _pEngineEventPool);
        ~ContentExplorerWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}