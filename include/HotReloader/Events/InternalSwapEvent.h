#pragma once

#include "HotReloader/RuntimeCompilerSystem.h"

namespace Duckvil { namespace HotReloader {

    struct InternalSwapEvent
    {
        Memory::Vector<RuntimeCompilerSystem::hot_object>* m_pHotObjects;
        duckvil_recorderd_types m_recordedTypes;

        void (*m_fnSwap)(Memory::Vector<RuntimeCompilerSystem::hot_object>*, duckvil_recorderd_types&);
    };

}}