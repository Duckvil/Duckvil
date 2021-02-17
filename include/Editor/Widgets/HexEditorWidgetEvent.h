#pragma once

#include "Memory/MemoryDebugger.h"

namespace Duckvil { namespace Editor {

    struct HexEditorWidgetInitEvent
    {
        duckvil_memory_debug_info* m_pDebugInfo;
    };

}}