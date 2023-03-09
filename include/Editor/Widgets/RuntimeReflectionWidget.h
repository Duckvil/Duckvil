#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "Engine/Time.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "HotReloader/ReflectionFlags.h"

#include "DependencyInjection/Inject.h"

#include "Editor/Widgets/RuntimeReflectionWidget.generated.h"

namespace Duckvil { namespace Editor {

    class RuntimeReflectionWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;

        Memory::Vector<RuntimeReflection::__duckvil_resource_type_t> m_aTypes;

        int m_uiCurrentItem;
        int m_uiCurrentFunctionItem;
        int m_uiCurrentPropertyItem;

    public:
        RuntimeReflectionWidget();
        RuntimeReflectionWidget(
            DUCKVIL_ARGUMENT("Size" = 1024 * 1024)
            const Memory::FreeList& _heap
        );
        ~RuntimeReflectionWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}