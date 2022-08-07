#pragma once

#include "RuntimeReflection/Markers.h"

#include "ProjectManager/ProjectManager.h"
#include "ProjectManager/Script.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "Memory/FreeList.h"
#include "Memory/Vector.h"

#include "ProjectManager/Events/AddScriptEvent.h"

#include "Serializer/EntitySerializerSystem.h"

#include "Editor/Widget.h"

#undef GetObject
#undef min

namespace {$projectName} {

    DUCKVIL_CLASS(Duckvil::ProjectManager::project_type::standalone)
    class Project : public Duckvil::Editor::Widget
    {
    private:
        Duckvil::Memory::FreeList m_heap;
        const Duckvil::Memory::ThreadsafeVector<duckvil_recorderd_types>* m_pSystems;
        Duckvil::PlugNPlay::__module_information m_module;

        Duckvil::Memory::Vector<Duckvil::HotReloader::ITrackKeeper*> m_pScripts;

        Duckvil::Serializer::EntitySerializerSystem* m_pSerializer;

        Duckvil::Event::Pool<Duckvil::Event::mode::immediate> m_eventPool;

        Duckvil::EntityFactory* m_pEntityFactory;

    public:
        Project(const Duckvil::Memory::FreeList& _heap, const Duckvil::Memory::ThreadsafeVector<duckvil_recorderd_types>* _pSystems, const Duckvil::PlugNPlay::__module_information& _module, Duckvil::EntityFactory* _pEntityFactory);
        ~Project();

        bool Init(Duckvil::Event::Pool<Duckvil::Event::mode::immediate>* _pPMEventPool);
        void Update();

        void InitEditor(void* _pImguiContext);
        void OnDraw();
        void OnMainMenuDraw();
    };

}