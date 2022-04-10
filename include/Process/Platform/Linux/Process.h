#pragma once

#include "Process/Process.h"

#include "Memory/Memory.h"

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_LINUX
    void linux_init(Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData);
    bool linux_setup(data* _pData);
    void linux_write(data* _pData, const char* _csMessage);
    void linux_start(data* _pData);
    void linux_stop(data* _pData);
    void linux_wait(data* _pData);
    bool linux_terminate(data* _pData);
    void linux_cleanup(Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData);
#endif

}}