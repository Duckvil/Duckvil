#pragma once

#include "RuntimeReflection/Recorder.h"

#include "Memory/Vector.h"

namespace Duckvil { namespace ProjectManager {

    struct project
    {
        Memory::Vector<duckvil_recorderd_types> m_aTypes;
        void* m_pObject;
    };

}}