#pragma once

#include "Process/Process.h"

#include "Memory/Memory.h"

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_LINUX
    void linux_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, data* _pData);
    bool linux_setup(data* _pData);
    void linux_write(data* _pData, const char* _csMessage);
    void linux_start(data* _pData);
    void linux_stop(data* _pData);
#endif

}}