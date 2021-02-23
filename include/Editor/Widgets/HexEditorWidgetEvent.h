#pragma once

#include "Memory/MemoryDebugger.h"

namespace Duckvil { namespace Editor {

    struct HexEditorWidgetInitEvent
    {
#ifdef DUCKVIL_MEMORY_DEBUGGER
        duckvil_memory_debug_info* m_pDebugInfo;
#endif
    };

}}