#pragma once

#include "RuntimeReflection/Markers.h"

#include "ProjectManager/ProjectManager.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Meta.h"

#include "Memory/FreeList.h"
#include "Memory/Vector.h"

namespace {$projectName} {

    DUCKVIL_CLASS(Duckvil::ProjectManager::project_type::standalone)
    class Project
    {
    private:
        Duckvil::Memory::FreeList m_heap;
        Duckvil::Memory::Vector<duckvil_recorderd_types> m_aSystems;

    public:
        Project(const Duckvil::Memory::FreeList& _heap, const Duckvil::Memory::Vector<duckvil_recorderd_types>& _aSystems);
        ~Project();

        bool Init();
    };

}