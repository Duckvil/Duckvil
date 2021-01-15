#pragma once

#include "Memory/Memory.h"

#include "Process/Process.h"

#include "Utils/Macro.h"

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_WINDOWS
    void windows_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, data* _pData);
    bool windows_setup(data* _pData);
    void windows_write(data* _pData, const char* _csMessage);
    void windows_start(data* _pData);
    void windows_stop(data* _pData);
#endif

}}